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
from src.Utils.circular_buffer           import CircularBuffer
from src.Utils.types                     import Frame
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
        
#===============================================================================
#         self.stop_event.set()
#         start_time = None
#         current_index = 0
#         
#         self.keep_on = True
#         while self.keep_on and self.stop_event.wait( 1 ):
#             
#             # gets next frame from camera
#             frame = self.camera.read()
#             
#             # is capture ok?
#             self.keep_on = frame is not None
#             
#             if self.keep_on:
#                 
#                 # this is the capturing time (or not that far from it)
#                 current_time = time.perf_counter()
#                                 
#                 if start_time is None:
#                     # let's store immediately this first captured frame
#                     start_time = current_time  ##time.perf_counter()
#                     self.buffer.append( IndexedFrame(0, frame) )
#                     self.sync_event.set()
#                     #===========================================================
#                     # print( f"{self.name} - {current_time:7.3f} ({self.buffer[0].index:4d}, {self.buffer[1].index:4d}, {self.buffer[2].index:4d}), {0:4d} / {self.buffer[-2].index + 1:4d}" )
#                     #===========================================================
#                     print( 0 )
#                     last_index = 0
#                 else:
#                     ##current_index = int( (current_time - start_time) * self.fps )
#                     
#                     #===========================================================
#                     # print( f"{self.name}, current_time = {current_time: 7.3f}, current_index = {current_index:4d}, index[-2] = {self.buffer[-2].index:4d}" )
#                     # print( self.name, 'index[0] =', self.buffer[0].index )
#                     # print( self.name, 'index[1] =', self.buffer[1].index )
#                     # print( self.name, 'index[2] =', self.buffer[2].index )
#                     # print( self.name, 'index[-2] =', self.buffer[-2].index )
#                     # print( self.name, 'index[-1] =', self.buffer[-1].index )
#                     #===========================================================
# 
#                     # if capture time is ok with period,
#                     if True:  ##current_index <= self.buffer[-1].index + 1 or not self.buffer.is_full():
#                         #=======================================================
#                         # print( self.name, current_index, '<=', self.buffer[-1].index + 1 )
#                         #=======================================================
#                         
#                         # let's just store the frame
#                         self.buffer.append( IndexedFrame(current_index, frame) )
#                         print( '' if current_index == last_index+1 else '==' if current_index == last_index else '++',
#                                current_index, f"{time.perf_counter():.3f}" )
#                         last_index = current_index
#                     else:
#                         #=======================================================
#                         # print( self.name, current_index, '> ', self.buffer[-1].index + 1 )
#                         #=======================================================
#                         
#                         # if not, let's store an intermediate frame (the one that is missing)
#                         self.buffer.append( IndexedFrame(current_index-1,
#                                                          frame // 2 + self.buffer[-2].frame // 2) )
#                         #=======================================================
#                         # print( f"{self.name} - {current_time:7.3f} ({self.buffer[-2].index:4d}), {current_index-1:4d} //" )
#                         # print( self.name, 'index[0] =', self.buffer[0].index )
#                         # print( self.name, 'index[1] =', self.buffer[1].index )
#                         # print( self.name, 'index[2] =', self.buffer[2].index )
#                         # print( self.name, 'index[-2] =', self.buffer[-2].index )
#                         # print( self.name, 'index[-1] =', self.buffer[-1].index )
#                         #=======================================================
#                         
#                         # then, let's store the lastly captured frame
#                         self.buffer.store( IndexedFrame(current_index, frame) )    
# 
#                     current_index += 1
#                     #===========================================================
#                     # print( f"{self.name} - {current_time:7.3f} ({self.buffer[0].index:4d}, {self.buffer[1].index:4d}, {self.buffer[2].index:4d}), {current_index:4d} / {self.buffer[-2].index + 1:4d}" )
#                     # print( self.name, 'index[0] =', self.buffer[0].index )
#                     # print( self.name, 'index[1] =', self.buffer[1].index )
#                     # print( self.name, 'index[2] =', self.buffer[2].index )
#                     # print( self.name, 'index[-2] =', self.buffer[-2].index )
#                     # print( self.name, 'index[-1] =', self.buffer[-1].index )
#                     #===========================================================
#         
#             ##time.sleep( self.period / 2.0 )
#             time.sleep( 0.002 )
#===============================================================================
                
    #-------------------------------------------------------------------------
    def stop(self) -> None:
        '''Definitively stops this thread.
        '''
        self.stop_event.clear()

#=====   end of   src.Cameras.camera_acquisition   =====#
