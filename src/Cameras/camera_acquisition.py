#!/usr/bin/env python
# -*- coding: utf-8 -*-
"""
Copyright (c) 2021 Philippe Schmouker

Permission is hereby granted,  free of charge,  to any person obtaining a copy
of this software and associated documentation files (the "Software"),  to deal
in the Software without restriction, including  without  limitation the rights
to use,  copy,  modify,  merge,  publish,  distribute, sublicense, and/or sell
copies of the Software,  and  to  permit  persons  to  whom  the  Software  is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS",  WITHOUT WARRANTY OF ANY  KIND,  EXPRESS  OR
IMPLIED,  INCLUDING  BUT  NOT  LIMITED  TO  THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT  SHALL  THE
AUTHORS  OR  COPYRIGHT  HOLDERS  BE  LIABLE  FOR  ANY CLAIM,  DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT,  TORT OR OTHERWISE, ARISING FROM,
OUT  OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
"""

#=============================================================================
from threading import Event, Thread
import time

from .camera                     import Camera
from src.Utils.circular_buffer   import CircularBuffer
from src.Utils.types             import Frame
from src.Utils.indexed_frame     import IndexedFrame


#=============================================================================
class CameraAcquisition( Thread ):
    """The class description.
    """
    #-------------------------------------------------------------------------
    def __init__(self, camera: Camera) -> None:
        '''Constructor.
        
        CameraAcquisition instances are thread  that  capture
        video frame from cameras at their own pace. They fill
        buffers that can be read by external threads.
        
        Args:
            camera: Camera
                A reference to the associated camera instance.
        '''
        self.camera = camera
        self.period = 1.0 / self.camera.get_fps()
        self.buffer = CircularBuffer( 3 )
        self.sync_event = Event()
        super().__init__( name=f"cam-acq-{camera.get_id()}-thrd" )

    #-------------------------------------------------------------------------
    @property
    def cam_id(self) -> int:
        try:
            return self.camera.cam_id
        except:
            return -1

    #-------------------------------------------------------------------------
    def get_frame(self) -> Frame:
        '''Gets oldest frame present in buffer.
        
        Returns:
            Either a copy of the oldest frame that is 
            present in the frames buffer,  or None in 
            case of any error.
        '''
        try:
            return self.buffer.get_oldest().frame.copy()
        except:
            return None

    #-------------------------------------------------------------------------
    def get_indexed_frame(self) -> IndexedFrame:
        '''Gets oldest frame present in buffer.
        
        Returns:
            Either a copy of the oldest indexed frame 
            that is present in the frames buffer,  or 
            None in case of any error.
        '''
        try:
            return self.buffer.get_oldest().frame
        except:
            return None

    #-------------------------------------------------------------------------
    def run(self) -> None:
        '''The running loop of this thread.
        '''
        first_frame = True
        
        self.keep_on = True
        while self.keep_on:
            
            # gets next frame from camera
            frame = self.camera.read()
            
            # is capture ok?
            if self.keep_on:
                
                # this is the capturing time (or not that far from it)
                current_time = time.perf_counter()
                
                if first_frame:
                    # let's stroe immediatly first captured frame
                    start_time = time.perf_counter()
                    self.buffer.append( IndexedFrame(0, frame) )
                    first_frame = False
                    self.sync_event.set()
                else:
                    # if not first frame,
                    # let's verify its capture time
                    current_index = round( (current_time - start_time) * self.period )

                    # if capture time is ok with period,
                    if current_index <= self.buffer[-2].index + 1:
                        # let's just store the frame
                        self.buffer.append( IndexedFrame(current_index, frame) )
                    else:
                        # if not, let's store an intermediaite frame (the one that is missing)
                        self.buffer.append( IndexedFrame(current_index-1,
                                                         frame // 2 + self.buffer[-2].frame // 2) )
                        # then, let's store the lastly captured frame
                        self.buffer.append( IndexedFrame(current_index, frame) )
            # that's it!

    #-------------------------------------------------------------------------
    def stop(self) -> None:
        '''Definitively stops this thread.
        '''
        self.keep_on = False

#=====   end of   src.Cameras.camera_acquisition   =====#
