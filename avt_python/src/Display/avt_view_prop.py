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
from .view_prop          import AVTWindowRef, ViewProp
from src.Shapes.rect     import Rect


#=============================================================================
class AVTViewProp( ViewProp ):
    """The class of views defined by proportional values and which specific to application AVT.
    
    These proportional values are set in interval [0.0, 1.0] 
    and  represent  percentages of the embedding window size 
    (i.e. width and height).
    """
    #-------------------------------------------------------------------------
    def __init__(self, parent     : AVTWindowRef,
                       x          : float       ,
                       y          : float       ,
                       width      : float       ,
                       height     : float       ,
                       parent_rect: Rect = None  ) -> None:
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
                ages of the embedding window size.
            parent_rect: Rect
                A reference to the  rectangle  in  which  this
                proportional  view takes place into the parent
                window.  If None,  the whole view size is used
                instead. Defaults to None.
        
        Raises:
            ValueError:  Some  of  the  coordinates  or  sizes 
                values are outside interval [0.0, 1.0].
        '''
        super().__init__( parent,
                          x, y,
                          width, height,
                          AVTConfig.DEFAULT_BACKGROUND / 2,
                          parent_rect )

#=====   end of   src.Display.avt_view_prop   =====#
