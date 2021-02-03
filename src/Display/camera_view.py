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
from threading import Thread
import time

from src.App.avt_config                      import AVTConfig
from .avt_view_prop                          import AVTViewProp
from .avt_window                             import AVTWindowRef
from src.Cameras.camera                      import Camera
from src.GUIItems.font                       import Font
from src.Display.fps_rate                    import FPSRateFrames
from src.Buffers.frames_acquisition_buffer   import FramesAcquisitionBuffer
from src.Utils.indexed_frame                 import IndexedFrame
from src.GUIItems.label                      import Label
from src.Shapes.rect                         import Rect
from .rgb_color                              import RGBColor, YELLOW
  

#=============================================================================
class CameraView( Thread, AVTViewProp ):
    """The class description.
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
        self.label = Label( self, f"Cam-{camera.cam_id}", 20, 40 )
        self.fps_label = Label( self, "", 20, 70, None, Font(14, YELLOW) )
        self.fps_rate = FPSRateFrames( 15 )
        ##self.buffer = FramesAcquisitionBuffer()
        
        self.camera = camera
        name = f"camera-thrd-{CameraView._CAM_VIEWS_COUNT}"
        CameraView._CAM_VIEWS_COUNT += 1

        Thread.__init__( self, name=name )
        AVTViewProp.__init__( self, parent, x, y, width, height, parent_rect )
        ##ViewProp.__init__( self, parent, x, y, width, height, RGBColor(16,16,16), parent_rect )
        
        ##self.camera.set_hw_dims( width, height )

    #-------------------------------------------------------------------------
    def draw(self) -> None:
        '''Draws the content of this view.
        '''
        ##self.content = self.buffer.get_frame().copy()
        
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
        self.content[ 2:-1, 2 ]  = (bg_color / 1.5).color
        self.content[ -2, 3:-1 ] = (bg_color * 1.5).color
        self.content[ 3:-1, -2 ] = (bg_color * 1.5).color
        self.content[ 3, 3:-2 ]  = (bg_color / 2).color
        self.content[ 4:-2, 3 ]  = (bg_color / 2).color
        self.content[ -3, 4:-2 ] = (bg_color * 3).color
        self.content[ 4:-3, -3 ] = (bg_color * 3).color

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
    def is_ok(self) -> bool:
        '''Returns True when status of this camera acquisition thread is ok, or False otherwise.
        '''
        try:
            return self.camera.hw_default_width != 0
        except:
            return False

    #-------------------------------------------------------------------------
    def run(self) -> None:
        '''The acquisition method once this thread has been started.
        '''
        frame_index = 0
        self.keep_on = self.is_ok()
        
        self.start_time = time.perf_counter()
        self.fps_rate.start()
        
        while self.keep_on:
            frame = self.camera.read()
            if frame is None:
                time.sleep( 0.020 )
            else:
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
                    
                    view_content = np.zeros( (self.height, self.width, 3) ) + 16
                    view_content[ y:y+new_height,
                                  x:x+new_width, : ] = frame[ :new_height, :new_width, : ] 
                
                else:
                    view_content = frame
                
                self.content = cv2.flip( view_content, 1 )
                
                self.draw()
                ##self.buffer.set( IndexedFrame(frame_index, frame) )
                
            frame_index += 1
        
        self.camera.release()

    #-------------------------------------------------------------------------
    def stop(self) -> None:
        '''Definitively stops this acquisition thread.
        '''
        self.keep_on = False

    #-------------------------------------------------------------------------
    _CAM_VIEWS_COUNT = 0

#=====   end of   src.Display.camera_view   =====#
