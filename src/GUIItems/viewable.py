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

from src.Utils.rgb_color import RGBColor


#=============================================================================
class Viewable:
    """The base class for all entities which get attribute '.content'.
    """
    #-------------------------------------------------------------------------
    def __init__(self, x       : int,
                       y       : int,
                       width   : int,
                       height  : int,
                       bg_color: RGBColor = None) -> None:
        '''Constructor.
        
        Args:
            x: int
                The X-coordinate of the top-left corner of 
                this viewable item,  expressed  as  pixels 
                counts.
            y: int
                The Y-coordinate of the top-left corner of 
                this viewable item,  expressed  as  pixels 
                counts.
            width: int
                The width (pixels count) of the  associated 
                content.
            height: int
                The height (pixels count) of the associated
                content.
            bg_color: RGBColor
                A reference to  a  solid  background  color  
                which will be used to fill the content.  If 
                None, content is created with black content.
        '''
        self.x        = x
        self.y        = y
        self.width    = width
        self.height   = height
        self.bg_color = bg_color

        try:
            if bg_color is None:
                self.content = np.zeros( (height,width,3), np.uint8 )
            else:
                self.content = (np.zeros((height,width,3), np.uint8) + bg_color.color).astype( np.uint8 )
        
        except:
            self.content = None

#=====   end of   src.GUIItems.viewable   =====#
