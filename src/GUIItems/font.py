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
## This module defines
#
#    class Font
#    class BoldFont
#    class ItalicFont
#    class BoldItalicFont
#

#=============================================================================
import cv2
from typing import ForwardRef, Optional

from src.Shapes.offset       import Offset
from src.Shapes.point        import Point
from src.Utils.rgb_color     import RGBColor, WHITE
from src.Display.view        import View
from src.GUIItems.viewable   import Viewable


#=============================================================================
FontRef = ForwardRef( "Font" )


#=============================================================================
class Font:
    """The class of text fonts.
    
    Notice: This is a minimal implementation that should help 
            using OpenCV when putting text in video frames.
    """
    #-------------------------------------------------------------------------
    def __init__(self, size      : int,
                       color     : RGBColor = WHITE,
                       bg_color  : RGBColor = None ,
                       bold      : bool     = False,
                       italic    : bool     = False,
                       sans_serif: bool     = True  ) -> None:
        '''Constructor.
        
        Notice: mode 'italic' is not available with 'sans-serif'
                fonts. This is an Open_CV restriction.
        
        Args:
            size: int
                The size of the police.  Should be greater  than
                five. Must be set.
            color: RGBColor
                A reference to the color for the text  displayed
                according to the font. Defaults to white.
            bg_color: RGBColor
                A reference to the background color  over  which
                the  text  will be displayed.  If None,  text is
                put with transparency, in which case an artifact 
                is  added to help the easy read of the text.  If
                set,  text is put into a rectangle  filled  with
                this background color. Defaults to None.
            bold: bool
                Set this to True to get an italic  font.  Prefer
                to  instantiate  class  BoldFont for such a use.
                Defaults to False (i.e. normal font).
            italic: bool
                Set this to True to get an italic  font.  Prefer
                to  instantiate class ItalicFont for such a use.
                Defaults to False (i.e. normal font).
            sans_serif: bool
                Set this to True to get  a  'sans-serif'  police
                used.  Set  it to False to get a 'serif' police.
                Defauts to True (i.e. 'sans-serif' police).
        
        Raises:
            AssertionError: some assertion failed.
        '''
        assert size > 5
        
        self.size = None
        self.set_color( color )
        self.bg_color = bg_color.copy() if bg_color is not None else None
        self.bold = bold
        self.italic = italic
        self.sans_serif = sans_serif
        
        self.cv_font = cv2.FONT_HERSHEY_SIMPLEX if sans_serif else cv2.FONT_HERSHEY_COMPLEX
        if italic:
            self.cv_font |= cv2.FONT_ITALIC
        
        self.thickness = 2 if bold else 1
        
        self.set_size( size )

    #-------------------------------------------------------------------------
    def copy(self, other: FontRef = None) -> Optional[ FontRef ]:
        '''Copies a font.
        
        Args:
            other: Font
                Either a reference to a font to be copied in this
                font,  or  None in which case a copy of this font
                is returned.
        
        Returns:
            Either nothing if 'other' is set, or a reference to a
            new  instance  of Font initialized with the values of 
            this font attributes.
        '''
        if other is None:
            return Font( self.size,
                         self.color,
                         self.bg_color if self.bg_color is not None else None,
                         self.bold,
                         self.italic,
                         self.sans_serif )
        else:
            self.size = other.size
            self.set_color( other.color )
            self.bg_color = other.bg_color.copy() if other.bg_color is not None else None,
            self.bold = other.bold
            self.italic = other.italic
            self.sans_serif = other.sans_serif
            self.cv_font = other.cv_font
            self.thickness = other.thickness
            self.font_scale = other.font_scale

    #-------------------------------------------------------------------------
    def draw_text(self, view    : Viewable    ,
                        pos     : Point       ,
                        text    : str         ,
                        b_shadow: bool = True ,
                        b_forced: bool = False ) -> None:
        '''Draws specified text with this font.
        
        Notice: next version could propose transparency for
                background and blurring background also  if
                no background rectangle is set.
        
        Args:
            view: Viewable
                A reference to the embedding view.
            pos: Point
                The absolute (x, y) position of the text in 
                the specified view.
            text: str
                The text to be drawn.
            b_shadow: bool
                Set this to True to get a shadowing of  the
                text, or False otherwise. Defaults to True.
            b_forced: bool
                Set this to True to force  the  drawing  of
                the text within the specified viewable. Set
                it to False otherwise. Defaults to False.
        '''
        if self.bg_color is None:
            if b_shadow:
                # artifact to get readable chars in frames while transparency is on 
                bg_color = (0,0,0)
                offset = 1
    
                view.content = cv2.putText( view.content,
                                            text,
                                            (pos + offset).to_tuple(),
                                            self.cv_font,
                                            self.font_scale,
                                            bg_color,
                                            self.thickness,
                                            cv2.LINE_AA )
        else:
            # put chars over background solid color
            text_size, baseline = cv2.getTextSize( text, self.cv_font, self.font_scale, self.thickness )
            cv2.rectangle( view.content,
                           pos + Offset(0, baseline + self.thickness),
                           pos + Offset(*text_size),
                           self.bg_color.color )
            
        view.content = cv2.putText( view.content,
                                    text,
                                    pos.to_tuple(),
                                    self.cv_font,
                                    self.font_scale,
                                    self.color.color,
                                    self.thickness,
                                    cv2.LINE_AA )
        if b_forced:
            view.draw()

    #-------------------------------------------------------------------------
    def forced_draw_text(self, view    : Viewable   ,
                               pos     : Point      ,
                               text    : str        ,
                               b_shadow: bool = True ) -> None:
        '''Draws specified text with this font.
        
        The drawing  is  forced.  Internally  calls  method
        '.draw_text()'.
        
        Args:
            view: Viewable
                A reference to the embedding view.
            pos: Point
                The absolute (x, y) position of the text in 
                the specified view.
            text: str
                The text to be drawn.
            b_shadow: bool
                Set this to True to get a shadowing of  the
                text, or False otherwise. Defaults to True.
        '''
        self.draw_text( view, pos, text, b_shadow, True )

    #-------------------------------------------------------------------------
    def get_text_height(self, text: str) -> int:
        '''Returns the width (pixels) of the specified text.
        '''
        return self.get_text_size( text )[ 1 ]

    #-------------------------------------------------------------------------
    def get_text_size(self, text: str) -> int:
        '''Returns the width (pixels) of the specified text.
        '''
        return cv2.getTextSize( text, self.cv_font, self.font_scale, self.thickness )[ 0 ]

    #-------------------------------------------------------------------------
    def get_text_width(self, text: str) -> int:
        '''Returns the width (pixels) of the specified text.
        '''
        return self.get_text_size( text )[ 0 ]

    #-------------------------------------------------------------------------
    def set_color(self, new_color: RGBColor) -> FontRef:
        '''Changes the color of this font.
        
        Args:
            new_color: RGBColor
                A reference to the new color.
        
        Returns:
            A reference to this instance of Font.
        '''
        self.color = new_color.copy()
        return self

    #-------------------------------------------------------------------------
    def set_size(self, size: int) -> FontRef:
        '''Changes the size of the police that is associated with this font. 
        
        Args:
            size: int
                The size of the police.
        
        Returns:
            A reference to this font.
        '''
        if self.size is None or size != self.size:
            self.size = size
            self.font_scale = cv2.getFontScaleFromHeight( self.cv_font,
                                                          size,
                                                          self.thickness )
        return self
    

