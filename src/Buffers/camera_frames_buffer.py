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
from typing      import ForwardRef
from threading   import Event, Lock
import time

from src.Utils.indexed_frame import IndexedFrame


#-------------------------------------------------------------------------
IndexRef = ForwardRef( "Index" )


#=============================================================================
class CameraFramesBuffer:
    """The class of camera captured frames buffers.
    
    Buffers containing frames that are  captured  by  cameras
    get two independent indexes:  a write one and a read one.
    The write one is moved forward each time a frame has been
    grabbed by the camera. The read index is moved forward at
    a periodical pace.  The reads from this  type  of  buffer
    should  not be operate before some full criteria has been
    validated (for instance, not before all but one entry  in
    the buffer have been filled).
    
    It is the responsibility of the  caller  to  ensure  that
    grabbed  images  are  read at a proper pace.  Should this
    not be ensured, frames could be read in a somewhat random
    manner. As a generic implementation, the grabbing and the 
    reading of frames should each be processed in two differ-
    ent threads.
    """
    #-------------------------------------------------------------------------
    def __init__(self, max_size: int) -> None:
        '''Constructor.
        
        Args:
            max_size: int
                The max number of frames that can be stored
                simultaneously  in  this  buffer.  Must  be
                greater than 1.
        '''
        assert max_size > 1
        
        self.buffer = [ None ] * max_size
        self.max_size     = max_size
        self.current_size = 0
        self.write_index  = self.Index( max_size )
        self.read_index   = self.Index( max_size )
        self.free_slots   = 1 if max_size <= 3 else 2
        self.start_event  = Event()
        self.start_event.clear()  # just to be sure that this event is not yet flagged.

    #-------------------------------------------------------------------------
    def append(self, indexed_frame: IndexedFrame) -> None:
        '''Writes a newly grabbed frame into this buffer.
        
        Args:
            indexed_frame: IndexedFrame
                A reference to the indexed frame that is 
                to be stored in this buffer.
        '''
        self.buffer[ self.write_index.val() ] = indexed_frame
        self.write_index += 1
        
        if self.current_size < self.max_size:
            if self.is_nearly_full():
                self.start_event.set()
                
            self.current_size += 1

    #-------------------------------------------------------------------------
    def get_oldest(self) -> IndexedFrame:
        '''Returns a copy of the oldest grabbed frame in buffer.
        
        Returns None if no grabbed frame is yet  available.
        This will be the case until "enough" grabbed frames
        will have been stored in buffer.
        '''
        indexed_frame = self[0]  ##.copy()
        self.read_index += 1
        return indexed_frame

    #-------------------------------------------------------------------------
    def is_full(self) -> bool:
        '''Returns True if this buffer is full.
        '''
        return self.current_size == self.max_size

    #-------------------------------------------------------------------------
    def is_nearly_full(self, free_slots: int = 1) -> bool:
        '''Returns True if this buffer is "nearly" full.
        
        Args:
            free_slots: int
                The max number of free slots for this buffer  to
                be evaluated as nearly full. Defaults to 1.
        '''
        return self.current_size >= self.max_size - free_slots

    #-------------------------------------------------------------------------
    def __getitem__(self, index: int) -> IndexedFrame:
        '''Operator [].
        
        Args:
            index: int
                The index of the item to be returned:
                 0 means: the oldest grabbed frame in buffer.
                 1 means: the older frame but the oldest one.
                -1 means: the newest grabbed frame in buffer.
                -2 means: the  2nd  newer  grabbed  frame  in 
                          buffer.
                Notice: indexes must be within interval:
                    -max_size <= index < max_size.
        
        Returns:
            A reference to the indexed frame that relates  to
            the passed index.  May return None if no frame is
            yet available in buffer for the specified  index.
            This will be the case when not enough frames have
            been grabbed and stored. It is the responsibility
            of the caller to test for this and to fairly wait
            or act when None is returned.
        
        Raises:
            KeyError: the index is out of bounds.
        '''
        self.start_event.wait()
        
        if -self.max_size <= index < self.max_size:
            return self.buffer[ (self.read_index.val() + index) % self.max_size ]
        else:
            raise KeyError( f"index value {index} is out of bounds [{-self.max_size}:{self.maxsize}]" )


    #-------------------------------------------------------------------------
    class Index:
        '''The internal class of guarded indexes.
        '''
        #---------------------------------------------------------------------
        def __init__(self, max_size: int) -> None:
            '''Constructor.
            '''
            self.value = 0
            self.max_size = max_size
            self.lock = Lock()

        #---------------------------------------------------------------------
        def val(self) -> int:
            '''Returns the current value of this guarded index.
            '''
            with self.lock:
                return self.value

        #---------------------------------------------------------------------
        def __iadd__(self, incr: int) -> IndexRef:
            '''Operator '+='.
            
            Args:
                incr: int
                    The integer value to be added to this index.
            
            Returns:
                A reference to this guarded index.
            '''
            with self.lock:
                self.value = (self.value + incr) % self.max_size
            return self

#=====   end of   src.Buffers.camera_frames_buffer   =====#
