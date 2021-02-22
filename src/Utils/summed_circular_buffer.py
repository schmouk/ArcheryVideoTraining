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
from typing import Union

from .circular_buffer import CircularBuffer


#=============================================================================
Numeric = Union[ int, float ]


#=============================================================================
class SummedCircularBuffer( CircularBuffer ):
    """The class of summed circular buffers.
    
    Summed buffers are circular buffers Which contain scalar 
    values or objects for which scalar casting operators are 
    defined.
    
    The 'sum 'of these values is maintained at any time. The 
    'mean' of this sum is also available at any time.  These
    are an attribute for one of them and a property of  this 
    class  for  the  other,  so  they  both  can be directly 
    accessed as attributes. 
    """
    #-------------------------------------------------------------------------
    def __init__(self, size: int) -> None:
        '''Constructor.
        
        Args:
            size: int
                The size of the buffer. Must be greater than 0.
                Notice: size == 1 gets no meaning for a buffer.
        '''
        super().__init__( size )
        self.sum = 0

    #-------------------------------------------------------------------------
    @property
    def mean(self) -> float:
        if self.count != 0:
            return self.sum / self.count
        else:
            return 0.0

    #-------------------------------------------------------------------------
    def __iadd__(self, value: Numeric) -> None:
        '''Appends a new value into this summed buffer.
        
        If this buffer is full,  the oldest item is removed 
        from it.
        
        Args:
            item: Any
                The new item to be stored in  this  buffer. 
                This  may  be a reference to an instance of
                any class.
        
        Returns:
            a reference to this circular buffer.
        
        Raises:
            AssertionError: value is neither an integer nor 
                a float.
        '''
        assert isinstance( value, int ) or isinstance( value, float ), \
               f"passed argument must be either an integer or a float, and not of type {type(value)}"
        
        if self.is_full():
            self.sum -= self.get_oldest()
        self.sum += value
            
        return super().__iadd__( value )

#=====   end of   src.Utils.summed_circular_buffer   =====#
