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
from threading import Thread

from src.App.avt_config  import AVTConfig
from .avt_view_prop      import AVTViewProp
from .view               import AVTWindowRef
from src.Utils.rgb_color import BLACK, RGBColor
from src.Shapes.rect     import Rect


#=============================================================================
class TargetView( Thread, AVTViewProp ):
    """The class description.
    """
    def __init__(self, parent             : AVTWindowRef,
                       x                  : float,
                       y                  : float,
                       width              : float,
                       height             : float,
                       parent_rect        : Rect = None,
                       b_periodically_hide: bool = False ) -> None:
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
            parent_rect: Rect
                A reference to the  rectangle  in  which  this
                proportional  view takes place into the parent
                window.  If None,  the whole view size is used
                instead. Defaults to None.
            b_periodically_hide: bool
                Set this to True to get  this  view  displayed 
                for a prefixed delay, then for it to disappear 
                for another prefixed delay.
        
        Raises:
            ValueError:  Some  of  the  coordinates  or  sizes 
                values are outside interval [0.0, 1.0].
        '''
        self.b_periodical = b_periodically_hide
        self.b_shown = True
        
        self.simulated_dist  = None
        self.true_dist       = None
        self.displayed_ratio = None
        self.target          = None
        
        Thread.__init__( self, name='target-thrd' )
        AVTViewProp.__init__( self, parent, x, y, width, height, parent_rect )

    #-------------------------------------------------------------------------
    def draw(self) -> None:
        '''Draws the content of this view.
        '''
        if self.b_shown:
            self.draw_target()
            self.draw_borders()
            super().draw()

    #-------------------------------------------------------------------------
    def draw_borders(self) -> None:
        '''Draws lines on this view borders.
        '''
        bg_color = RGBColor( *AVTConfig.DEFAULT_BACKGROUND.color )
        
        self.content[  0,  : ] = bg_color.color
        self.content[  1,  : ] = bg_color.color
        self.content[ -1,  : ] = bg_color.color
        self.content[ -2,  : ] = bg_color.color
        self.content[  :,  0 ] = bg_color.color
        self.content[  :,  1 ] = bg_color.color
        self.content[  :, -1 ] = bg_color.color
        self.content[  :, -2 ] = bg_color.color

        self.content[ 2, 2:-1 ]  = (bg_color // 2).color
        self.content[ 2:-2, 2 ]  = (bg_color // 2).color
        self.content[ -3, 3:-1 ] = (bg_color * 3).color
        self.content[ 3:-3, -2 ] = (bg_color * 3).color
        self.content[ 3, 3:-2 ]  = BLACK.color
        self.content[ 4:-3, 3 ]  = BLACK.color
        self.content[ -4, 4:-2 ] = (bg_color * 1.5).color
        self.content[ 4:-3, -3 ] = (bg_color * 1.5).color

    #-------------------------------------------------------------------------
    def draw_target(self) -> None:
        '''Draws the previously selected target with its selected size also.
        '''
        if self.target is not None and self.displayed_ratio is not None:
            # evaluate dimensions of displayed target
            self.target.draw( self, self.ratio )
        else:
            self.fill_background()

    #-------------------------------------------------------------------------
    def hide(self) -> None:
        '''Hides this Target View.
        
        See method 'show()'.
        '''
        self.b_shown = False

    #-------------------------------------------------------------------------
    def select_distances(self) -> None:
        '''Selects the simulated and the true distances.
        
        The simulated distance is the distance that the archer 
        wants to simulate with the selected target.
        The  true distance is the distance between the display 
        screen and the archer.
        '''
        self.simulated_dist = None
        self.true_dist      = None
        self._evaluate_display_ratio()

    #-------------------------------------------------------------------------
    def select_target(self) -> None:
        '''Selects a target from the list of all available targets.
        '''
        self.target = None

    #-------------------------------------------------------------------------
    def show(self) -> None:
        '''Shows this Target View.
        
        See method 'hide()'.
        '''
        self.b_shown = True
        self.draw()

    #-------------------------------------------------------------------------
    def _evaluate_display_ratio(self) -> None:
        '''Evaluates the resizing of the displayed target on display.
        
        This ratio is a function of:
            - the simulated distance
            - the true distance
            - and the dpi of the used display - which is also
              a function of the display resolution.
        '''
        self.displayed_ratio = None

#=====   end of   src.Display.target_view   =====#
