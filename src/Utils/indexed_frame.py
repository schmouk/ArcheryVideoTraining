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
from typing import ForwardRef

from src.Utils.types import Frame


#=============================================================================
IndexedFrameRef = ForwardRef( "IndexedFrame" )


#=============================================================================
class IndexedFrame:
    """The class of frames associated with an index.
    """
    #-------------------------------------------------------------------------
    def __init__(self, index: int   = None,
                       frame: Frame = None,
                       *, 
                       copy: IndexedFrameRef = None) -> None:
        '''Constructor.
        
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
            copy: IndexFrame
                Named argument.  This is a reference  to  an 
                indexed  frame instance that is to be copied 
                into this newly created one. Must be None if 
                'index'  and  'frame'  are set.  Defaults to 
                None.
        Raises:
            AssertionError:  some assertion on the arguments
                values has failed.
        '''
        if copy is None:
            assert (index is None and frame is None) or (index is not None and frame is not None)
            self.index = index
            self.frame = frame
        else:
            assert index is None and frame is None
            self = copy.copy()

    #-------------------------------------------------------------------------
    def copy(self) -> IndexedFrameRef:
        '''Returns a copy of this indexed frame as a new instance.
        
        Notice: the content of the frame is truly copied, unless
        self.frame is None.
        '''
        try:
            return IndexedFrame( self.index, self.frame.copy() )
        except:
            return IndexedFrame( self.index, self.frame )

#=====   end of   src.Utils.indexed_frame   =====#
