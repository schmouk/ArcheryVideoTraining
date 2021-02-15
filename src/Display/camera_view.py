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
import time

from threading import Event, Lock, Thread
from typing    import ForwardRef

from src.App.avt_config                      import AVTConfig
from .avt_view_prop                          import AVTViewProp
from .avt_window                             import AVTWindowRef
from src.Cameras.camera                      import Camera
from src.GUIItems.font                       import Font
from src.Buffers.frames_acquisition_buffer   import FramesAcquisitionBuffer
from src.Display.fps_rate                    import FPSRateFrames
from src.Utils.indexed_frame                 import IndexedFrame
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

        self.buffer = FramesAcquisitionBuffer()
        self.sync_event = Event()
        
        super().__init__( parent, x, y, width, height, parent_rect )

        self.acq_thread  = self._Acquisition( f"{self.view_name}-acq-thrd" ,
                                              self.buffer,
                                              self.sync_event,
                                              self.camera,
                                              self.fps_rate )
        self.disp_thread = self._Display    ( f"{self.view_name}-disp-thrd",
                                              self.buffer,
                                              self.sync_event,
                                              self )
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


    #-------------------------------------------------------------------------
    class _Acquisition( Thread ):
        '''The internal class for the acquisition thread.
        '''
        #---------------------------------------------------------------------
        def __init__(self, name         : str,
                           frames_buffer: FramesAcquisitionBuffer,
                           sync_event   : Event,
                           camera       : Camera,
                           fps_rate     : FPSRateFrames) -> None:
            '''Constructor.
            
            Args:
                name: str
                    The name of this thread.
                frames_buffer: FramesAcquisitionBuffer:
                    A reference to the flip-flop frames  buffer
                    used to acquire/display frames.
                sync_event: Event
                    A reference to the  synchronization  signal
                    used to synchronize acquisition and display
                    of frames.
                camera: Camera
                    A reference tot he associated camera.
                fps_rate: FPSRateFrames
                    A reference to the parent_view frames  rate
                    evaluator.
            '''
            super().__init__( name=name )
            self.frames_buffer = frames_buffer
            self.sync_event = sync_event
            self.sync_event.clear()
            self.camera = camera
            self.fps_rate = fps_rate
    
        #---------------------------------------------------------------------
        def run(self) -> None:
            '''The acquisition method once this thread has been started.
            '''
            frame_index = 0
            self.keep_on = True            
            
            while self.keep_on:
                frame = self.camera.read()
                
                if frame is None:
                    time.sleep( 0.020 )
                    
                else:
                    if frame_index == 0:
                        self.fps_rate.start()

                    print( f"{self.name} + {frame_index:6d} / {time.perf_counter():.3f} s")
                        
                    self.frames_buffer.set( IndexedFrame(frame_index, cv2.flip( frame, 1 )) )  # notice: we're mirroring the captured frame
                    self.sync_event.set()
            
                    frame_index += 1
                    
        #---------------------------------------------------------------------
        def stop(self) -> None:
            '''Definitively stops this thread.
            '''
            self.keep_on = False


    #-------------------------------------------------------------------------
    class _Display( Thread ):
        '''The internal class for the display thread.
        '''
        #---------------------------------------------------------------------
        def __init__(self, name         : str,
                           frames_buffer: FramesAcquisitionBuffer,
                           sync_event   : Event                  ,
                           parent_view  : CameraViewRef           ) -> None:
            '''Constructor.
            
            Args:
                name: str
                    The name of this thread.
                frames_buffer: FramesAcquisitionBuffer:
                    A reference to the flip-flop frames  buffer
                    used to acquire/display frames.
                sync_event: Event
                    A reference to the  synchronization  signal
                    used to synchronize acquisition and display
                    of frames.
                parent_view: CameraViewRef
                    A reference to the parent camera view.
            '''
            super().__init__( name=name )
            self.frames_buffer = frames_buffer
            self.sync_event = sync_event
            self.parent_view = parent_view
            self.width  = self.parent_view.width
            self.height = self.parent_view.height
    
        #-------------------------------------------------------------------------
        def run(self) -> None:
            '''The acquisition method once this thread has been started.
            '''
            self.keep_on = self.parent_view.is_ok()
            
            #===================================================================
            # last_frame_index = -1
            #===================================================================
            
            while self.keep_on:
                self.sync_event.wait()
                indexed_frame = self.frames_buffer.get()
                
                if indexed_frame is None:
                    time.sleep( 0.020 )
                    continue
                
                #===============================================================
                # if indexed_frame.index == last_frame_index:
                #     time.sleep( 0.005 )
                #     continue
                # else:
                #     last_frame_index = indexed_frame.index
                #===============================================================

                self.sync_event.clear()
                
                frame = indexed_frame.frame
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
                    
                    self.parent_view.content = np.zeros( (self.height, self.width, 3), np.uint8 ) + 16
                    self.parent_view.content[ y:y+new_height,
                                              x:x+new_width, : ] = frame[ :new_height, :new_width, : ] 
                
                else:
                    self.parent_view.content = frame.copy()
                
                print( f"{self.name} - {indexed_frame.index:6d} / {time.perf_counter():.3f} s")
                self.parent_view.draw()
        
        #---------------------------------------------------------------------
        def stop(self) -> None:
            '''Definitively stops this thread.
            '''
            self.keep_on = False

#=====   end of   src.Display.camera_view   =====#
