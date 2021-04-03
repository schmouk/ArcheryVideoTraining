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
import cv2
from threading   import Event, Thread
import time

from .camera                             import Camera
from src.Buffers.camera_frames_buffer    import CameraFramesBuffer
from src.Utils.indexed_frame             import IndexedFrame


#=============================================================================
class CameraAcquisition( Thread ):
    """The class description.
    """
    #-------------------------------------------------------------------------
    def __init__(self, camera      : Camera,
                       frame_buffer: CameraFramesBuffer) -> None:
        '''Constructor.
        
        CameraAcquisition instances are thread  that  capture
        video frame from cameras at their own pace. They fill
        buffers that can be read by external threads.
        
        Args:
            camera: Camera
                A reference to the associated camera instance.
            frame_buffer: CameraFramesBuffer
                A reference to the camera frames  buffer  that
                is associated with the specified camera.
        '''
        self.camera = camera
        self.buffer = frame_buffer
        self.stop_event = Event()
        self.fps = self.camera.get_fps()
        self.flip_status = True
        super().__init__( name=f"cam-acq-{camera.get_id()}-thrd" )

    #-------------------------------------------------------------------------
    @property
    def cam_id(self) -> int:
        try:
            return self.camera.cam_id
        except:
            return None
    
    @property
    def period(self) -> float:
        return 1.0 / self.fps if self.fps > 0 else 0.0

    #-------------------------------------------------------------------------
    def flip_image(self) -> None:
        '''Modifies the image flipping status of the camera acquisition.
        
        Facing and sides camera views should be  mirrored, 
        while up and back ones should not. This is then an 
        option that is available to the  user  via  a  GUI 
        control. This method acts as a toggle.
        '''
        self.flip_status = not self.flip_status

    #-------------------------------------------------------------------------
    def run(self) -> None:
        '''The running loop of this thread.
        '''
        self.stop_event.set()
        frames_count = 0
        
        while self.stop_event.is_set():
            frm = self.camera.read()

            if frm is not None:
                if self.flip_status:
                    frm = cv2.flip( frm, 1 )
                self.buffer.append( IndexedFrame(frames_count, frm) )
                frames_count += 1
                time.sleep( 0.004 )
                
            else:
                self.stop()

        self.camera.release()
        
    #-------------------------------------------------------------------------
    def stop(self) -> None:
        '''Definitively stops this thread.
        '''
        self.stop_event.clear()

#=====   end of   src.Cameras.camera_acquisition   =====#
