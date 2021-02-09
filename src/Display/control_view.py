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
import time
from threading import Thread

from src.Utils.rgb_color             import ANTHRACITE, DEEP_GRAY, GRAY, GrayColor, LIGHT_GRAY, RED, YELLOW
from src.App.avt_config              import AVTConfig
from .avt_view                       import AVTView
from .view                           import AVTWindowRef, View
from src.GUIItems.font               import BoldFont, Font
from src.Cameras.camera              import Camera, NullCamera
from src.Cameras.cameras_pool        import CamerasPool
from src.Shapes.point                import Point
from src.GUIItems.Controls.sliders   import IntSlider


#=============================================================================
class ControlView( Thread, AVTView ):
    """The class description.
    """
    def __init__(self, parent      : AVTWindowRef,
                       cameras_pool: CamerasPool  ) -> None:
        '''Constructor.
        
        Args:
            parent: AVTWindow
                A reference to the AVT window that embeds this
                view.
            cameras_pool: CamerasPool
                A reference to the pool of cameras  that  have
                been detected as connected to the AVT app.

        Raises:
            ValueError:  Some  of  the  coordinates  or  sizes 
                values are outside interval [0.0, 1.0].
        '''
        Thread.__init__( self, name='controls-thrd' )
        AVTView.__init__( self,
                            parent,
                            parent.width - self.WIDTH, 0,
                            self.WIDTH, parent.height     )
        self.create_controls( cameras_pool )
        
    #-------------------------------------------------------------------------
    def create_controls(self, cameras_pool: CamerasPool) -> None:
        '''Creates all controls in this control view.
        
        Args:
            cameras_pool: CamerasPool
                A reference to the pool of cameras that have
                been detected as connected to the AVT app.
        '''
        y = 15 + self.ICON_PADDING

        self.cameras_ctrls = [ self._CtrlCamera(camera,
                                                None,
                                                y + self.ICON_HEIGHT*camera.cam_id) for camera in cameras_pool ]
        for cam_id in range( len(cameras_pool), AVTConfig.CAMERAS_MAX_COUNT ):
            self.cameras_ctrls.append( self._CtrlCamera( NullCamera( cam_id ),
                                                         None,
                                                         y + self.ICON_HEIGHT*cam_id ) )
        
        y += AVTConfig.CAMERAS_MAX_COUNT * self.ICON_HEIGHT
        self.target_ctrl   = self._CtrlTarget(   5, y + 0 * self.ICON_HEIGHT, True, False )
        
        y += self.ICON_PADDING
        self.delay_ctrl    = self._CtrlDelay(    5, y + 1 * self.ICON_HEIGHT, True, False )
        
        y += self.ICON_PADDING
        self.record_ctrl   = self._CtrlRecord(   5, y + 2 * self.ICON_HEIGHT, False, False )
        self.replay_ctrl   = self._CtrlReplay(   5, y + 3 * self.ICON_HEIGHT, False, False )
        self.overlays_ctrl = self._CtrlOverlays( 5, y + 4 * self.ICON_HEIGHT, False, False )
        
        y += self.ICON_PADDING
        self.lines_ctrl    = self._CtrlLines(    5, y + 5 * self.ICON_HEIGHT, False, False )

        y += self.ICON_PADDING
        self.match_ctrl    = self._CtrlMatch(    5, y + 6 * self.ICON_HEIGHT, False, False )

        self.exit_ctrl     = self._CtrlExit( self.width, self.height )
        
        self.controls_list = [ *self.cameras_ctrls,
                                self.target_ctrl  ,
                                self.delay_ctrl   ,
                                self.record_ctrl  ,
                                self.replay_ctrl  ,
                                self.overlays_ctrl,
                                self.lines_ctrl   ,
                                self.match_ctrl   ,
                                self.exit_ctrl      ]
        
    #-------------------------------------------------------------------------
    def draw(self) -> None:
        '''Draws this view content within the parent window.
        '''
        self.draw_borders()
        self.draw_controls()
        super().draw()

    #-------------------------------------------------------------------------
    def draw_borders(self) -> None:
        '''Draws lines on this view borders.
        '''
        bg_color = self.bg_color
        self.content = cv2.rectangle( self.content,
                                      (4, 4), (self.width-2, self.height-2),
                                      (bg_color / 2).color,
                                      1, cv2.LINE_4 )
        self.content = cv2.rectangle( self.content,
                                      (5, 5), (self.width-3, self.height-3),
                                      (bg_color / 2).color,
                                      1, cv2.LINE_4 )

        self.content = cv2.rectangle( self.content,
                                      (2, 2), (self.width-4, self.height-4),
                                      (bg_color * 2).color,
                                      1, cv2.LINE_4 )
        self.content = cv2.rectangle( self.content,
                                      (3, 3), (self.width-5, self.height-5),
                                      (bg_color * 2).color,
                                      1, cv2.LINE_4 )

    #-------------------------------------------------------------------------
    def draw_controls(self) -> None:
        '''Draws all controls in this control view.
        '''
        try:
            for ctrl in self.controls_list:
                try:
                    ctrl.draw( self )
                except Exception as e:
                    print( 'caught exception', str(e), 'while drawing control', str(ctrl) )
        except:
            pass
        
    #-------------------------------------------------------------------------
    def run(self) -> None:
        '''The core loop of this view thread.
        '''
        self.keep_on = True
        self.draw()
        while self.keep_on:
            time.sleep( 0.200 )

    #-------------------------------------------------------------------------
    def stop(self) -> None:
        '''Definitively stops this view thread.
        '''
        self.keep_on = False

    #-------------------------------------------------------------------------
    # Class data
    WIDTH = 96
    ICON_HEIGHT = 40
    ICON_PADDING = ICON_HEIGHT // 2
        

    #-------------------------------------------------------------------------
    class _CtrlBase:
        '''The base class for all internal controls.
        '''
        ...
        #---------------------------------------------------------------------
        def __init__(self, x: int = None,
                           y: int = None,
                           enabled: bool = True,
                           active : bool = False,
                           *,
                           pos: Point = None) -> None:
            '''Constructor
            
            Args:
                x, y: int
                    The top-left position of  this  control  in
                    the  ControlView.  Ignored if 'pos' is set.
                    Must be set if 'pos' is  None.  Defaults to
                    None (i.e. 'pos' should be set instead).
                enabled: bool
                    Set this to True to get this control enabl-
                    ed  or set it to False otherwise.  Defaults
                    to False.
                active: bool
                    Set this to True to get this control active
                    or  set  it  to  False  to get it inactive.
                    Defaults to False.
                pos: Point
                    The top-left position of  this  control  in
                    the ControlView.  Takes precedence over 'x'
                    and 'y' if set. This argument must be named
                    if set. Defaults to None.
                    
            Raises:
                AssertionError:  x, y and pos are all None, or
                    pos is None and either x or y is None also.
            '''
            assert pos is not None or (x is not None and y is not None)
            if pos is None:
                self.x, self.y = x, y
            else:
                self.x, self.y = pos.x, pos.y
            
            self.text_pos = Point( self.x, self.y + (ControlView.ICON_HEIGHT + self._FONT_SIZE) // 2 )
            
            self.enabled = enabled
            self.is_active = active
        #---------------------------------------------------------------------
        def draw(self, view: View) -> None:
            '''Draws a control in its embedding content.
            
            This method SHOULD BE overwritten in inheriting 
            classes.  The  drawings take place in attribute
            'self.content'.

            Args:
                view: View
                    A reference to the embedding view.
            '''
            # default behavior: put end of class name in view
            try:
                if self.enabled:
                    font = self._FONT_ACTIVE if self.is_active else self._FONT_ENABLED
                else:
                    font = self._FONT_DISABLED
                font.draw_text( view, self.text_pos, self.__class__.__name__[5:] )
            except:
                raise
        #---------------------------------------------------------------------
        _FONT_SIZE     = 14
        _FONT_ACTIVE   = Font( _FONT_SIZE, YELLOW )
        _FONT_DISABLED = Font( _FONT_SIZE, DEEP_GRAY )
        _FONT_ENABLED  = Font( _FONT_SIZE, LIGHT_GRAY )


    #-------------------------------------------------------------------------
    class _CtrlCamera( _CtrlBase ):
        '''The cameras controls.
        '''
        #---------------------------------------------------------------------
        def __init__(self, camera: Camera    ,
                           x     : int = None,
                           y     : int = None,
                           *,
                           pos   : Point = None) -> None:
            '''Constructor
            
            Args:
                camera: Camera
                    A reference to the asdsociated camera.
                x: int
                    The left position of  this  control  in the 
                    ControlView.  Ignored  if 'pos' is set.  If
                    None, the control is horizontally centered
                    in the ControlView. Defaults to None.
                y: int
                    The top position of  this  control  in  the 
                    ControlView.  Ignored if 'pos' is set. Must 
                    be set if 'pos' is  None.  Defaults to None 
                    (i.e. 'pos' should be set instead).
                pos: Point
                    The top-left position of  this  control  in
                    the ControlView.  Takes precedence over 'x'
                    and 'y' if set. This argument must be named
                    if set. Defaults to None.

            Raises:
                AssertionError:  x, y and pos are all None,  or
                    pos is None and y is None also.
            '''
            self.camera = camera
            self.is_on = camera.is_ok()
            if pos is None:
                super().__init__( x or (ControlView.WIDTH - self._WIDTH) // 2, y )
            else:
                super().__init__( pos=pos )
        #---------------------------------------------------------------------
        def draw(self, view: View) -> None:
            '''Draws a control in its embedding content.

            Args:
                view: View
                    A reference to the embedding view.
            '''
            try:
                if self.camera.is_ok():
                    if self.is_on:
                        img = self._ICON_ON
                        font = self._FONT_ON
                        x_id = ControlView.WIDTH // 2 - 5
                    else:
                        img = self._ICON_OFF
                        font = self._FONT_OFF
                        x_id = ControlView.WIDTH // 2 - 9
                else:
                    img = self._ICON_DISABLED
                    font = self._FONT_NOT_OK
                    x_id = ControlView.WIDTH // 2 - 9
                
                view.content[ self.y:self.y+self._HEIGHT,
                              self.x:self.x+self._WIDTH , : ] = img[ :, :, : ]
                font.draw_text( view,
                                Point(x_id, self.y + self._HEIGHT//2 + font.size//2),
                                str(self.camera.cam_id),
                                b_shadow=self.camera.is_ok() )
            except Exception as e:
                print( 'caught exception', str(e) )
        #---------------------------------------------------------------------
        def switch(self) -> bool:
            '''Changes the status of this camera switch.
            '''
            self.is_on = not self.is_on
        #---------------------------------------------------------------------
        _FONT_NOT_OK   = BoldFont( 14, ANTHRACITE )
        _FONT_OFF      = BoldFont( 14, GRAY )
        _FONT_ON       = BoldFont( 14, YELLOW )
        _ICON_OFF      = cv2.imread( '../picts/controls/switch-off.png' )
        _ICON_ON       = cv2.imread( '../picts/controls/switch-on.png' )
        _ICON_DISABLED = cv2.imread( '../picts/controls/switch-disabled.png' )
        _HEIGHT, _WIDTH = _ICON_ON.shape[ :2 ]


    #-------------------------------------------------------------------------
    class _CtrlDelay( _CtrlBase ):
        '''The delay control.
        '''
        #---------------------------------------------------------------------
        def __init__(self, x: int = None,
                           y: int = None,
                           enabled: bool = True,
                           active : bool = False,
                           *,
                           pos: Point = None) -> None:
            '''Constructor
            
            Args:
                x, y: int
                    The top-left position of  this  control  in
                    the  ControlView.  Ignored if 'pos' is set.
                    Must be set if 'pos' is  None.  Defaults to
                    None (i.e. 'pos' should be set instead).
                enabled: bool
                    Set this to True to get this control enabl-
                    ed  or set it to False otherwise.  Defaults
                    to False.
                active: bool
                    Set this to True to get this control active
                    or  set  it  to  False  to get it inactive.
                    Defaults to False.
                pos: Point
                    The top-left position of  this  control  in
                    the ControlView.  Takes precedence over 'x'
                    and 'y' if set. This argument must be named
                    if set. Defaults to None.
                    
            Raises:
                AssertionError:  x, y and pos are all None, or
                    pos is None and either x or y is None also.
            '''
            super().__init__( x, y, enabled, active, pos=pos )
            self.slider = IntSlider( x = (x if x else pos.x) + 5,
                                     y = (y if y is not None else pos.y) + self._FONT_SIZE + 10,
                                     width = ControlView.WIDTH - 10*2,
                                     height = 5,
                                     min_value = 5,
                                     max_value = 12,
                                     current_value = 7,
                                     bar_color = GRAY,
                                     cursor_color = GrayColor( 211 ),
                                     text_font = self._TICKS_FONT_ENABLED,
                                     shadow_height = 0,
                                     visible = True,
                                     enabled = True,
                                     active = False   )
            
        #---------------------------------------------------------------------
        def draw(self, view: View) -> None:
            '''Draws a control in its embedding content.

            Args:
                view: View
                    A reference to the embedding view.
            '''
            if self.enabled:
                font = self._FONT_ACTIVE if self.is_active else self._FONT_ENABLED
                ##icon_img = self._ICON_ON if self.is_active else self._ICON_OFF
            else:
                font = self._FONT_DISABLED
                ##icon_img = self._ICON_DISABLED
                
            font.draw_text( view, Point(self.x + 5, self.y + self._FONT_SIZE), 'Delay' )
            self.slider.draw( view )

        #---------------------------------------------------------------------
        _TICKS_FONT_SIZE = 8
        _TICKS_FONT_ENABLED = Font( _TICKS_FONT_SIZE, LIGHT_GRAY )


    #-------------------------------------------------------------------------
    class _CtrlExit( _CtrlBase ):
        '''The exit button control.
        '''
        #---------------------------------------------------------------------
        def __init__(self, view_width: int, view_height: int) -> None:
            '''Constructor
            
            Args:
                view_width: int
                    The width of the embedding view. Ignored if 
                    'pos' is set.  Must be set if 'pos' is None. 
                    Defaults to None (i.e. 'pos' should be  set 
                    instead).
                view_height: int
                    The height of the embedding  view.  Ignored 
                    if  'pos'  is set.  Must be set if 'pos' is 
                    None.  Defaults to None (i.e. 'pos'  should 
                    be set instead).
            '''
            self.height, self.width = self._ICON_EXIT.shape[:2]
            super().__init__( (view_width - self.width) // 2,
                              view_height - self.height - 12  )
        #---------------------------------------------------------------------
        def draw(self, view: View) -> None:
            '''Draws a control in its embedding content.

            Args:
                view: View
                    A reference to the embedding view.
            '''
            try:
                view.content[ self.y:self.y+self.height,
                              self.x:self.x+self.width , : ] = self._ICON_EXIT[ :, :, : ]
            except:
                pass
        #---------------------------------------------------------------------
        _ICON_EXIT = cv2.imread( '../picts/controls/exit-48.png' )


    #-------------------------------------------------------------------------
    class _CtrlLines( _CtrlBase ):
        '''The lines control.
        '''
        #---------------------------------------------------------------------
        def draw(self, view: View) -> None:
            '''Draws a control in its embedding content.

            Args:
                view: View
                    A reference to the embedding view.
            '''
            super().draw( view )
        

    #-------------------------------------------------------------------------
    class _CtrlMatch( _CtrlBase ):
        '''The match simulation control.
        '''
        #---------------------------------------------------------------------
        def draw(self, view: View) -> None:
            '''Draws a control in its embedding content.

            Args:
                view: View
                    A reference to the embedding view.
            '''
            super().draw( view )
    

    #-------------------------------------------------------------------------
    class _CtrlOverlays( _CtrlBase ):
        '''The video overlays control.
        '''
        #---------------------------------------------------------------------
        def draw(self, view: View) -> None:
            '''Draws a control in its embedding content.

            Args:
                view: View
                    A reference to the embedding view.
            '''
            super().draw( view )


    #-------------------------------------------------------------------------
    class _CtrlRecord( _CtrlBase ):
        '''The video recording control.
        '''
        #---------------------------------------------------------------------
        def draw(self, view: View) -> None:
            '''Draws a control in its embedding content.

            Args:
                view: View
                    A reference to the embedding view.
            '''
            super().draw( view )


    #-------------------------------------------------------------------------
    class _CtrlReplay( _CtrlBase ):
        '''The replay control.
        '''
        #---------------------------------------------------------------------
        def draw(self, view: View) -> None:
            '''Draws a control in its embedding content.

            Args:
                view: View
                    A reference to the embedding view.
            '''
            super().draw( view )
        

    #-------------------------------------------------------------------------
    class _CtrlTarget( _CtrlBase ):
        '''The target control.
        '''
        #---------------------------------------------------------------------
        def draw(self, view: View) -> None:
            '''Draws a control in its embedding content.

            Args:
                view: View
                    A reference to the embedding view.
            '''
            x = (ControlView.WIDTH  - self._SIZE) // 2
            y = self.y + (ControlView.ICON_HEIGHT - self._SIZE) // 2
            if self.enabled:
                img = self._ICON_ACTIVE if self.is_active else self._ICON_INACTIVE
            else:
                img = self._ICON_DISABLED
            view.content[ y:y+self._SIZE, x:x+self._SIZE, : ] = img[ :, :, : ]
            
        #---------------------------------------------------------------------
        _SIZE = 31
        _ICON_ACTIVE = cv2.imread( '../picts/controls/target-active.png' )
        _ICON_INACTIVE = cv2.imread( '../picts/controls/target-inactive.png' )
        _ICON_DISABLED = cv2.imread( '../picts/controls/target-disabled.png' )


#=====   end of   src.Display.cantrol_view   =====#
