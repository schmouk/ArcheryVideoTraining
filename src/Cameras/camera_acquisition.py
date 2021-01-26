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
from threading import Thread
import time

from src.Cameras.camera                      import Camera
from src.Utils.types                         import Frame
from src.Buffers.frames_acquisition_buffer   import FramesAcquisitionBuffer
from src.Utils.indexed_frame                 import IndexedFrame

#=============================================================================
class CameraAcquisition( Thread ):
    """The class of camera acquisition threads.
    """
    
    #-------------------------------------------------------------------------
    def __init__(self, camera: Camera, name: str = None) -> None:
        '''Constructor.
        
        Args:
            camera: Camera
                A reference to the associated camera.
            name: str
                The name of this thread. If not set, a default
                name will be given to the associated thread.
        '''
        if name is None:
            name = f"cam-acq-thrd-{self._CAM_ACQ_THREADS_COUNT}"
            self._CAM_ACQ_THREADS_COUNT += 1
        
        super().__init__( name=name )
        self.camera = camera
        self.buffer = FramesAcquisitionBuffer()
        self.last_frame_index = -1
        
    #-------------------------------------------------------------------------
    def get(self) -> IndexedFrame:
        '''Returns a reference to the newly acquired frame.
        
        Cannot be called within the context of the  frames
        acquisition thread.  Must be called outside of it.
        Deadlocks will eventually happen otherwise.
        
        Returns:
            Either a reference to the buffer that contains 
            the  newly  acquired  frame or None if timeout
            happened.
        '''
        return self.buffer.get()
        
    #-------------------------------------------------------------------------
    def get_frame(self) -> Frame:
        '''Returns a reference to the currently accessible buffer.
        
        Cannot be called within the context of the  frames
        acquisition thread.  Must be called outside of it.
        Deadlocks will eventually happen otherwise.
        
        Returns:
            Either a reference to the newly acquired frame
            or None if timeout happened.
        '''
        try:
            return self.buffer.get().frame
        except:
            return None

    #-------------------------------------------------------------------------
    def is_ok(self) -> bool:
        '''Returns True when status of this camera acquisition thread is ok, or False otherwise.
        '''
        return self.camera is not None  and  self.camera.hw_default_width != 0

    #-------------------------------------------------------------------------
    def run(self) -> None:
        '''The acquisition method once this thread has been started.
        '''
        frame_index = 0
        self._keep_on = self.is_ok()
        while self._keep_on:
            frame = self.camera.read()
            if frame is None:
                time.sleep( 0.020 )
            else:
                self.buffer.set( IndexedFrame(frame_index, frame) )
            frame_index += 1
        self.camera.release()

    #-------------------------------------------------------------------------
    def stop(self) -> None:
        '''Definitively stops this acquisition thread.
        '''
        self._keep_on = False

    #-------------------------------------------------------------------------
    _CAM_ACQ_THREADS_COUNT = 0

#=====   end of   src.Cameras.camera_acquisition   =====#
