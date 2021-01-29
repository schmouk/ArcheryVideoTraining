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
from typing import ForwardRef

from .coord_2d import Coord2D


#=============================================================================
SizeRef = ForwardRef( "Size" )


#=============================================================================
class Size( Coord2D ):
    """The class of sizes.
    
    Notice: sizes cannot be negative.
    """
    #-------------------------------------------------------------------------
    def __init__(self, width: int = 0, height: int = 0, *, copy: SizeRef = None) -> None:
        '''Constructor.
        
        Args:
            width: int
                The width of this size.
            height: int
                The height of this size.
            copy: Size
                A reference to a 2D-size to be  copied.  If  set, 
                takes precedence over arguments x and y. Defaults 
                to None.
        
        Raises:
            AssertionError: width, height or both are negative.
        '''
        assert width >= 0 and height >= 0
        super().__init__( width, height, copy=copy )

    #-------------------------------------------------------------------------
    def copy(self) -> SizeRef:
        '''Creates a copy of this 2D size.
        '''
        return Size( self.x, self.y )

    #-------------------------------------------------------------------------
    @property
    def width(self) -> int:
        return self.x
    @width.setter
    def width(self, val: int) -> None:
        self.x = val if val >= 0 else 0

    @property
    def height(self) -> int:
        return self.y
    @height.setter
    def height(self, val: int) -> None:
        self.y = val if val >= 0 else 0

    #-------------------------------------------------------------------------
    def _clip(self) -> None:
        '''Clips both coordinates.
        '''
        if self.x < 0: self.x = 0
        if self.y < 0: self.y = 0

    #-------------------------------------------------------------------------
    def _clipped(self, value) -> int:
        '''Returns a clipped value.
        '''
        return value if value >= 0 else 0

#=====   end of   src.Shapes.size   =====#
