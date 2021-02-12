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
from .avt_fonts          import AVTDefaultFont
from src.GUIItems.Controls.gui_control_base   import GUIControlBase
from .font               import Font
from src.Shapes.point    import Point
from src.Utils.rgb_color import RGBColor
from src.Display.view    import View


#=============================================================================
class Label( GUIControlBase ):
    """The class of labels items used with the GUI.
    
    Notice: current version is a first draft one.
    """
    #-------------------------------------------------------------------------
    def __init__(self, parent_view: View,
                       text       : str,
                       x          : int  = None,
                       y          : int  = None,
                       size       : int  = None,
                       text_font  : Font = None,
                       *,
                       pos        : Point = None) -> None:
        '''Constructor.
        
        Args:
            parent_view: View 
                A reference to the containing view.
            text: str
                The text of this label.
            x, y : int
                The position of this label in the  view.  This
                is  the  position of the baseline of the text.
                If argument 'pos' is set, 'x' and 'y' are both
                ignored.  If  argument 'pos' is None,  'x' and 
                'y' must be both set.
            size: int
                The size of the text to display.  If None, the
                default size of the specified font is used.
            text_font: Font
                A reference to the font to  be  used  for  the
                text associated with this label.
            pos: Point
                If set, takes precedence over x and y. This is
                the position of the bottom-left corner of this
                label.
        
        Raises:
            AssertionError: if 'pos' is not set,  both 'x' and
                'y' must be and are not both set.
            ValueError: text must be set.
        '''
        if text is None:
            raise ValueError( "argument 'text' must be set and cannot be None" )
        
        self.parent_view = parent_view
        self.text = str( text )
        self.set_font( text_font, size )
        if pos is None:
            assert x is not None and y is not None
            pos = Point( x, y )
        self.pos = pos

    #-------------------------------------------------------------------------
    @property
    def size(self) -> int:
        return self.font.size
    @size.setter
    def size(self, value: int) -> None:
        self.font.set_size( value )

    #-------------------------------------------------------------------------
    def draw(self) -> None:
        '''Draws this label into its parent view.
        '''
        self.font.draw_text( self.parent_view, self.pos, self.text )

    #-------------------------------------------------------------------------
    def move(self, dx: int, dy: int) -> None:
        '''Relative move for this label in its parent view.
        
        Args:
            dx, dy: int
                The offsets related to X- and Y- axis.
        '''
        self.pos.x += dx
        self.pos.y += dy

    #-------------------------------------------------------------------------
    def move_at(self, x: int, y: int) -> None:
        '''Absolute move for this label in its parent view.
        
        Args:
            x, y: int
                The absolute corrdinates related to X- and Y- axis.
        '''
        self.pos.x = x
        self.pos.y = y

    #-------------------------------------------------------------------------
    def set_bg_color(self, color: RGBColor) -> None:
        '''Modifies the background color of this label.
        
        Args:
            color: RGBColor
                A reference to the RGB color to use for the
                display of this label.
        '''
        if color != self.font.bg_color:
            if self.font is AVTDefaultFont:
                self.font = AVTDefaultFont.copy()
            self.font.bg_color = color.copy()

    #-------------------------------------------------------------------------
    def set_color(self, color: RGBColor) -> None:
        '''Modifies the color of this label.
        
        Args:
            color: RGBColor
                A reference to the RGB color to use for the
                display of this label.
        '''
        if color != self.font.color:
            if self.font is AVTDefaultFont:
                self.font = AVTDefaultFont.copy()
            self.font.color = color.copy()

    #-------------------------------------------------------------------------
    def set_font(self, font: Font = None, size: int = None) -> None:
        '''Modifies the font and/or the size of the font.
        
        Args:
            font: Font
                A reference to the font to be  used  for  the
                text associated with this label. If None, the
                AVT default font is used instead. Defaults to
                None.
            size: int
                The size of the text to display. If None, the
                default  size  of the specified font is used.
                Defaults to None.
        '''
        if font is None:
            if size is None or size == AVTDefaultFont.size:
                self.font = AVTDefaultFont.copy()
            else:
                self.font = AVTDefaultFont.copy().set_size( size )
        else:
            if size is None or size == font.size:
                self.font = font.copy()
            else:
                self.font = font.copy().set_size( size )

#=====   end of   src.GUIItems.label   =====#
