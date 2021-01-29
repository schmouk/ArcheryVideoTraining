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
from typing import ForwardRef, Union

from src.Utils.types import Numeric


#=============================================================================
Coord2DRef = ForwardRef( "Coord2D" )


#=============================================================================
class Coord2D:
    """The class of 2D integer coordinates.
    """
    #-------------------------------------------------------------------------
    def __init__(self, x: int = 0, y: int = 0, *, copy: Coord2DRef = None) -> None:
        '''Constructor.
        
        Args:
            x, y: int
                The values of the 2D coordinates. Defaults to 0.
            copy: Coord2D
                A reference to a 2D-coordinates to be copied. If
                set,  takes  precedence  over arguments x and y.
                Defaults to None.
        '''
        if copy is not None:
            self.x, self.y = copy.x, copy.y
        else:
            self.x, self.y = x, y

    #-------------------------------------------------------------------------
    def copy(self) -> Coord2DRef:
        '''Creates a copy of this 2D coordinates.
        '''
        return Coord2D( self.x, self.y )

    #-------------------------------------------------------------------------
    def _clip(self) -> None:
        '''Clips both coordinates.
        '''
        pass

    #-------------------------------------------------------------------------
    def _clipped(self, value) -> int:
        '''Returns a clipped value.
        '''
        return value

    #-------------------------------------------------------------------------
    def __add__(self, val: Union[Numeric, Coord2DRef]) -> Coord2DRef:
        '''
        '''
        if isinstance( val, Coord2DRef ):
            return Coord2D( self._clipped(self.x + val.x),
                            self._clipped(self.y + val.y) )
        else:
            val = round( val )
            return Coord2D( self._clipped(self.x + val),
                            self._clipped(self.y + val) )

    #-------------------------------------------------------------------------
    def __iadd__(self, val: Union[Numeric, Coord2DRef]) -> Coord2DRef:
        '''
        '''
        if isinstance( val, Coord2DRef ):
            self.x += round( val.x )
            self.y += round( val.y )
        else:
            val = round( val )
            self.x += val
            self.y += val
        self._clip()
        return self

    #-------------------------------------------------------------------------
    def __radd__(self, val: Numeric) -> Coord2DRef:
        '''
        '''
        return self.__add__( val )

    #-------------------------------------------------------------------------
    def __div__(self, val: Numeric) -> Coord2DRef:
        '''
        '''
        return Coord2D( self._clipped(round(self.x / val)),
                        self._clipped(round(self.y / val)) )
        
    #-------------------------------------------------------------------------
    def __idiv__(self, val: Numeric) -> Coord2DRef:
        '''
        '''
        self.x = self._clipped( round(self.x / val) )
        self.y = self._clipped( round(self.y / val) )
        return self

    #-------------------------------------------------------------------------
    def __rdiv__(self, val: Numeric) -> Coord2DRef:
        '''
        '''
        return Coord2D( self._clipped(round(val / self.x)),
                        self._clipped(round(val / self.y)) )

    #-------------------------------------------------------------------------
    def __mul__(self, val: Numeric) -> Coord2DRef:
        '''
        '''
        return Coord2D( self._clipped(round(self.x * val)),
                        self._clipped(round(self.y * val)) )
        
    #-------------------------------------------------------------------------
    def __imul__(self, val: Numeric) -> Coord2DRef:
        '''
        '''
        self.x = self._clipped( round(self.x * val) )
        self.y = self._clipped( round(self.y * val) )
        return self

    #-------------------------------------------------------------------------
    def __rmul__(self, val: Numeric) -> Coord2DRef:
        '''
        '''
        return self.__mul__( val )

    #-------------------------------------------------------------------------
    def __sub__(self, val: Union[Numeric, Coord2DRef]) -> Coord2DRef:
        '''
        '''
        if isinstance( val, Coord2DRef ):
            return Coord2D( self._clipped(self.x - val.x),
                            self._clipped(self.y - val.y) )
        else:
            val = round( val )
            return Coord2D( self._clipped(self.x - val),
                            self._clipped(self.y - val) )

    #-------------------------------------------------------------------------
    def __isub__(self, val: Union[Numeric, Coord2DRef]) -> Coord2DRef:
        '''
        '''
        if isinstance( val, Coord2DRef ):
            self.x -= round( val.x )
            self.y -= round( val.y )
        else:
            val = round( val )
            self.x -= val
            self.y -= val
        self.clip()
        return self

    #-------------------------------------------------------------------------
    def __rsub__(self, val: Numeric) -> Coord2DRef:
        '''
        '''
        if isinstance( val, Coord2DRef ):
            return Coord2D( self._clipped(val.x - self.x),
                            self._clipped(val.y - self.y) )
        else:
            val = round( val )
            return Coord2D( self._clipped(val - self.x),
                            self._clipped(val - self.y) )

#=====   end of   src.Shapes.coord_2d   =====#
