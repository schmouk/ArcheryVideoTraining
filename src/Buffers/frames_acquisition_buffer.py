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
from threading import Lock

from src.Utils.types         import Frame
from src.Utils.indexed_frame import IndexedFrame


#=============================================================================
class FramesAcquisitionBuffer:
    """The class of the cameras buffers for frames acquisition.
    
    These buffers are flip-flop buffers.
    """

    #-------------------------------------------------------------------------
    def __init__(self, acquisition_timeout: float = 0.100) -> None:
        '''Constructor.
        
        Args:
            acquisition_timeout: float
                The value, expressed in fractional seconds,
                of the timeout to be applied on acquisition
                of frames. Defaults to 100 ms.
        '''
        self.acq_timeout = acquisition_timeout

        self._buf = [ None, None ]
        self._ndx = 0
        self._lock = Lock()
        
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
        return self._buf[ self._flipflop_index() ]
        
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
        return self.get().frame

    #-------------------------------------------------------------------------
    def set(self, indexed_frame: IndexedFrame) -> None:
        '''Sets new data into the not yet accessible buffer.
        
        Internally calls protected method '_set()' which may
        be overwritten in inheriting classes.
        
        Args:
            indexed_frame: IndexedFrame
                A reference to an indexed frame.
        '''
        with self._lock:
            self._buf[ self._ndx ] = indexed_frame
            self._ndx = self._flipflop_index()

    #-------------------------------------------------------------------------
    def _flipflop_index(self) -> int:
        '''Returns the index of the currently accessible buffer.
        '''
        return 0 if self._ndx == 1 else 1

#=====   end of   src.Buffers.frames_acquisition_buffer   =====#
