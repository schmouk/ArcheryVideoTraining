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
from typing import Any, ForwardRef
from threading import Lock


#=============================================================================
CircularBufferRef = ForwardRef( "CircularBuffer" )


#=============================================================================
class CircularBuffer:
    """The class of circular buffers.
    
    Circular buffers are buffers which contain at most 'n' items 
    and  which  remember the 'n' last ones,  removing the oldest 
    one when appending a new one as soon as the buffer is full.
    
    New version of this class is thread-safe and  implementation 
    of '__get_item__()' has been optimized (31% faster).
    """
    #-------------------------------------------------------------------------
    def __init__(self, size: int) -> None:
        '''Constructor.
        
        Args:
            size: int
                The size of the buffer. Must be greater than 0.
                Notice: size == 1 gets no meaning for a buffer.
            min_delay: int
                The minimum delay,  expressed in  milliseconds,
                to  wait  before two successive appends in this
                buffer.
        
        Raises:
            ValueError:  size is less than 1 or min_delay_ms  is
                zero or less.
        '''
        if size < 1:
            raise ValueError( f'Summed Buffers must contain at least 1 value, not {size}.')
        self.buf       = [ None ]*size
        self.ndx       = 0
        self.count     = 0
        self.max_count = size
        
        self._lock = Lock()

    #-------------------------------------------------------------------------
    def append(self, item: Any) -> CircularBufferRef:
        '''Appends a new item to this circular buffer.
        
        If this buffer is full, the oldest item is removed 
        from it.
        
        Args:
            item: Any
                The new item to be stored in  this  buffer. 
                This  may be a reference to an instance of
                any class.
        
        Returns:
            a reference to this circular buffer.
        '''
        with self._lock:
            self.buf[ self.ndx ] = item
            self.ndx = (self.ndx + 1) % self.max_count
            
            if self.count < self.max_count:
                self.count += 1
            
        return self

    #-------------------------------------------------------------------------
    def get_latest(self) -> Any:
        '''Returns the latest item stored in this buffer.
        '''
        return self[ -1 ]

    #-------------------------------------------------------------------------
    def get_oldest(self) -> Any:
        '''Returns the oldest item stored in this buffer.
        '''
        return self[ 0 ]

    #-------------------------------------------------------------------------
    def is_empty(self) -> bool:
        '''Returns True if this buffer contains no item.
        '''
        return self.count == 0

    #-------------------------------------------------------------------------
    def is_full(self) -> bool:
        '''Returns True if this buffer is full.
        '''
        return self.count == self.max_count

    #-------------------------------------------------------------------------
    def is_nearly_full(self, free_slots: int = 1) -> bool:
        '''Returns True if this buffer is full or nearly full.
        
        Args:
            free_slots: int
                The number of slots that have not yet been set
                before this method returns False. Negative
                values and 0 make 'is_nearly_full()' acting as
                'is_full()'.
        '''
        return self.max_count - self.count <= free_slots if free_slots >= 1 else self.is_full()

    #-------------------------------------------------------------------------
    def store(self, item: Any) -> CircularBufferRef:
        '''Stores a new item into this circular buffer.
        
        Stores a new item into the current index position.
        
        Args:
            item: Any
                The new item to be stored in this  buffer.
                This  may be a reference to an instance of 
                any class.
        
        Returns:
            a reference to this circular buffer.
        '''
        with self._lock:
            self.buf[ self.ndx ] = item
            
            if self.count < self.max_count:
                self.count += 1
            
        return self

    #-------------------------------------------------------------------------
    def __getitem__(self, index: int) -> Any:
        '''Returns an indexed item from this circular buffer.
        
        Notice: index 0 references the oldest item that has
            been stored in this buffer. Index -1 references
            the latest one,  i.e. the most recently  stored
            in this buffer.  Indexes 1 up to 'self.count-1'
            reference the item that are contained  in  this
            buffer in their chronological order of storing.
        
        Args:
            index: int
                The index of the stored item to give back.
        
        Returns:
            The  item stored in this buffer that is refer-
            enced by the specified index,  or None if  the
            index is out of bounds.  Notice:  never raises
            any exception, such as KeyError for instance.
        '''
        if -self.max_count <= index < self.max_count:
            return self.buf[ (index + self.ndx) % self.count ]
        else:
            return None

    #-------------------------------------------------------------------------
    def __iadd__(self, item: Any) -> CircularBufferRef:
        '''Appends a new item to this circular buffer.
        
        If this buffer is full, the oldest item is removed 
        from it.
        
        Args:
            item: Any
                The new item to be stored in this  buffer. 
                This  may be a reference to an instance of
                any class.
        
        Returns:
            a reference to this circular buffer.
        '''
        return self.append( item )

    #-------------------------------------------------------------------------
    def __len__(self) -> int:
        '''Returns the current count of items that are contained in this buffer.
        '''
        return self.count

#=====   end of   src.Buffers.circular_buffer   =====#
