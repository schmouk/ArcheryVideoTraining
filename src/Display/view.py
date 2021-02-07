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
import numpy as np
from typing import ForwardRef
from threading import Lock

from .rgb_color import RGBColor


#=============================================================================
AVTWindowRef = ForwardRef( "AVTWindow" )


#=============================================================================
class View:
    """The base class for views that are embedded in the main window.
    
    Notice: for simplification purposes, views are rectangular.
    """
    #-------------------------------------------------------------------------
    def __init__(self, parent  : AVTWindowRef,
                       x       : int,
                       y       : int,
                       width   : int,
                       height  : int,
                       bg_color: RGBColor = RGBColor(32, 32, 32) ) -> None:
        '''Constructor.
        
        Args:
            parent: AVTWindow
                A reference to the AVT window that embeds this
                view.
            x, y: int
                The coordinates of the top-left corner of this
                view,  expressed  as pixels in the main window
                coordinates.
            width, height: int
                The size of this view,  expressed as pixels in
                the main window coordinates.
            bg_color: RGBColor
                The solid color of the background of this view
                expressed  as RGB values in interval [0, 255].
                Notice: values are clipped into this interval. 
                Defaults to dark gray (32, 32, 32).
        
        Raises:
            ValueError: coordinates get  negative  values,  or
                width or height get negative or null values.
        '''
        if x < 0 or y < 0:
            raise ValueError( f"position coordinates ({x}, {y}) cannot be negative.")
        if width <= 0 or height <= 0:
            raise ValueError( f"sizes ({width}, {height}) must be greater than 0.")
        
        self.lock = Lock()
        
        self.parent_window = parent
        self.x, self.y = x, y
        self.width, self.height = width, height
        self.bg_color = bg_color
        
        self.content = None
        self._fill_background()

        self.draw()

    #-------------------------------------------------------------------------
    def draw(self) -> None:
        '''Draws this view content within the parent window.
        
        May be overwritten in inheriting classes. See class
        'CameraView' or class 'ControlView' for examples of 
        code.
        '''
        with self.lock:
            self.parent_window.insert_view_content( self )
        self.parent_window.draw()

    #-------------------------------------------------------------------------
    def get_view_content(self) -> np.ndarray:
        '''Returns a reference to this view content, or None if not yet created.
        '''
        try:
            return self.content
        except:
            return None

    #-------------------------------------------------------------------------
    def join(self) -> None:
        '''Joins this view, even if it does not inherits from Thread.
        '''
        try:
            super().join()
        except:
            pass

    #-------------------------------------------------------------------------
    def start(self) -> None:
        '''Starts the thread that may be associated with this view.
        
        Should be overwritten in inheriting classes that 
        inherits  also  from  class  Thread.  See  class 
        'CameraView' for an example of code.
        '''
        pass

    #-------------------------------------------------------------------------
    def stop(self) -> None:
        '''Definitively stops the thread that may be associated with this view.
        
        Should be overwritten in inheriting classes that 
        inherits  also  from  class  Thread.  See  class 
        'CameraView' for an example of code.
        '''
        pass

    #-------------------------------------------------------------------------
    def _fill_background(self) -> None:
        '''Fills the parent window content with the background solid color of this view.
        '''
        self.content = np.zeros( (self.height, self.width, 3), np.uint8 ) + self.bg_color.color

#=====   end of   src.Display.view   =====#
