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
## This module defines:
#
#    class RGBColor
#    class GrayColor
#
# and colors
#    ANTHRACITE
#    BLACK
#    BLUE
#    BROWN
#    DARK_RED
#    DEEP_GRAY
#    DEEP_GREEN
#    GRAY
#    LIGHT_BLUE
#    LIGHT_GRAY
#    LIGHT_GREEN
#    NAVY_BLUE
#    ORANGE
#    RED
#    YELLOW 
#    WHITE
#
#    TARGET_WHITE
#    TARGET_BLACK
#    TARGET_BLUE
#    TARGET_RED
#    TARGET_GOLD
#    TARGET_BLUE_6
#    TARGET_BLUE_NFAA
#

#=============================================================================
from typing import ForwardRef, Optional, Union

from .types import Numeric, PixelColor


#=============================================================================
RGBColorRef = ForwardRef( "RGBColor" )
Color = Union[ Numeric, PixelColor, RGBColorRef ]


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
        return round( 0.299 * self.color[2] + 0.587 * self.color[1] + 0.114 * self.color[0] )
    @y.setter
    def y(self, value: int) -> None:
        comp = self._clipped( value )
        self.color = [ comp, comp, comp ]
        
    #-------------------------------------------------------------------------
    def copy(self, other: RGBColorRef = None) -> Optional[ RGBColorRef ]:
        '''Copies an RGB color.
        
        Args:
            other: RGBColor
                Either a reference to an RGB color to  be  copied 
                in  this  color,  or None in which case a copy of 
                this color is returned.
        
        Returns:
            Either nothing if 'other' is set, or a reference to a
            new  instance of RGBColor initialized with the values 
            of this color attributes.
        '''
        if other is None:
            return RGBColor( self.r, self.g, self.b )
        else:
            self.set( other.r, other.g, other.b )
        
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
        return 0 if value <= 0 else 255 if value >= 255 else value

    #-------------------------------------------------------------------------
    def __eq__(self, other: Color) -> bool:
        '''Returns True if both colors have same color components.
        '''
        try:
            return self.color == other.color
        except:
            try:
                return self.color == other
            except:
                return self.color == (other, other, other)

    #-------------------------------------------------------------------------
    def __ne__(self, other: Color) -> bool:
        '''Returns True if any same color component differs in this and in other.
        '''
        try:
            return self.color != other.color
        except:
            try:
                return self.color != other
            except:
                return self.color != (other, other, other)

    #-------------------------------------------------------------------------
    def __add__(self, other: Color) -> RGBColorRef:
        '''
        '''
        try:
            return RGBColor( self.r + other.r, self.g + other.g, self.r + other.b )
        except:
            try:
                return RGBColor( self.r + other[0], self.g + other[1], self.b + other[2] )
            except:
                return RGBColor( self.r + other, self.g + other, self.b + other )

    #-------------------------------------------------------------------------
    def __iadd__(self, other: Color) -> RGBColorRef:
        '''
        '''
        try:
            self.set( self.r + other.r, self.g + other.g, self.r + other.b )
        except:
            try:
                self.set( self.r + other[0], self.g + other[1], self.b + other[2] )
            except:
                self.set( self.r + other, self.g + other, self.b + other )
        return self

    #-------------------------------------------------------------------------
    def __radd__(self, other: Color) -> RGBColorRef:
        '''
        '''
        return self.add( other )

    #-------------------------------------------------------------------------
    def __floordiv__(self, den: Numeric) -> RGBColorRef:
        '''
        '''
        return RGBColor( int(self.r / den), int(self.g / den), int(self.b / den) )

    #-------------------------------------------------------------------------
    def __ifloordiv__(self, den: Numeric) -> RGBColorRef:
        '''
        '''
        self.set( int(self.r / den), int(self.g / den), int(self.b / den) )

    #-------------------------------------------------------------------------
    def __mul__(self, coeff: Numeric) -> RGBColorRef:
        '''
        '''
        return RGBColor( round(self.r * coeff), round(self.g * coeff), round(self.b * coeff) )

    #-------------------------------------------------------------------------
    def __imul__(self, coeff: Numeric) -> RGBColorRef:
        '''
        '''
        self.set( round(self.r * coeff), round(self.g * coeff), round(self.b * coeff) )
        return self

    #-------------------------------------------------------------------------
    def __rmul__(self, coeff: Numeric) -> RGBColorRef:
        '''
        '''
        return RGBColor( round(self.r * coeff), round(self.g * coeff), round(self.b * coeff) )

    #-------------------------------------------------------------------------
    def __sub__(self, other: Color) -> RGBColorRef:
        '''
        '''
        try:
            return RGBColor( self.r - other.r, self.g - other.g, self.r - other.b )
        except:
            try:
                return RGBColor( self.r - other[0], self.g - other[1], self.b - other[2] )
            except:
                return RGBColor( self.r - other, self.g - other, self.b - other )

    #-------------------------------------------------------------------------
    def __isub__(self, other: Color) -> RGBColorRef:
        '''
        '''
        try:
            self.set( self.r - other.r, self.g - other.g, self.r - other.b )
        except:
            try:
                self.set( self.r - other[0], self.g - other[1], self.b - other[2] )
            except:
                self.set( self.r - other, self.g - other, self.b - other )
        return self

    #-------------------------------------------------------------------------
    def __rsub__(self, other: Color) -> RGBColorRef:
        '''
        '''
        try:
            return RGBColor( other.r - self.r, other.g - self.g, other.b - self.b  )
        except:
            try:
                return RGBColor( other[0] - self.r, other[1] - self.g, other[2] - self.b )
            except:
                return RGBColor( other - self.r, other - self.g, other- self.b )

    #-------------------------------------------------------------------------
    def __truediv__(self, den: Numeric) -> RGBColorRef:
        '''
        '''
        return RGBColor( round(self.r / den), round(self.g / den), round(self.b / den) )

    #-------------------------------------------------------------------------
    def __itruediv__(self, den: Numeric) -> RGBColorRef:
        '''
        '''
        self.set( round(self.r / den), round(self.g / den), round(self.b / den) )
        return self


