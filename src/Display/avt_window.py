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
import cv2
import numpy as np

from threading   import Lock
from typing      import Tuple

from src.Utils.types import PixelColor
from .view           import View


#=============================================================================
class AVTWindow:
    """The base class for all windows in Archery Video Training application.
    """
    #-------------------------------------------------------------------------
    def __init__(self, name  : str = None,
                       title : str = None,
                       width : int = None,
                       height: int = None,
                       bg_color: PixelColor = (32, 32, 32),
                       *,
                       full_screen: bool = False) -> None:
        '''Constructor.
        
        Args:
            name: str
                The name for this window.  This is used by  OpenCV
                for referencing windows. If not set, a default and
                unique one will be created.  Notice:  the  default 
                naming is thread safe.
            title: str
                The title of this window,  as display in  the  top
                bar of it.
            width: int
                The wished width for this window  when  displayed.
                Is  not  set,  will  be displayed according to the
                width of its content.  Ignored when  'full_screen'
                is set to True. Defaults to not set.
            height: int
                The wished height for this window when  displayed.
                Is  not  set,  will  be displayed according to the
                height of its content.  Ignored when 'full_screen'
                is set to True. Defaults to not set.
            bg_color: (int,int,int)
                The background solid color for this window.
            full_screen: bool
                Set this to True to get a full  screen  displayed
                window.  No title bar will then be displayed. This
                takes precedence over 'width'  and  'height'  when
                set  to  True.  Defaults  to  False (i.e. not full
                screen). This argument must be named at call time.
        
        Raises:
            ValueError: width and height must be both set or both 
                None.  If not,  a ValueError exception is raised.
                Furthermore,  this exception is raised when width
                or size are negative or zero.
        '''
        self.lock = Lock()
        
        self.name = self._get_default_name() if name is None else str(name)
        
        self.bg_color = bg_color
        self.full_screen = full_screen
        self.fixed_size = True
        
        if full_screen:
            cv2.namedWindow( self.name, cv2.WINDOW_FULLSCREEN )
            
        elif width is None:
            if height is not None:
                raise ValueError( 'args width and height must be both None or both set.' )
            cv2.namedWindow( self.name, cv2.WINDOW_AUTOSIZE | cv2.WINDOW_KEEPRATIO | cv2.WINDOW_GUI_EXPANDED )
            self.fixed_size = False
            self.height, self.width = self.get_size()
            
        else:
            if width is None:
                raise ValueError( 'args width and height must be both None or both set.' )
            if width <= 0 or height <= 0:
                raise ValueError ( f"width and height ({width}, {height}) must be both greater than 0" )
            cv2.namedWindow( self.name, cv2.WINDOW_NORMAL | cv2.WINDOW_KEEPRATIO | cv2.WINDOW_GUI_EXPANDED )
            cv2.resizeWindow( self.name, width, height )
            self.height, self.width = height, width

        self.set_title( f"AVT Window # {self.__WINDOWS_COUNT}" if title is None else title )
        
        self.content = (np.zeros( (height, width, 3), dtype=np.uint8 ) + self.bg_color).astype( np.uint8 )
            
    #-------------------------------------------------------------------------
    def draw(self, hit_delay_ms: int = 1) -> int:
        '''Draws a content into this window.
        
        Notice: the content will automatically be  resized  to
                the  window  current  size  if this window has 
                been created with a specified size.
        
        Args:
            hit_delay_ms: int
                A delay,  expressed in milliseconds,  used for 
                the scanning of keyboards hits.  If any key is 
                hit within this delay,  its  integer  code  is 
                immediately  returned  as  the  result of this 
                method,  otherwise -1 is returned. If delay is 
                0 or  negative,  the  application  is  stopped 
                until a key is hit.  Defaults to  1 ms,  which 
                is the shortest delay that is available before 
                completing this method.
        
        Returns:
            The integer code of the key  that  was  hit  while 
            displaying   this  content,   or  -1 if no key was
            hit after expressed delay.
        '''
        if not self.fixed_size:
            #-- the window size adapts itself to the content size
            cv2.imshow( self.name, self.content )

        else:
            #-- the content size adapts itself to the window size
            content_height, content_width = self.content.shape[:2]
            window_width, window_height = self.get_size()
            
            height_ratio = window_height / content_height
            width_ratio  = window_width  / content_width
            ratio = height_ratio if height_ratio <= width_ratio else width_ratio
        
            if ratio != 1.0:
                window_content = np.zeros( (window_height, window_width, 3), np.uint8 )
                new_content = cv2.resize( self.content, None,
                                          fx=ratio, fy=ratio,
                                          interpolation=cv2.INTER_LINEAR )
                
                new_height, new_width = new_content.shape[:2]
                if new_width > window_width:
                    new_width = window_width
                if new_height > window_height:
                    new_height = window_height
                    
                x = (window_width - new_width) // 2
                y = (window_height - new_height) // 2
                
                window_content[ y:y+new_height,
                                x:x+new_width, : ] = new_content[ :new_height, :new_width, : ] 
                
                cv2.imshow( self.name, window_content )
            
            else:
                cv2.imshow( self.name, self.content )
        
        return cv2.waitKey( hit_delay_ms )

    #-------------------------------------------------------------------------
    def get_pos(self) -> Tuple[int, int]:
        '''Returns the current (x, y) position of this window, expressed in pixels.
        '''
        return  cv2.getWindowImageRect( self.name )[:2]

    #-------------------------------------------------------------------------
    def get_rect(self) -> Tuple[int, int, int, int]:
        '''Returns the current rectangle (x, y, width, height) of this window, expressed in pixels.
        '''
        return  cv2.getWindowImageRect( self.name )

    #-------------------------------------------------------------------------
    def get_size(self) -> Tuple[int, int]:
        '''Returns the current (width, height) of this window, expressed in pixels.
        '''
        return  cv2.getWindowImageRect( self.name )[2:]

    #-------------------------------------------------------------------------
    def insert_view_content(self, view: View) -> None:
        '''Inserts the content of a view in this window content.
        
        Args:
            view: View
                A reference to the view from which the content
                is to be inserted in this window content.
        '''
        content_height, content_width = self.content.shape[:2]
        
        width  = min( view.width , content_width  - view.x )
        height = min( view.height, content_height - view.y )
        
        self.content[ view.y:view.y+height,
                      view.x:view.x+width, : ] = view.content[ :height, :width, : ]
        

    #-------------------------------------------------------------------------
    def set_title(self, title: str) -> None:
        '''Sets the title of this window as shown in its top bar.
        
        Args:
            title: str
                The text for this window title.
        '''
        cv2.setWindowTitle( self.name, str(title) )

    #-------------------------------------------------------------------------
    def _get_default_name(self) -> str:
        '''Returns a unique default name for this window.
        
        Notice: this is a private method which should not
                have  to be called in inheriting classes. 
                It is thread safe.
        '''
        with self.lock:
            name = f"AVT-Window-{self.__WINDOWS_COUNT:03d}"
            self.__WINDOWS_COUNT += 1
        return name

    #-------------------------------------------------------------------------
    # Class data
    __WINDOWS_COUNT = 0

#=====   end of   src.Display.avt_window   =====#
