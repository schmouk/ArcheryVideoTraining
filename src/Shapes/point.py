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
PointRef = ForwardRef( "Point" )


#=============================================================================
class Point( Coord2D ):
    """The class of 2D points.
    
    Notice: 2D points are just 2D coordinates.
    """
    #-------------------------------------------------------------------------
    def __init__(self, x: int = 0, y: int = 0, *, copy: PointRef) -> None:
        '''Constructor.
        
        Args:
            x, y: int
                The values of the 2D point. Defaults to 0.
            copy: Offset
                A reference to a 2D-point to be copied.  If  set, 
                takes precedence over arguments x and y. Defaults 
                to None.
        '''
        super().__init__( x, y, copy=copy )

    #-------------------------------------------------------------------------
    def copy(self) -> PointRef:
        '''Creates a copy of this 2D point.
        '''
        return Point( self.x, self.y )

#=====   end of   src.Shapes.point   =====#