#=============================================================================
class GrayColor( RGBColor ):
    """The class of RGB gray colors.
    
    """
    #-------------------------------------------------------------------------
    def __init__(self, comp: int) -> None:
        '''Constructor.
        
        Args:
            comp: int
                The value of the gray level of this gray color,
                clipped within interval [0, 255].
                Notice:  value 0 stands for  black,  value  255
                stands for white.
        '''
        super().__init__( comp, comp, comp )


#=============================================================================
ANTHRACITE  = RGBColor( 32, 32, 32 )
BLACK       = RGBColor( 0, 0, 0 )
BLUE        = RGBColor( 0, 0, 255 )
BROWN       = RGBColor( 96, 48, 0 )
DARK_RED    = RGBColor( 128, 0, 0 )
DEEP_GRAY   = RGBColor( 64, 64, 64 )
DEEP_GREEN  = RGBColor( 0, 96, 0 )
GRAY        = RGBColor( 128, 128, 128 )
LIGHT_BLUE  = RGBColor( 0, 255, 255 )
LIGHT_GRAY  = RGBColor( 192, 192, 192 )
LIGHT_GREEN = RGBColor( 0, 255, 0 )
NAVY_BLUE   = RGBColor( 0, 0, 64 )
ORANGE      = RGBColor( 255, 128, 0 )
RED         = RGBColor( 255, 0, 0 )
YELLOW      = RGBColor( 255, 255, 0 )
WHITE       = RGBColor( 255, 255, 255 )

TARGET_WHITE = RGBColor( 255, 255, 255 )
TARGET_BLACK = RGBColor(   0,   0,   0 )
TARGET_BLUE  = RGBColor(  65, 181, 200 )
TARGET_RED   = RGBColor( 255,  37,  21 )
TARGET_GOLD  = RGBColor( 255, 245,  55 )

TARGET_BLUE_6 = RGBColor( 17, 165, 255 )
TARGET_BLUE_NFAA = RGBColor( 63, 63,  95 )

#=====   end of   src.Utils.rgb_color   =====#
