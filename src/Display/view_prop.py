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
from .view           import AVTWindowRef, View
from src.Utils.types import PixelColor


#=============================================================================
class ViewProp( View ):
    """The class of views defined by proportional values.
    
    These proportional values are set in interval [0.0, 1.0] 
    and  represent  percentages of the embedding window size 
    (i.e. width and height).
    """
    #-------------------------------------------------------------------------
    def __init__(self, parent  : AVTWindowRef,
                       x       : float,
                       y       : float,
                       width   : float,
                       height  : float,
                       bg_color: PixelColor = (32, 32, 32) ) -> None:
        '''Constructor.
        
        Args:
            parent: AVTWindow
                A reference to the AVT window that embeds this
                view.
            x, y: float
                The coordinates of the top-left corner of this
                view,  expressed as percentages of resp. width
                and height of  the  embedding  window.  Values
                must be included in [0.0, 1.0]
            width, height: float
                The size of this view,  expressed as  percent-
                ages of the embedding window sizes.
            bg_color: PixelColor
                The solid color of the background of this view
                expressed  as RGB values in interval [0, 255].
                Notice: values are clipped into this interval. 
                Defaults to (32, 32, 32).
        
        Raises:
            ValueError:  Some  of  the  coordinates  or  sizes 
                values are outside interval [0.0, 1.0].
        '''
        if not 0.0 <= x <= 1.0  or  not 0.0 <= y <= 1.0:
            raise ValueError( f"position coordinates ({x}, {y}) must be in [0.0, 1.0].")
        if not 0.0 <= width <= 1.0  or  not 0.0 <= height <= 1.0:
            raise ValueError( f"sizes ({width}, {height}) must be in [0.0, 1.0].")
        
        self.prop_x = x
        self.prop_y = y
        self.prop_width = width
        self.prop_height = height
        self.parent = parent

        super().__init__( parent,
                          round( x * parent.width ), 
                          round( y * parent.height ),
                          round( width * parent.width),
                          round( height * parent.height),
                          bg_color )

#===============================================================================
#     #-------------------------------------------------------------------------
#     @property
#     def height(self) -> int:
#         return round( self.parent.height * self.prop_height )
# 
#     @property
#     def width(self) -> int:
#         return round( self.parent.width * self.prop_width )
# 
#     @property
#     def x(self) -> int:
#         return round( self.parent.width * self.prop_x )
# 
#     @property
#     def y(self) -> int:
#         return round( self.parent.height * self.prop_y )
#===============================================================================

#=====   end of   src.Display.view_prop   =====#