#=============================================================================
class BoldFont( Font ):
    """The class of text fonts with bold body.
    
    Notice: This is a minimal implementation that should help 
            using OpenCV when putting text in video frames.
    """
    #-------------------------------------------------------------------------
    def __init__(self, size    : int,
                       color   : RGBColor = WHITE,
                       bg_color: RGBColor = None ,
                       sans_serif: bool   = True  ) -> None:
        '''Constructor.
        
        Args:
            size: int
                The size of the police.
            color: RGBColor
                A reference to the color for the text  displayed
                according to the font. Defaults to white.
            bg_color: RGBColor
                A reference to the background color  over  which
                the  text  will be displayed.  If None,  text is
                put with transparency, in which case an artifact 
                is  added to help the easy read of the text.  If
                set,  text is put into a rectangle  filled  with
                this background color. Defaults to None.
            sans_serif: bool
                Set this to True to get  a  'sans-serif'  police
                used.  Set  it to False to get a 'serif' police.
                Defauts to True (i.e. 'sans-serif' police).
        '''
        super().__init__( size, color, bg_color, True, False, sans_serif )
 

#=============================================================================
class ItalicFont( Font ):
    """The class of text fonts with italic body.
    
    Notice: This is a minimal implementation that should help 
            using OpenCV when putting text in video frames.
    """
    #-------------------------------------------------------------------------
    def __init__(self, size    : int,
                       color   : RGBColor = WHITE,
                       bg_color: RGBColor = None ,
                       sans_serif: bool   = True  ) -> None:
        '''Constructor.
        
        Args:
            size: int
                The size of the police.
            color: RGBColor
                A reference to the color for the text  displayed
                according to the font. Defaults to white.
            bg_color: RGBColor
                A reference to the background color  over  which
                the  text  will be displayed.  If None,  text is
                put with transparency, in which case an artifact 
                is  added to help the easy read of the text.  If
                set,  text is put into a rectangle  filled  with
                this background color. Defaults to None.
        '''
        super().__init__( size, color, bg_color, False, True, sans_serif )


#=============================================================================
class BoldItalicFont( Font ):
    """The class of text fonts with bold italic body.
    
    Notice: This is a minimal implementation that should help 
            using OpenCV when putting text in video frames.
    """
    #-------------------------------------------------------------------------
    def __init__(self, size    : int,
                       color   : RGBColor = WHITE,
                       bg_color: RGBColor = None ,
                       sans_serif: bool   = True  ) -> None:
        '''Constructor.
        
        Args:
            size: int
                The size of the police.
            color: RGBColor
                A reference to the color for the text  displayed
                according to the font. Defaults to white.
            bg_color: RGBColor
                A reference to the background color  over  which
                the  text  will be displayed.  If None,  text is
                put with transparency, in which case an artifact 
                is  added to help the easy read of the text.  If
                set,  text is put into a rectangle  filled  with
                this background color. Defaults to None.
            sans_serif: bool
                Set this to True to get  a  'sans-serif'  police
                used.  Set  it to False to get a 'serif' police.
                Defauts to True (i.e. 'sans-serif' police).
        '''
        super().__init__( size, color, bg_color, True, True, sans_serif )
 
#=====   end of   src.GUIItems.font   =====#
