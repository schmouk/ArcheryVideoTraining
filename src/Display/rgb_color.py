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


#=============================================================================
RGBColorRef = ForwardRef( "RGBColor" )


#=============================================================================
class RGBColor:
    """The class of RGB components colors.
    
    Library OpenCV uses a default BGR coding for  colors 
    which is not the usual RGB one.
    To help developments, class RGBColor codes colors in 
    a  usual  way while the BGR conversion is internally 
    automated for its use with library OpenCV.
    """
    #-------------------------------------------------------------------------
    def __init__(self, r: int, g: int, b: int) -> None:
        '''Constructor.
        
        Args:
            r, g, b: int
                The resp. red, green and blue components values 
                of  the  color to be encoded.  These values are
                clipped within interval [0, 255].
        '''
        self.set( r, g, b )

    #-------------------------------------------------------------------------
    @property
    def r(self) -> int:
        return self.color[2]
    @r.setter
    def r(self, value: int) -> None:
        self.color[2] = self._clipped( value )

    @property
    def g(self) -> int:
        return self.color[1]
    @g.setter
    def g(self, value: int) -> None:
        self.color[1] = self._clipped( value )

    @property
    def b(self) -> int:
        return self.color[0]
    @b.setter
    def b(self, value: int) -> None:
        self.color[0] = self._clipped( value )

    @property
    def y(self) -> int:
        return 0.299 * self.color[2] + 0.587 * self.color[1] + 0.114 * self.color[0]
    @y.setter
    def y(self, value: int) -> None:
        comp = self._clipped( value )
        self.color = [ comp, comp, comp ]
        
    #-------------------------------------------------------------------------
    def set(self, r: int, g: int, b: int) -> None:
        '''Sets the color components values of this RGB color.        

        Args:
            r, g, b: int
                The resp. red, green and blue components values 
                of  the  color to be encoded.  These values are
                clipped within interval [0, 255].
        '''
        self.color = [ self._clipped(b), self._clipped(g), self._clipped(r) ]

    #-------------------------------------------------------------------------
    def to_gray_color(self) -> RGBColorRef:
        '''Returns a reference to the gray color equivalent of this RGB color.
        '''
        comp = self.y
        return RGBColor( comp, comp, comp ) 

    #-------------------------------------------------------------------------
    @classmethod
    def _clipped(cls, value: int) -> int:
        '''Returns the clipped value of a specified value.
        '''
        return 0 if value <= 0 else 255 if value >= 255 else value  ## i.e. google
    
#=====   end of   src.Display.rgb_color   =====#
