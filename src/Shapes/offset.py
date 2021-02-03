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
OffsetRef = ForwardRef( "Offset" )


#=============================================================================
class Offset( Coord2D ):
    """The class of 2D offsets.
    
    Notice: offset coordinates are '.dx' and '.dy'.
    """
    #-------------------------------------------------------------------------
    def __init__(self, dx: int = 0, dy: int = 0, *, copy: OffsetRef) -> None:
        '''Constructor.
        
        Args:
            dx, dy: int
                The values of this offset.
            copy: Offset
                A reference to a 2D-offset to be copied.  If set, 
                takes precedence over arguments x and y. Defaults 
                to None.
        '''
        super().__init__( dx, dy, copy=copy )

    #-------------------------------------------------------------------------
    def copy(self) -> OffsetRef:
        '''Creates a copy of this 2D offset.
        '''
        return Offset( self.x, self.y )

    #-------------------------------------------------------------------------
    @property
    def dx(self) -> int:
        return self.x
    @dx.setter
    def dx(self, val: int) -> None:
        self.x = val

    @property
    def dy(self) -> int:
        return self.y
    @dy.setter
    def dy(self, val: int) -> None:
        self.y = val

#=====   end of   src.Shapes.offset   =====#
