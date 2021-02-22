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
from threading import Semaphore

from src.Utils.circular_buffer   import CircularBuffer
from src.Utils.types             import Frame
from src.Utils.indexed_frame     import IndexedFrame


#=============================================================================
class FramesAcquisitionBuffer( CircularBuffer ):
    """The class of the cameras buffers for frames acquisition.
    """

    #-------------------------------------------------------------------------
    def __init__(self) -> None:
        '''Constructor.
        
        Args:
            sync_event: Event
                A reference to a synchronizing event between
                acquisition and display of captured frames.
        '''
        entries_count = 2
        super().__init__( entries_count )
        self.sems = [ Semaphore() for _ in range(entries_count) ]
        
    #-------------------------------------------------------------------------
    def append(self, index: int, frame: Frame) -> None:
        '''Appends a new indexed frame to this buffer.
        
        This is a convenient wrapper to the inherited  over-
        written method.
        
        Notice:  returns nothing while the inherited  called
            method returns a reference to 'self'.
        Args:
            index: int
                The index of the associated frame within the 
                video stream. Must be set if 'frame' is set. 
                Must be None if 'copy' is set.  Defaults to 
                None.
            frame: Frame
                A reference to a frame associated  with  the 
                index.  Must  be set if 'index' is set. Must 
                be None if 'copy' is set. Defaults to None.
        '''
        super().append( IndexedFrame(index, frame) )
        
    #-------------------------------------------------------------------------
    def get(self) -> IndexedFrame:
        '''Returns a reference to the oldest acquired indexed frame.
        
        Cannot be called within the context of the  frames
        acquisition thread.  Must be called outside of it.
        Deadlocks will eventually happen otherwise.
        
        Returns:
            Either a reference to the buffer that contains 
            the oldest acquired frame or None if buffer is
            not yet full.
        '''
        return self.get_oldest() if self.is_nearly_full(1) else None

#=====   end of   src.Buffers.frames_acquisition_buffer   =====#
