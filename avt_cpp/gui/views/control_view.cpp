/*
MIT License

Copyright (c) 2022 Philippe Schmouker, ph.schmouker (at) gmail.com

Permission is hereby granted,  free of charge,  to any person obtaining a copy
of this software and associated documentation files (the "Software"),  to deal
in the Software without restriction,  including without limitation the  rights
to use,  copy,  modify,  merge,  publish,  distribute, sublicense, and/or sell
copies of the Software,  and  to  permit  persons  to  whom  the  Software  is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS",  WITHOUT WARRANTY OF ANY  KIND,  EXPRESS  OR
IMPLIED,  INCLUDING  BUT  NOT  LIMITED  TO  THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT  SHALL  THE
AUTHORS  OR  COPYRIGHT  HOLDERS  BE  LIABLE  FOR  ANY CLAIM,  DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,  ARISING FROM,
OUT  OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

//===========================================================================
module;

#include <atomic>
#include <cstring>
#include <exception>
#include <format>

#include <opencv2/core/cvstd.hpp>
#include <opencv2/highgui.hpp>

#include "gui/views/view.h"


module gui.views.control_view;

import avt.config;
import utils.coords2d;
import gui.items.cursor;
import gui.fonts.font;
import mtmp.mutex;
import utils.rgb_color;
import utils.size;


//===========================================================================
namespace avt::gui::views
{
    /** Value Constructor. */
    ControlView::ControlView(ViewType* p_parent_view) noexcept(false) //, const avt::cameras::CamerasPool& cameras_pool) noexcept(false)
        : ViewType{ p_parent_view,
                    0, p_parent_view->width() - WIDTH, // i.e. position in main window
                    WIDTH, p_parent_view->height() },  // i.e. size of this view in main window
          ThreadType{ "controls-thrd", 1000 }
    {
        m_create_controls();  //(cameras_pool);
    }

    /** @brief Draws this view content within the parent window. */
    void ControlView::draw() noexcept
    {
        m_draw_borders();
        m_draw_controls();
        if (p_parent_view != nullptr)
            ViewType::draw(*p_parent_view);
    }


    /** Internally creates all the controls that are embedded in this Control View. */
    void ControlView::m_create_controls() noexcept  // (const avt::cameras::CamerasPool& cameras_pool) noexcept
    {
        /*** /
        y = 15 + self.ICON_PADDING

        self.cameras_ctrls = [ self._CtrlCamera(camera,
                                                None,
                                                y + self.ICON_HEIGHT*camera.cam_id) for camera in cameras_pool ]
        for cam_id in range( len(cameras_pool), AVTConfig.CAMERAS_MAX_COUNT ):
            self.cameras_ctrls.append( self._CtrlCamera( NullCamera( cam_id ),
                                                         None,
                                                         y + self.ICON_HEIGHT*cam_id ) )

        y += AVTConfig.CAMERAS_MAX_COUNT * self.ICON_HEIGHT + 6
        self.target_ctrl = self._CtrlTarget( 5, y, False, False )

        y += 2 * self.ICON_PADDING + self.ICON_HEIGHT
        self.lines_ctrl = self._CtrlLines( 5, y, False, False )

        y += 2 * self.ICON_PADDING + self.ICON_HEIGHT
        self.delay_ctrl = self._CtrlDelay( 5, y , False, False )

        y += self.ICON_PADDING * 2 + self.ICON_HEIGHT
        self.record_ctrl= self._CtrlRecord( 5, y, False, False )

        y += self.ICON_PADDING + self.ICON_HEIGHT
        self.replay_ctrl = self._CtrlReplay(5, y, False, False )

        y += 2 * self.ICON_PADDING + self.ICON_HEIGHT + 20
        self.overlays_ctrl = self._CtrlOverlays( 5, y, False, False )

        y += (self.overlays_ctrl._SIZE - self.ICON_HEIGHT) + self.ICON_PADDING + self.ICON_HEIGHT
        self.timer_ctrl = self._CtrlTimer( 5, y, False, False )

        y += self.ICON_PADDING + self.ICON_HEIGHT
        self.match_ctrl = self._CtrlMatch( 5, y, False, False )

        y += 2 * self.ICON_PADDING + self.ICON_HEIGHT
        self.time_ctrl = self._CtrlTime( 5, y )

        self.exit_ctrl = self._CtrlExit( self.width, self.height )

        self.controls_list = [ *self.cameras_ctrls,
                                self.target_ctrl  ,
                                self.delay_ctrl   ,
                                self.record_ctrl  ,
                                self.replay_ctrl  ,
                                self.overlays_ctrl,
                                self.lines_ctrl   ,
                                self.timer_ctrl   ,
                                self.match_ctrl   ,
                                self.time_ctrl    ,
                                self.exit_ctrl      ]
        /***/
    }

    /** @brief Draws lines on this view borders. */
    void ControlView::m_draw_borders() noexcept
    {
    /** /
    bg_color = self.bg_color
    self.content = cv2.rectangle( self.content,
                                  (3, 3), (self.width-2, self.height-2),
                                  (bg_color / 2).color,
                                  1, cv2.LINE_4 )
    self.content = cv2.rectangle( self.content,
                                  (4, 4), (self.width-3, self.height-3),
                                  (bg_color / 2).color,
                                  1, cv2.LINE_4 )

    self.content = cv2.rectangle( self.content,
                                  (1, 1), (self.width-4, self.height-4),
                                  (bg_color * 2).color,
                                  1, cv2.LINE_4 )
    self.content = cv2.rectangle( self.content,
                                  (2, 2), (self.width-5, self.height-5),
                                  (bg_color * 2).color,
                                  1, cv2.LINE_4 )
    /**/
    }

    /** @brief Draws all controls in this control view. */
    void ControlView::m_draw_controls() noexcept
    {
    /** /
        try:
            for ctrl in self.controls_list:
                try:
                    ctrl.draw( self )
                except Exception as e:
                    print( 'caught exception', str(e), 'while drawing control', str(ctrl) )
        except:
            pass
    /**/
    }

    /** Draws a control in its embedding content - Controls Base class. */
    void ControlView::_CtrlBase::draw(avt::ImageType& image) noexcept
    {
        avt::gui::fonts::Font font;
        try {
            if (enabled)
                font = active ? _FONT_ACTIVE : _FONT_ENABLED;
            else
                font = _FONT_DISABLED;
            font.draw_text(typeid(*this).name(), image, text_pos, false);
        }
        catch(...) {}
    }

    /** Draws a control in its embedding content - Camera Controls. */
    void ControlView::_CtrlCamera::draw(avt::ImageType& image) noexcept
    {
        try {
            /*** /
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
                                str(self.camera.get_id()),
                                b_shadow=self.camera.is_ok() )
            /***/
        }
        catch (...) {}
    }

    /** Draws a control in its embedding content - Delay Control. */
    void ControlView::_CtrlDelay::draw(avt::ImageType& image) noexcept
    {
        /*** /
        x = (view.WIDTH - self._SIZE) // 2
        y = self.y + 1
        if self.enabled:
            img = self._ICON_ON if self.is_active else self._ICON_OFF
        else:
            img = self._ICON_DISABLED
        view.content[ y:y+self._SIZE, x:x+self._SIZE, : ] = img[ :, :, : ]

        ##font.draw_text( view, Point(self.x + 5, self.y + self._FONT_SIZE), 'Delay' )
        self.slider.draw( view )
        /***/
    }

    /** Creates the associated slider - Delay Control. */
    void ControlView::_CtrlDelay::m_create_slider(const avt::CoordsType x, const avt::CoordsType y) noexcept
    {
        /** /
        self.slider = IntSlider( x = (x if x else pos.x) + 5,
                                 y = (y if y is not None else pos.y) + self._SIZE + 8,
                                 width = ControlView.WIDTH - 12*2,
                                 height = 5,
                                 min_value = 5,
                                 max_value = 12,
                                 current_value = 7,
                                 bar_color = GRAY,
                                 cursor_color = self._TICKS_FONT_ENABLED.color,
                                 text_font = self._TICKS_FONT_ENABLED,
                                 shadow_height = 0,
                                 visible = True,
                                 enabled = enabled,
                                 active = active   )
        /**/
    }





    /** /
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
            if self.enabled:
                color = YELLOW if self.is_active else GRAY
            else:
                color = ANTHRACITE // 2

            x = self.x + 13
            y = self.y + view.ICON_HEIGHT // 2
            cv2.line( view.content,
                      (x+1, y+1),
                      (x+1 + self._LINE_LENGTH, y+1),
                      (color//2).color, self._LINE_THICKNESS, cv2.LINE_AA )
            cv2.line( view.content,
                      (x, y),
                      (x + self._LINE_LENGTH, y),
                      color.color, self._LINE_THICKNESS, cv2.LINE_AA )

            x = (view.WIDTH + x + self._LINE_LENGTH) // 2
            y = self.y + (view.ICON_HEIGHT - self._LINE_LENGTH) // 2
            cv2.line( view.content,
                      (x+1, y+1),
                      (x+1, y+1 + self._LINE_LENGTH),
                      (color//2).color, self._LINE_THICKNESS, cv2.LINE_AA )
            cv2.line( view.content,
                      (x, y),
                      (x, y + self._LINE_LENGTH),
                      color.color, self._LINE_THICKNESS, cv2.LINE_AA )

        #---------------------------------------------------------------------
        _LINE_LENGTH = 35
        _LINE_THICKNESS = 7

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
            x = (view.WIDTH - self._SIZE) // 2
            y = self.y + 1
            if self.enabled:
                img = self._ICON_ON if self.is_active else self._ICON_OFF
            else:
                img = self._ICON_DISABLED
            view.content[ y:y+self._SIZE, x:x+self._SIZE, : ] = img[ :, :, : ]

        #---------------------------------------------------------------------
        _ICON_DISABLED = cv2.imread( '../picts/controls/match-disabled.png' )
        _ICON_OFF      = cv2.imread( '../picts/controls/match-off.png' )
        _ICON_ON       = cv2.imread( '../picts/controls/match-on.png' )
        _SIZE = _ICON_ON.shape[ 0 ]


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
            x = (view.WIDTH - self._SIZE) // 2
            y = self.y + 5
            if self.enabled:
                img = self._ICON_ON if self.is_active else self._ICON_OFF
            else:
                img = self._ICON_DISABLED
            view.content[ y:y+self._SIZE, x:x+self._SIZE, : ] = img[ :, :, : ]

        #---------------------------------------------------------------------
        _ICON_DISABLED = cv2.imread( '../picts/controls/overlays-disabled.png' )
        _ICON_OFF      = cv2.imread( '../picts/controls/overlays-off.png' )
        _ICON_ON       = cv2.imread( '../picts/controls/overlays-on.png' )
        _SIZE = _ICON_ON.shape[ 0 ]


    #-------------------------------------------------------------------------
    class _CtrlRecord( _CtrlBase ):
        '''The video recording control.
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
            self.slider = FloatSlider( x = (x if x else pos.x) + 5,
                                       y = (y if y is not None else pos.y) + self._ICON_SIZE + 8,
                                       width = ControlView.WIDTH - 12*2,
                                       height = 5,
                                       min_value = 20,
                                       max_value = 130,
                                       current_value = 60,
                                       bar_color = GRAY,
                                       cursor_color = self._TICKS_FONT_ENABLED.color,
                                       text_font = self._TICKS_FONT_ENABLED,
                                       shadow_height = 0,
                                       visible = True,
                                       enabled = enabled,
                                       active = active,
                                       show_cursor_text = False )

        #---------------------------------------------------------------------
        def draw(self, view: View) -> None:
            '''Draws a control in its embedding content.
            Args:
                view: View
                    A reference to the embedding view.
            '''
            cursor_text = str( self.slider.value )

            if self.enabled:
                if self.is_active:
                    img = self._ICON_ON
                    font = self._FONT_2_ON if len(cursor_text) < 3 else self._FONT_3_ON
                else:
                    img = self._ICON_OFF
                    font = self._FONT_2_OFF if len(cursor_text) < 3 else self._FONT_3_OFF
            else:
                img = self._ICON_DISABLED
                font = self._FONT_2_DISABLED if len(cursor_text) < 3 else self._FONT_3_DISABLED

            x = (view.WIDTH - self._ICON_SIZE) // 2
            y = self.y + 1

            view.content[ y:y+self._ICON_SIZE, x:x+self._ICON_SIZE, : ] = img[ :, :, : ]

            cursor_text_width = font.get_text_width( cursor_text )
            x = (view.WIDTH - cursor_text_width ) // 2 + 1
            y = self.y + (self._ICON_SIZE + self._FONT_SIZE) // 2 - (2 if len(cursor_text) < 3 else 4)
            font.draw_text( view, Point(x,y), cursor_text, True )

            ##font.draw_text( view, Point(self.x + 5, self.y + self._FONT_SIZE), 'Delay' )
            self.slider.draw( view )

        #---------------------------------------------------------------------
        _FONT_3_SIZE        = 8
        _FONT_2_SIZE        = 11
        _FONT_3_DISABLED    = Font( _FONT_3_SIZE, GRAY )
        _FONT_3_OFF         = Font( _FONT_3_SIZE, LIGHT_GRAY )
        _FONT_3_ON          = Font( _FONT_3_SIZE, YELLOW )
        _FONT_2_DISABLED    = Font( _FONT_2_SIZE, GRAY )
        _FONT_2_OFF         = Font( _FONT_2_SIZE, LIGHT_GRAY )
        _FONT_2_ON          = Font( _FONT_2_SIZE, YELLOW )
        _ICON_DISABLED      = cv2.imread( '../picts/controls/record-disabled.png' )
        _ICON_OFF           = cv2.imread( '../picts/controls/record-off.png' )
        _ICON_ON            = cv2.imread( '../picts/controls/record-on.png' )
        _ICON_SIZE          = _ICON_ON.shape[ 0 ]
        _TICKS_FONT_SIZE    = 8
        _TICKS_FONT_ENABLED = Font( _TICKS_FONT_SIZE, YELLOW // 1.33 )


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
            if self.enabled:
                if self.is_active:
                    icons = (self._ICON_STEP_BW_ON,
                             self._ICON_STEP_FW_ON,
                             self._ICON_PLAY_ON,
                             self._ICON_FBW_ON,
                             self._ICON_FFW_ON)
                else:
                    icons = (self._ICON_STEP_BW_OFF,
                             self._ICON_STEP_FW_OFF,
                             self._ICON_PLAY_OFF,
                             self._ICON_FBW_OFF,
                             self._ICON_FFW_OFF)
            else:
                icons = (self._ICON_STEP_BW_DISABLED,
                         self._ICON_STEP_FW_DISABLED,
                         self._ICON_PLAY_DISABLED,
                         self._ICON_FBW_DISABLED,
                         self._ICON_FFW_DISABLED)

            x0 = self.x + 5
            y0 = self.y + 23
            x1 = x0 + self._SIZE
            x2 = x1 + self._SIZE
            y1 = y0 + self._SIZE // 2 + 2
            y2 = y0 + self._SIZE + 3

            view.content[ y0:y0+self._SIZE,
                          x0:x0+self._SIZE, : ] = icons[0][:,:,:]

            view.content[ y0:y0+self._SIZE,
                          x2:x2+self._SIZE, : ] = icons[1][:,:,:]

            view.content[ y1:y1+self._SIZE,
                          x1:x1+self._SIZE, : ] = icons[2][:,:,:]

            view.content[ y2:y2+self._SIZE,
                          x0:x0+self._SIZE, : ] = icons[3][:,:,:]

            view.content[ y2:y2+self._SIZE,
                          x2:x2+self._SIZE, : ] = icons[4][:,:,:]

        #---------------------------------------------------------------------
        _ICON_FBW_DISABLED     = cv2.imread( '../picts/controls/fbw-25-disabled.png' )
        _ICON_FBW_OFF          = cv2.imread( '../picts/controls/fbw-25-off.png' )
        _ICON_FBW_ON           = cv2.imread( '../picts/controls/fbw-25-on.png' )
        _ICON_FFW_DISABLED     = cv2.imread( '../picts/controls/ffw-25-disabled.png' )
        _ICON_FFW_OFF          = cv2.imread( '../picts/controls/ffw-25-off.png' )
        _ICON_FFW_ON           = cv2.imread( '../picts/controls/ffw-25-on.png' )
        _ICON_PAUSE_DISABLED   = cv2.imread( '../picts/controls/pause-25-disabled.png' )
        _ICON_PAUSE_OFF        = cv2.imread( '../picts/controls/pause-25-off.png' )
        _ICON_PAUSE_ON         = cv2.imread( '../picts/controls/pause-25-on.png' )
        _ICON_PLAY_DISABLED    = cv2.imread( '../picts/controls/play-25-disabled.png' )
        _ICON_PLAY_OFF         = cv2.imread( '../picts/controls/play-25-off.png' )
        _ICON_PLAY_ON          = cv2.imread( '../picts/controls/play-25-on.png' )
        _ICON_STEP_BW_DISABLED = cv2.imread( '../picts/controls/step-bw-25-disabled.png' )
        _ICON_STEP_BW_OFF      = cv2.imread( '../picts/controls/step-bw-25-off.png' )
        _ICON_STEP_BW_ON       = cv2.imread( '../picts/controls/step-bw-25-on.png' )
        _ICON_STEP_FW_DISABLED = cv2.imread( '../picts/controls/step-fw-25-disabled.png' )
        _ICON_STEP_FW_OFF      = cv2.imread( '../picts/controls/step-fw-25-off.png' )
        _ICON_STEP_FW_ON       = cv2.imread( '../picts/controls/step-fw-25-on.png' )
        _SIZE = 25 ##_ICON_PLAY_ON.shape[0]


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
        _ICON_ACTIVE = cv2.imread( '../picts/controls/target-on.png' )
        _ICON_INACTIVE = cv2.imread( '../picts/controls/target-off.png' )
        _ICON_DISABLED = cv2.imread( '../picts/controls/target-disabled.png' )
        _SIZE          = _ICON_ACTIVE.shape[0]


    #-------------------------------------------------------------------------
    class _CtrlTime( _CtrlBase ):
        '''The time and session duration control.
        '''
        #---------------------------------------------------------------------
        def __init__(self, x:int, y: int) -> None:
            '''Constructor.

            Args:
                x, y: top-left position of this control.
            '''
            super().__init__( x, y, False, True )
            self.time_label = Label( x=x,
                                     y=y+self._TIME_TEXT_SIZE,
                                     text_font=Font(self._TIME_TEXT_SIZE, YELLOW-32, bold=True) )
            self.duration_label = Label( x=x,
                                         y=y+self._FULL_HEIGHT,
                                         text_font=Font(self._DURATION_TEXT_SIZE, YELLOW) )

        #---------------------------------------------------------------------
        def draw(self, view: View) -> None:
            '''Draws a control in its embedding content.
            Args:
                view: View
                    A reference to the embedding view.
            '''
            date = time.localtime()
            self.time_label.text = f"{date.tm_hour:02d}:{date.tm_min:02d}"
            time_label_width = self.time_label.get_text_width()

            duration = time.perf_counter()
            hr = int( duration // 3600 )
            mn = int( (duration - 3600 * hr) // 60 )
            sc = int( duration % 60 )
            self.duration_label.text = f"({hr:d}:{mn:02d}:{sc:02d})"
            duration_label_width = self.duration_label.get_text_width()

            cv2.rectangle( view.content,
                           (self.x, self.y-3),
                           (view.width-self.x-2, self.y+self._FULL_HEIGHT + 3),
                           AVTConfig.DEFAULT_BACKGROUND.color,
                           -1 )

            self.time_label.draw_at( (view.width - time_label_width) // 2,
                                     self.time_label.pos.y,
                                     view )
            self.duration_label.draw_at( (view.width - duration_label_width) // 2,
                                          self.duration_label.pos.y,
                                          view )

        #---------------------------------------------------------------------
        _DURATION_TEXT_SIZE = 11
        _PADDING            = 11
        _TIME_TEXT_SIZE     = 15
        _FULL_HEIGHT        = _TIME_TEXT_SIZE + _PADDING + _DURATION_TEXT_SIZE


    #-------------------------------------------------------------------------
    class _CtrlTimer( _CtrlBase ):
        '''The timer control.
        '''
        #---------------------------------------------------------------------
        def draw(self, view: View) -> None:
            '''Draws a control in its embedding content.
            Args:
                view: View
                    A reference to the embedding view.
            '''
            x = (view.WIDTH - self._SIZE) // 2
            y = self.y + 1
            if self.enabled:
                img = self._ICON_ON if self.is_active else self._ICON_OFF
            else:
                img = self._ICON_DISABLED
            view.content[ y:y+self._SIZE, x:x+self._SIZE, : ] = img[ :, :, : ]

        #---------------------------------------------------------------------
        _ICON_DISABLED = cv2.imread( '../picts/controls/timer-disabled.png' )
        _ICON_OFF      = cv2.imread( '../picts/controls/timer-off.png' )
        _ICON_ON       = cv2.imread( '../picts/controls/timer-on.png' )
        _SIZE          = _ICON_ON.shape[ 0 ]
    /**/

}