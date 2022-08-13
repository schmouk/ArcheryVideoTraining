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

    /** Value Constructor - Exit Control. */
    ControlView::_CtrlExit::_CtrlExit(const int view_width, const int view_height) noexcept
        : _CtrlBase{}
    {
        /*** /
            self.height, self.width = self._ICON_EXIT.shape[:2]
        /***/
        //set((view_width - width) / 2, view_height - height - 12);
    }

    /** Draws a control in its embedding content - Exit Control. */
    void ControlView::_CtrlExit::draw(avt::ImageType& image) noexcept
    {
        /*** /
        try:
            view.content[ self.y:self.y+self.height,
                            self.x:self.x+self.width , : ] = self._ICON_EXIT[ :, :, : ]
        except:
            pass
        /***/
    }

    /** Draws a control in its embedding content - Lines Control. */
    void ControlView::_CtrlLines::draw(avt::ImageType& image) noexcept
    {
        const avt::utils::Coords2D diagonal_offset{ 1, 1 };
        avt::utils::Coords2D       start_pt;
        avt::utils::Coords2D       end_pt;
        int                        x_;
        int                        y_;

        RGBColor color;
        if (enabled)
            color = active ? RGBColor::YELLOW : RGBColor::GRAY;
        else
            color = RGBColor::ANTHRACITE / 2;

        x_       = x + 13;
        y_       = y + ControlView::ICON_HEIGHT / 2;
        start_pt = { x_, y_ };
        end_pt   = { x_ + _LINE_LENGTH, y_ };
        cv::line(image, start_pt + diagonal_offset, end_pt + diagonal_offset, color / 2, _LINE_THICKNESS, cv::LINE_AA);
        cv::line(image, start_pt, end_pt, color, _LINE_THICKNESS, cv::LINE_AA);

        x_       = (ControlView::WIDTH + x_ + _LINE_LENGTH) / 2;
        y_       = (y + ControlView::ICON_HEIGHT - _LINE_LENGTH) / 2;
        start_pt = { x_, y_ };
        end_pt   = { x_ + _LINE_LENGTH, y_ };
        cv::line(image, start_pt + diagonal_offset, end_pt + diagonal_offset, color / 2, _LINE_THICKNESS, cv::LINE_AA);
        cv::line(image, start_pt, end_pt, color, _LINE_THICKNESS, cv::LINE_AA);
    }

    /** Draws a control in its embedding content - Match Control. */
    void ControlView::_CtrlMatch::draw(avt::ImageType& image) noexcept
    {
        /*** /
        x = (view.WIDTH - self._SIZE) // 2
        y = self.y + 1
        if self.enabled:
            img = self._ICON_ON if self.is_active else self._ICON_OFF
        else:
            img = self._ICON_DISABLED
        view.content[ y:y+self._SIZE, x:x+self._SIZE, : ] = img[ :, :, : ]
        /***/
    }

    /** Draws a control in its embedding content - Overlays Control. */
    void ControlView::_CtrlOverlays::draw(avt::ImageType& image) noexcept
    {
        /*** /
        x = (view.WIDTH - self._SIZE) // 2
        y = self.y + 5
        if self.enabled:
            img = self._ICON_ON if self.is_active else self._ICON_OFF
        else:
            img = self._ICON_DISABLED
        view.content[ y:y+self._SIZE, x:x+self._SIZE, : ] = img[ :, :, : ]
        /***/
    }

    /** Draws a control in its embedding content - Record Control. */
    void ControlView::_CtrlRecord::draw(avt::ImageType& image) noexcept
    {
        /*** /
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
        /***/
    }

    /** Creates the associated slider - Record Control. */
    void ControlView::_CtrlRecord::m_create_slider() noexcept
    {
        /*** /
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
        /***/
    }

    /** Draws a control in its embedding content - Replay Control. */
    void ControlView::_CtrlReplay::draw(avt::ImageType& image) noexcept
    {
        /*** /
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
        /***/
    }

    /** Draws a control in its embedding content - Target Control. */
    void ControlView::_CtrlTarget::draw(avt::ImageType& image) noexcept
    {
        /*** /
        x = (ControlView.WIDTH  - self._SIZE) // 2
        y = self.y + (ControlView.ICON_HEIGHT - self._SIZE) // 2
        if self.enabled:
            img = self._ICON_ACTIVE if self.is_active else self._ICON_INACTIVE
        else:
            img = self._ICON_DISABLED
        view.content[ y:y+self._SIZE, x:x+self._SIZE, : ] = img[ :, :, : ]
        /***/
    }

    /** Draws a control in its embedding content - Time Control. */
    void ControlView::_CtrlTime::draw(avt::ImageType& image) noexcept
    {
        /*** /
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
        /***/
    }

    /** Creates the labels that are associated with the Time control. */
    void ControlView::_CtrlTime::m_create_labels() noexcept
    {
        /*** /
        self.time_label = Label( x=x,
                                y=y+self._TIME_TEXT_SIZE,
                                text_font=Font(self._TIME_TEXT_SIZE, YELLOW-32, bold=True) )
        self.duration_label = Label( x=x,
                                    y=y+self._FULL_HEIGHT,
                                    text_font=Font(self._DURATION_TEXT_SIZE, YELLOW) )
        /***/
    }

    /** Draws a control in its embedding content - Time Control. */
    void ControlView::_CtrlTimer::draw(avt::ImageType& image) noexcept
    {
        /*** /
        x = (view.WIDTH - self._SIZE) // 2
        y = self.y + 1
        if self.enabled:
            img = self._ICON_ON if self.is_active else self._ICON_OFF
        else:
            img = self._ICON_DISABLED
        view.content[ y:y+self._SIZE, x:x+self._SIZE, : ] = img[ :, :, : ]
        /***/
    }

}