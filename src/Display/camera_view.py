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

from threading import Event
from typing    import ForwardRef

from src.App.avt_config                      import AVTConfig
from .avt_view_prop                          import AVTViewProp
from .avt_window                             import AVTWindowRef
from src.Cameras.camera                      import Camera
from src.Cameras.camera_acquisition          import CameraAcquisition
from src.Cameras.camera_direct_display       import CameraDirectDisplay
from src.GUIItems.font                       import Font
from src.Utils.types                         import Frame
from src.Display.fps_rate                    import FPSRateFrames
from src.Utils.rgb_color                     import RGBColor, YELLOW
from src.GUIItems.label                      import Label
from src.Shapes.rect                         import Rect
  

#=============================================================================
CameraViewRef = ForwardRef( "CameraView" )


#=============================================================================
class CameraView( AVTViewProp ):
    """The class of the cameras views.
    """
    def __init__(self, parent     : AVTWindowRef,
                       camera     : Camera,
                       x          : float,
                       y          : float,
                       width      : float,
                       height     : float,
                       parent_rect: Rect = None ) -> None:
        '''Constructor.
        
        Args:
            parent: AVTWindow
                A reference to the AVT window that  embeds this
                view.
            camera: Camera
                A reference to the associated camera.
            x, y: float
                The coordinates of the top-left corner of  this
                view,  expressed  as percentages of resp. width
                and height of the embedding window. Values must 
                be included in [0.0, 1.0]
            width, height: float
                The size of this view, expressed as percentages 
                of the embedding window sizes.
            parent_rect: Rect
                A reference to  the  rectangle  in  which  this
                proportional  view  takes place into the parent
                window.  If None,  the whole view size is  used
                instead. Defaults to None.

        Raises:
            ValueError:  Some  of  the  coordinates  or  sizes 
                values are outside interval [0.0, 1.0].
        '''
        self.view_name = f"Cam-{camera.get_id()}"
        self.label = Label( self, self.view_name, 20, 40 )
        self.fps_label = Label( self, "", 20, 70, None, Font(14, YELLOW) )
        self.fps_rate = FPSRateFrames( 15 )
        self.joined = False
        
        self.camera = camera
        CameraView._CAM_VIEWS_COUNT += 1

        self.sync_event = Event()
        
        super().__init__( parent, x, y, width, height, parent_rect )

        self.acq_thread  = CameraAcquisition( self.camera )
        self.disp_thread = CameraDirectDisplay( self.acq_thread, self )
        
        self.draw()

    #-------------------------------------------------------------------------
    def draw(self) -> None:
        '''Draws the content of this view.
        '''
        self.draw_fps()
        self.label.draw()
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

        self.content[ 2, 2:-1 ]  = (bg_color / 1.5).color
        self.content[ 2:-2, 2 ]  = (bg_color / 1.5).color
        self.content[ -3, 3:-1 ] = (bg_color * 3).color
        self.content[ 3:-3, -2 ] = (bg_color * 3).color
        self.content[ 3, 3:-2 ]  = (bg_color / 2).color
        self.content[ 4:-3, 3 ]  = (bg_color / 2).color
        self.content[ -4, 4:-2 ] = (bg_color * 1.5).color
        self.content[ 4:-3, -3 ] = (bg_color * 1.5).color

    #-------------------------------------------------------------------------
    def draw_fps(self) -> None:
        '''Draws the frames per second rate.
        '''
        self.fps_rate.new_frame()
        fps_text = self.fps_rate.get_text()
        if fps_text != '':
            self.fps_label.text = f"{fps_text} fps"
            self.fps_label.draw()

    #-------------------------------------------------------------------------
    def draw_frame(self, frame: Frame) -> None:
        '''Draws a new frame within this camera view
        '''
        frame_height, frame_width = frame.shape[:2]
        
        if frame_width != self.width or frame_height != self.height:
            
            ratio_x = self.width / frame_width
            ratio_y = self.height / frame_height
            ratio = min( ratio_x, ratio_y )                    
            frame = cv2.resize( frame, None, fx=ratio, fy=ratio, interpolation=cv2.INTER_LINEAR )
            
            new_height, new_width = frame.shape[:2]
            if new_width > self.width:
                new_width = self.width
            if new_height > self.height:
                new_height = self.height
                
            x = (self.width - new_width) // 2
            y = (self.height - new_height) // 2
            
            self.content = np.zeros( (self.height, self.width, 3), np.uint8 ) + 16
            self.content[ y:y+new_height,
                          x:x+new_width, : ] = frame[ :new_height, :new_width, : ] 
        
        else:
            self.content = frame
        
        self.draw()

    #-------------------------------------------------------------------------
    def is_ok(self) -> bool:
        '''Returns True when status of this camera acquisition thread is ok, or False otherwise.
        '''
        try:
            return self.camera.hw_default_width != 0
        except:
            return False

    #-------------------------------------------------------------------------
    def join(self) -> None:
        '''Joins the internal threads.
        '''
        if not self.joined:
            self.acq_thread.join()
            self.disp_thread.join()
            self.joined = True

    #-------------------------------------------------------------------------
    def start(self) -> None:
        '''Starts every internal thread.
        '''
        self.acq_thread.start()
        self.disp_thread.start()
        self.fps_rate.start()

    #-------------------------------------------------------------------------
    def stop(self) -> None:
        '''Definitively stops every internal threads.
        '''
        self.acq_thread.stop()
        self.disp_thread.stop()
        self.join()
        self.camera.release()

    #-------------------------------------------------------------------------
    _CAM_VIEWS_COUNT = 0

#=====   end of   src.Display.camera_view   =====#
