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
#    class SliderBase
#    class IntSlider
#    class FloatSlider
#

#=============================================================================
import cv2
from typing import List, Tuple

from src.Utils.rgb_color                     import DARK_RED, GRAY, RGBColor, WHITE
from src.GUIItems.font                       import Font
from src.GUIItems.Controls.gui_control_base  import GUIControlBase
from src.Utils.types                         import Numeric
from src.Shapes.point                        import Point
from src.Display.view                        import View 


#=============================================================================
class SliderBase( GUIControlBase ):
    """The base class for all control sliders.
    """
    #-------------------------------------------------------------------------
    def __init__(self, x            : int      = None,
                       y            : int      = None,
                       width        : int      = None,
                       height       : int      = None,
                       min_value    : Numeric  =   0 ,
                       max_value    : Numeric  = 100 ,
                       current_value: Numeric  =  50 ,
                       bar_color    : RGBColor = GRAY,
                       cursor_color : RGBColor = DARK_RED,
                       text_font    : Font     = None,
                       shadow_height: int      =   0 ,
                       visible      : bool     = True,
                       enabled      : bool     = True,
                       active       : bool     = True,
                       *,          
                       pos          : Point    = None) -> None:
        '''Constructor.
        
        Args:
            x, y : int
                The position of this label in the view.This
                is  the position of the top-left corner  of 
                this control. If argument 'pos' is set, 'x' 
                and 'y' are both ignored. If argument 'pos' 
                is None, 'x' and 'y' must be both set.
            width, height: int
                The width and the height of this control.
            min_value: float or int
                The minimum value for this slider. Defaults
                to 0.
            max_value: float or int
                The maximum value for this slider. Defaults
                to 100.
            current_value: float or int
                The current value of this slider.  Defaults
                to 50.
            bar_color: RGBColor
                A reference to the color of the slider bar.
                Default to GRAY.
            cursor_color: RGBColor
                A reference to the color of the  cursor  on
                the slider bar. Defaults to DARK_RED.
            text_font: Font
                A reference to the font for  text  that  is
                associated  with  this slider.  This mostly
                is the font for numbers.  Defaults to None,
                which means use of slider default font.
            shadow_height: int
                Shadow may be cast at bottom-right side  of
                sliders.  This is the height of this slider
                over the view  background.  Defaults  to  0
                (i.e. no shadow cast).
            visible: bool
                Set this to True to get this control  drawn
                at  its  drawing  time.  Set it to False to
                hide this control at drawing time. Defaults
                to True.
            enabled: bool
                Set this to True to  get  this  control  as 
                being enabled.  Set it to False to get this
                control  as  being  disabled.  Defaults  to
                True.
            active: bool
                Set this to True to  get  this  control  as
                being active.  Set it to False to get it as
                being inactive. Defaults to True.
            pos: Point
                If set, takes precedence over x and y. This 
                is  the  position of the top-left corner of 
                this control.  This argument must be  named 
                when passed at construction time. 
        
        Raises:
            AssertionError:  if 'pos' is not set,  both  'x' 
                and 'y' must be and are not both set.
            ValueError:  'max_value'  is  not  greater  than
                'min_value'  or  'current_value'  is outside
                bounds.
        '''
        if max_value <= min_value:
            raise ValueError( f"max_value ({max_value}) must be greater than min_value ({min_value}) in slider constructor." )
        if not (min_value <= current_value <= max_value):
            raise ValueError( f"current_value ({current_value}) is out of interval [{min_value}, {max_value}]." )
        
        super().__init__( x, y,
                          width  if width  > 3 else 3,
                          height if height > 3 else 3,
                          visible, enabled, active, pos=pos )
        
        self.min_value = min_value
        self.max_value = max_value
        self.value     = current_value
        
        self.bar_color = bar_color
        self.cursor_color = cursor_color
        self.set_font( text_font )
        self.shadow_height = shadow_height

    #-------------------------------------------------------------------------
    def is_horizontal(self) -> bool:
        '''Returns True if this slider is horizontal.
        '''
        return self.width >= self.height

    #-------------------------------------------------------------------------
    def is_vertical(self) -> bool:
        '''Returns True if this slider is vertical.
        '''
        return not self.is_horizontal()

    #-------------------------------------------------------------------------
    def set_font(self, text_font: Font) -> None:
        '''Sets the font for the text associated with this slider.
        
        Args:
            text_font: Font
                A reference to the font to be used, mostly
                to put numbers near the slider.
        '''
        self.enabled_font = text_font if text_font is not None else self.DEFAULT_FONT
        self.disabled_font = self.enabled_font.copy()
        self.disabled_font.color //= 2.5

    #-------------------------------------------------------------------------
    def _draw(self, view: View) -> None:
        '''Draws this slider in its parent view.
        
        This is the protected part of the drawing method. It
        is  automatically  called  by  method  'draw()'.  It
        relates to the specific part of  drawing  inheriting
        controls.
        
        Args:
            view: View
                This is a reference to  the  parent  view  of 
                this control. It is provided as a convenience 
                to this protected method.
        '''
        self._draw_bar( view )
        self._draw_shadow( view )
        self._draw_cursor( view )
        self._draw_text( view, self._evaluate_text_font() )
        
    #-------------------------------------------------------------------------
    def _draw_bar(self, view: View) -> None:
        '''Draws the bar of this slider.
        
        Args:
            view: View
                This is a reference to the parent view of 
                this control. 
        '''
        if self.enabled:
            color = self.bar_color
        else:
            color = self.bar_color // 2
        bright_color = color * 1.40
        dark_color = color // 3
            
        cv2.rectangle( view.content,
                       (self.x+1, self.y+1),
                       (self.x + self.width-1, self.y + self.height-1),
                       color.color, cv2.FILLED, cv2.LINE_AA )
        cv2.line( view.content,
                  (self.x, self.y),
                  (self.x + self.width - 1, self.y),
                  bright_color.color, 1, cv2.LINE_AA )
        cv2.line( view.content,
                  (self.x, self.y+1),
                  (self.x, self.y + self.height - 1),
                  bright_color.color, 1, cv2.LINE_AA )
        cv2.line( view.content,
                  (self.x, self.y+self.height),
                  (self.x + self.width, self.y + self.height),
                  dark_color.color, 1, cv2.LINE_AA )
        cv2.line( view.content,
                  (self.x + self.width, self.y),
                  (self.x + self.width, self.y + self.height - 1),
                  dark_color.color, 1, cv2.LINE_AA )

        self._draw_ticks( view )
        
    #-------------------------------------------------------------------------
    def _draw_cursor(self, view: View) -> None:
        '''Draws the cursor of this slider.
        
        Args:
            view: View
                This is a reference to the parent view of 
                this control. 
        '''
        if self.enabled:
            color = self.cursor_color if self.active else self.cursor_color // 1.5
        else:
            color = self.bar_color // 2
        
        x, y = self._evaluate_cursor_pos()
        
        if self.is_horizontal():
            # horizontal slider
            cv2.rectangle( view.content,
                           (x - self.CURSOR_WIDTH // 2, y),
                           (x + self.CURSOR_WIDTH // 2, y + self.height + 3),
                           color.color, cv2.FILLED, cv2.LINE_AA )
            cv2.line( view.content, (x,y+2), (x,y+self.height+1), WHITE.color, 1, cv2.LINE_AA )
        else:
            # vertical slider
            cv2.rectangle( view.content,
                           (x, y),
                           (x + self.width + 3, y + self.CURSOR_WIDTH),
                           color.color, cv2.FILLED, cv2.LINE_AA )
            cv2.line( view.content, (x+2,y), (x+self.width+1,y), WHITE.color, 1, cv2.LINE_AA )

    #-------------------------------------------------------------------------
    def _draw_cursor_value(self, view     : View,
                                 text_font: Font,
                                 value    : str  ) -> None:
        '''Internal drawing of values under ticks.
        
        Args:
            view: View
                A reference to the embedding view.
            text_font: Font
                A reference to the font to be used to draw
                ticks values.
            value: str
                The text of the cursor value.
        '''
        width = text_font.get_text_width( value )
        x, y = self._evaluate_cursor_pos()
        
        if self.is_horizontal():
            # horizontal slider
            text_font.draw_text( view,
                                 Point(x - width // 2,
                                       y + self.height + 5 + text_font.size),
                                 value )
        else:
            # vertical slider
            text_font.draw_text( view, Point(x + 2, y + text_font.size // 2), value )

    #-------------------------------------------------------------------------
    def _draw_shadow(self, view: View) -> None:
        '''Draws the shadow of this slider cast on the view background.
        
        Args:
            view: View
                This is a reference to the parent view of 
                this control. 
        '''
        if self.shadow_height > 0:
            # bottom shadow
            left_x   = max( 0, self.x + self.shadow_height )
            right_x  = min( self.x + self.width + self.shadow_height + 1, view.width )
            top_y    = min( self.y + self.height, view.height )
            bottom_y = max( self.y + self.height + self.shadow_height + 1 , view.height )
            view.content[ top_y:bottom_y, left_x:right_x, : ] = view.content[ top_y:bottom_y, left_x:right_x, : ] // 2
            
            # right shadow
            left_x   = min( view.width, self.x + self.width + 1 )
            right_x  = min( view.width, self.x + self.width + 1 + self.shadow_height )
            top_y    = min( view.height, self.y + 1 + self.shadow_height )
            bottom_y = min( view.height, self.y + 1 + self.height )
            view.content[ top_y:bottom_y, left_x:right_x, : ] = view.content[ top_y:bottom_y, left_x:right_x, : ] // 2
                    
    #-------------------------------------------------------------------------
    def _draw_text(self, view: View, text_font: Font) -> None:
        '''Draws the text associated with this slider.
        
        This method does nothing in this base  class  and
        should  be  overwritten  in inheriting classes if
        needed.
        
        Args:
            view: View
                This is a reference to the parent view of 
                this control.
            text_font: Font
                A reference to the font for the  text  to
                be drawn.  Notice: this font is evaluated
                outside this method.
        '''
        pass            
                    
    #-------------------------------------------------------------------------
    def _draw_ticks(self, view: View) -> None:
        '''Draws ticks within this slider.
        
        This method must  be  implemented  in  inheriting
        classes.
        
        Args:
            view: View
                This is a reference to the parent view of 
                this control.
        
        Raise:
            NotImplementedError: this method is not impl-
                emented in inheriting class.
        '''
        raise NotImplementedError( f"method '_draw_ticks()' is not implemented in class {self.__class__.__name__}" )            

    #-------------------------------------------------------------------------
    def _draw_ticks_values(self, view     : View,
                                 text_font: Font,
                                 min_val  : str ,
                                 max_val  : str  ) -> None:
        '''Internal drawing of values under ticks.
        
        Args:
            view: View
                A reference to the embedding view.
            text_font: Font
                A reference to the font ti be used to draw
                ticks values.
            min_val: str
                The text of the minimum  value  (left-most 
                or bottom_most tick in slider).
            max_val: str
                The text of the maximum value  (right-most 
                or top_most tick in slider).
        '''
        min_width = text_font.get_text_width( min_val )      
        max_width = text_font.get_text_width( max_val )
        
        self.list_ticks = self._evaluate_ticks_pos( view )
        min_x, min_y = self.list_ticks[0][0]
        max_x, max_y = self.list_ticks[-1][0]
        
        min_x = max( 5, min_x - min_width // 2 )
        max_x = min( view.width - max_width - 5, max_x - max_width // 2 )
        
        text_font.draw_text( view, Point(min_x, min_y + self.height + text_font.size + 2), min_val )
        text_font.draw_text( view, Point(max_x, max_y + self.height + text_font.size + 2), max_val )

    #-------------------------------------------------------------------------
    def _evaluate_cursor_pos(self) -> Tuple[int, int]:
        '''Evaluates the current position of this slider cursor.
        
        The returned position is the central position of the
        cursor on the slider.
        '''
        if self.is_horizontal():
            # horizontal slider
            x = self.x + round( (self.value - self.min_value) * self.width / (self.max_value - self.min_value) )
            y = self.y - 2
        else:
            # vertical slider
            x = self.x - 2
            y = self.y + round( self.value * self.height / (self.max_value - self.min_value) )

        return (x, y)

    #-------------------------------------------------------------------------
    def _evaluate_text_font(self) -> Font:
        '''Internally evaluates the final font for associated text.
        
        Returns:
            A reference to the finally evaluated font.
        '''
        return self.enabled_font if self.enabled else self.disabled_font
              
    #-------------------------------------------------------------------------
    def _evaluate_ticks_pos(self, view: View) -> List[ Tuple[ Tuple[int,int], Tuple[int,int] ] ]:
        '''Evaluates the ticks positions within this slider.
        
        Args:
            view: View
                This is a reference to the parent view of 
                this control.
        '''
        main_size = max( self.width, self.height ) - 4
        ticks_count = self.max_value - self.min_value + 1
        if 10 * (ticks_count - 1) > main_size:
            ticks_count = main_size // 10 + 1
        step = main_size / (ticks_count - 1)
        
        if self.is_horizontal():
            return [ ( (self.x + 2 + round(i * step), self.y + 1),
                       (self.x + 2 + round(i * step), self.y + self.height - 1) ) for i in range( ticks_count ) ]
        else:
            return [ ( (self.x + 1, self.y + 2 + round(i * step)),
                       (self.x + self.width - 1,self.y + 2 + round(i * step)) ) for i in range( ticks_count ) ]

    #-------------------------------------------------------------------------
    # Class data
    CURSOR_WIDTH = 9
    DEFAULT_FONT = Font( 8, GRAY )


#=============================================================================
class IntSlider( SliderBase ):
    """The class of control sliders with integer values.
    """
    #-------------------------------------------------------------------------
    def __init__(self, x            : int      = None,
                       y            : int      = None,
                       width        : int      = None,
                       height       : int      = None,
                       min_value    : int      =   0 ,
                       max_value    : int      = 100 ,
                       current_value: int      =  50 ,
                       bar_color    : RGBColor = GRAY,
                       cursor_color : RGBColor = DARK_RED,
                       text_font    : Font     = None,
                       shadow_height: int      =   0 ,
                       visible      : bool     = True,
                       enabled      : bool     = True,
                       active       : bool     = True,
                       *,          
                       pos          : Point    = None) -> None:
        '''Constructor.

        Args:
            x, y : int
                The position of this label in the view.This
                is  the position of the top-left corner  of 
                this control. If argument 'pos' is set, 'x' 
                and 'y' are both ignored. If argument 'pos' 
                is None, 'x' and 'y' must be both set.
            width, height: int
                The width and the height of this control.
            min_value: float or int
                The minimum value for this slider. Defaults
                to 0.
            max_value: float or int
                The maximum value for this slider. Defaults
                to 100.
            current_value: float or int
                The current value of this slider.  Defaults
                to 50.
            bar_color: RGBColor
                A reference to the color of the slider bar.
                Default to GRAY.
            cursor_color: RGBColor
                A reference to the color of the  cursor  on
                the slider bar. Defaults to DARK_RED.
            text_font: Font
                A reference to the font for  text  that  is
                associated  with  this slider.  This mostly
                is the font for numbers.  Defaults to None,
                which means use of slider default font.
            shadow_height: int
                Shadow may be cast at bottom-right side  of
                sliders.  This is the height of this slider
                over the view  background.  Defaults  to  0
                (i.e. no shadow cast).
            visible: bool
                Set this to True to get this control  drawn
                at  its  drawing  time.  Set it to False to
                hide this control at drawing time. Defaults
                to True.
            enabled: bool
                Set this to True to  get  this  control  as 
                being enabled.  Set it to False to get this
                control  as  being  disabled.  Defaults  to
                True.
            active: bool
                Set this to True to  get  this  control  as
                being active.  Set it to False to get it as
                being inactive. Defaults to True.
            pos: Point
                If set, takes precedence over x and y. This 
                is  the  position of the top-left corner of 
                this control.  This argument must be  named 
                when passed at construction time. 
        
        Raises:
            AssertionError:  if 'pos' is not set,  both  'x' 
                and 'y' must be and are not both set.
            ValueError:  'max_value'  is  not  greater  than
                'min_value'  or  'current_value'  is outside
                bounds.
        '''
        super().__init__( x                     ,
                          y                     ,
                          width                 ,
                          height                ,
                          round( min_value     ),
                          round( max_value     ),
                          round( current_value ),
                          bar_color             ,
                          cursor_color          ,
                          text_font             ,
                          shadow_height         ,
                          visible               ,
                          enabled               ,
                          active                ,
                          pos=pos                 )
                    
    #-------------------------------------------------------------------------
    def _draw_text(self, view: View, text_font: Font) -> None:
        '''Draws the text associated with this slider.
        
        Args:
            view: View
                This is a reference to the parent view of 
                this control.
            text_font: Font
                A reference to the font for the  text  to
                be drawn.  Notice: this font is evaluated
                outside this method.
        '''
        # draws min and max values centered on ticks
        min_val = f"{self.min_value:d}"  
        max_val = f"{self.max_value:d}"
        self._draw_ticks_values( view, text_font, min_val, max_val )

        # then, draws cursor value centered on cursor tick
        value = f"{self.value:d}"            
        self._draw_cursor_value( view, text_font, value )
        
    #-------------------------------------------------------------------------
    def _draw_ticks(self, view: View) -> None:
        '''Draws ticks within this slider.
        
        Args:
            view: View
                This is a reference to the parent view of 
                this control.
        '''
        self.ticks_pos = self._evaluate_ticks_pos( view )

        for pos in self.ticks_pos:
            cv2.line( view.content, pos[0], pos[1], (self.bar_color // 2.5).color, 1, cv2.LINE_AA )


#=============================================================================
class FloatSlider( SliderBase ):
    """The class of control sliders with float values.
    """
    #-------------------------------------------------------------------------
    def __init__(self, x            : int      = None,
                       y            : int      = None,
                       width        : int      = None,
                       height       : int      = None,
                       min_value    : float    = 0.0 ,
                       max_value    : float    = 1.0 ,
                       current_value: float    = 0.5 ,
                       bar_color    : RGBColor = GRAY,
                       cursor_color : RGBColor = DARK_RED,
                       text_font    : Font     = None,
                       shadow_height: int      =   0 ,
                       visible      : bool     = True,
                       enabled      : bool     = True,
                       active       : bool     = True,
                       *,          
                       pos          : Point    = None) -> None:
        '''Constructor.

        Args:
            x, y : int
                The position of this label in the view.This
                is  the position of the top-left corner  of 
                this control. If argument 'pos' is set, 'x' 
                and 'y' are both ignored. If argument 'pos' 
                is None, 'x' and 'y' must be both set.
            width, height: int
                The width and the height of this control.
            min_value: float or int
                The minimum value for this slider. Defaults
                to 0.
            max_value: float or int
                The maximum value for this slider. Defaults
                to 100.
            current_value: float or int
                The current value of this slider.  Defaults
                to 50.
            bar_color: RGBColor
                A reference to the color of the slider bar.
                Default to GRAY.
            cursor_color: RGBColor
                A reference to the color of the  cursor  on
                the slider bar. Defaults to DARK_RED.
            text_font: Font
                A reference to the font for  text  that  is
                associated  with  this slider.  This mostly
                is the font for numbers.  Defaults to None,
                which means use of slider default font.
            shadow_height: int
                Shadow may be cast at bottom-right side  of
                sliders.  This is the height of this slider
                over the view  background.  Defaults  to  0
                (i.e. no shadow cast).
            visible: bool
                Set this to True to get this control  drawn
                at  its  drawing  time.  Set it to False to
                hide this control at drawing time. Defaults
                to True.
            enabled: bool
                Set this to True to  get  this  control  as 
                being enabled.  Set it to False to get this
                control  as  being  disabled.  Defaults  to
                True.
            active: bool
                Set this to True to  get  this  control  as
                being active.  Set it to False to get it as
                being inactive. Defaults to True.
            pos: Point
                If set, takes precedence over x and y. This 
                is  the  position of the top-left corner of 
                this control.  This argument must be  named 
                when passed at construction time. 
        
        Raises:
            AssertionError:  if 'pos' is not set,  both  'x' 
                and 'y' must be and are not both set.
            ValueError:  'max_value'  is  not  greater  than
                'min_value'  or  'current_value'  is outside
                bounds.
        '''
        super().__init__( x            ,
                          y            ,
                          width        ,
                          height       ,
                          min_value    ,
                          max_value    ,
                          current_value,
                          bar_color    ,
                          cursor_color ,
                          text_font    ,
                          shadow_height,
                          visible      ,
                          enabled      ,
                          active       ,
                          pos=pos        )
                    
    #-------------------------------------------------------------------------
    def _draw_text(self, view: View, text_font: Font) -> None:
        '''Draws the text associated with this slider.
        
        Args:
            view: View
                This is a reference to the parent view of 
                this control.
            text_font: Font
                A reference to the font for the  text  to
                be drawn.  Notice: this font is evaluated
                outside this method.
        '''
        min_val = f"{self.min_value:.1f}"            
        max_val = f"{self.max_value:.1f}"
        
        min_width = text_font.get_text_width( min_val )      
        max_width = text_font.get_text_width( max_val )
        
        self.list_ticks = self._evaluate_ticks_pos( view )
        min_x, min_y = self.list_ticks[0][0]
        max_x, max_y = self.list_ticks[-1][0]
        max_x -= max_width
        
        text_font.draw_text( view, Point(min_x, min_y + self.height + 5), min_val )
        text_font.draw_text( view, Point(max_x, max_y + self.height + 5), max_val )

    #-------------------------------------------------------------------------
    def _draw_ticks(self, view: View) -> None:
        '''Draws ticks within this slider.
        
        Notice: no ticks are displayed with float values.
        
        Args:
            view: View
                This is a reference to the parent view of 
                this control.
        '''
        pass
    
#=====   end of   src.GUIItems.Controls.sliders   =====#
