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
from src.App.avt_config  import AVTConfig
from .view               import AVTWindowRef, View


#=============================================================================
class AVTView( View ):
    """The class of Views that are specific to application AVT.
    """
    #-------------------------------------------------------------------------
    def __init__(self, parent: AVTWindowRef,
                       x     : int,
                       y     : int,
                       width : int,
                       height: int ) -> None:
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
        
        Raises:
            ValueError: coordinates get  negative  values,  or
                width or height get negative or null values.
        '''
        super().__init__( parent, x, y, width, height, AVTConfig.DEFAULT_BACKGROUND )

#=====   end of   src.Display.avt_view   =====#
