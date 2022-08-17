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

#include <array>
#include <atomic>
#include <cstring>
#include <exception>
#include <format>

#include <opencv2/core/cvstd.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

#include "devices/cameras/camera.h"
#include "gui/items/icon.h"
#include "utils/types.h"


module gui.views.control_view;

import devices.cameras.cameras_pool;
import avt.config;
import utils.coords2d;
import gui.items.cursor;
import gui.fonts.font;
import gui.items.picture;
import utils.rgb_color;
import gui.views.view;


//===========================================================================
namespace avt::gui::views
{
    /** Value Constructor. */
    ControlView::ControlView(ViewType* p_parent_view, const CamerasPool& cameras_pool) noexcept(false)
        : ViewType{ p_parent_view,
                    0, p_parent_view->width() - WIDTH, // i.e. position in main window
                    WIDTH, p_parent_view->height() },  // i.e. size of this view in main window
        ThreadType{ "controls-thrd", 1000 }
    {
        m_create_controls(cameras_pool);
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
    void ControlView::m_create_controls(const CamerasPool& cameras_pool) noexcept
    {
        int y = 15 + ControlView::ICON_PADDING;

        // Cameras controls
        m_cameras_ctrls.clear();
        m_cameras_ctrls.reserve(avt::config::CAMERAS_MAX_COUNT);

        for (Camera camera : cameras_pool) {
            m_cameras_ctrls.emplace_back(
                ControlView::_CtrlCamera(camera,
                    ControlView::CENTER,
                    y + ControlView::ICON_HEIGHT * camera.cam_id)
            );
        }

        for (int cam_id = int(m_cameras_ctrls.size()); cam_id < avt::config::CAMERAS_MAX_COUNT; ++cam_id) {
            avt::devices::cameras::NullCamera null_cam{ cam_id };
            m_cameras_ctrls.emplace_back(
                ControlView::_CtrlCamera(null_cam,
                    ControlView::CENTER,
                    y + ControlView::ICON_HEIGHT * cam_id)
            );
        }

        for (auto cam_ctrl : m_cameras_ctrls)
            m_controls_list.emplace_back(cam_ctrl);

        // Targets control
        y += avt::config::CAMERAS_MAX_COUNT * ControlView::ICON_HEIGHT + 6;
        m_target_ctrl = ControlView::_CtrlTarget(5, y, false, false);
        m_controls_list.emplace_back(m_target_ctrl);

        // Lines controls
        y += 2 * ControlView::ICON_PADDING + ControlView::ICON_HEIGHT;
        m_lines_ctrl = ControlView::_CtrlLines(5, y, false, false);
        m_controls_list.emplace_back(m_lines_ctrl);

        // Delay control
        y += 2 * ControlView::ICON_PADDING + ControlView::ICON_HEIGHT;
        m_delay_ctrl = ControlView::_CtrlDelay(5, y, false, false);
        m_controls_list.emplace_back(m_delay_ctrl);

        // Record controls
        y += ControlView::ICON_PADDING * 2 + ControlView::ICON_HEIGHT;
        m_record_ctrl = ControlView::_CtrlRecord(5, y, false, false);
        m_controls_list.emplace_back(m_record_ctrl);

        // Replay controls
        y += ControlView::ICON_PADDING + ControlView::ICON_HEIGHT;
        m_replay_ctrl = ControlView::_CtrlReplay(5, y, false, false);
        m_controls_list.emplace_back(m_replay_ctrl);

        // Overlays control
        y += 2 * ControlView::ICON_PADDING + ControlView::ICON_HEIGHT + 20;
        m_overlays_ctrl = ControlView::_CtrlOverlays(5, y, false, false);
        m_controls_list.emplace_back(m_overlays_ctrl);

        // Timer control
        y += (ControlView::_CtrlOverlays::SIZE - ControlView::ICON_HEIGHT) + ControlView::ICON_PADDING + ControlView::ICON_HEIGHT;
        m_timer_ctrl = ControlView::_CtrlTimer(5, y, false, false);
        m_controls_list.emplace_back(m_timer_ctrl);

        // Match control
        y += ControlView::ICON_PADDING + ControlView::ICON_HEIGHT;
        m_match_ctrl = ControlView::_CtrlMatch(5, y, false, false);
        m_controls_list.emplace_back(m_match_ctrl);

        // Time controls
        y += 2 * ControlView::ICON_PADDING + ControlView::ICON_HEIGHT;
        m_time_ctrl = ControlView::_CtrlTime(5, y);
        m_controls_list.emplace_back(m_time_ctrl);

        // Exit control
        m_exit_ctrl = ControlView::_CtrlExit(ControlView::width(), ControlView::height());
        m_controls_list.emplace_back(m_exit_ctrl);
    }

    /** @brief Draws lines on this view borders. */
    void ControlView::m_draw_borders() noexcept
    {
        /***/
        RGBColor bg_color{ avt::config::DEFAULT_BACKGROUND };
        const cv::Scalar dark_color  = (cv::Scalar)(bg_color / 2 );
        const cv::Scalar light_color = (cv::Scalar)(bg_color * 2 );
        cv::rectangle(*this, cv::Rect(3, 3, width() - 2, height() - 2), dark_color , 1, cv::LINE_4);
        cv::rectangle(*this, cv::Rect(4, 4, width() - 3, height() - 3), dark_color , 1, cv::LINE_4);
        cv::rectangle(*this, cv::Rect(1, 1, width() - 4, height() - 4), light_color, 1, cv::LINE_4);
        cv::rectangle(*this, cv::Rect(2, 2, width() - 5, height() - 5), light_color, 1, cv::LINE_4);
    }

    /** @brief Draws all controls in this control view. */
    void ControlView::m_draw_controls() noexcept
    {
        try {
            for (ControlView::_CtrlBase& ctrl : m_controls_list) {
                try {
                    ctrl.draw(*this);
                }
                catch (...) {}
            }
        }
        catch (...) {}
    }

    /** Draws a control in its embedding content - Controls Base class. */
    void ControlView::_CtrlBase::draw(avt::ImageType& view_image) noexcept
    {
        avt::gui::fonts::Font font;
        try {
            if (enabled)
                font = active ? FONT_ACTIVE : FONT_ENABLED;
            else
                font = FONT_DISABLED;
            font.draw_text(typeid(*this).name(), view_image, text_pos, false);
        }
        catch (...) {}
    }

    /** Draws a control in its embedding content - Camera Controls. */
    void ControlView::_CtrlCamera::draw(avt::ImageType& view_image) noexcept
    {
        avt::ImageType img;
        Font           font;
        int            x_id;

        try {
            if (camera.is_ok()) {
                if (is_on) {
                    img = ControlView::_CtrlCamera::ICON_ON;
                    font = ControlView::_CtrlCamera::FONT_ON;
                    x_id = ControlView::WIDTH / 2 - 5;
                }
                else {
                    img = ControlView::_CtrlCamera::ICON_OFF;
                    font = ControlView::_CtrlCamera::FONT_OFF;
                    x_id = ControlView::WIDTH / 2 - 9;
                }
            }
            else {
                img = ControlView::_CtrlCamera::ICON_DISABLED;
                font = ControlView::_CtrlCamera::FONT_NOT_OK;
                x_id = ControlView::WIDTH / 2 - 9;
            }

            img.copyTo(view_image(cv::Range(y, y + ControlView::_CtrlCamera::HEIGHT),
                cv::Range(x, x + ControlView::_CtrlCamera::WIDTH)));
            font.draw_text(std::format("{:d}", camera.get_id()),
                view_image,
                x_id,
                y + (ControlView::_CtrlCamera::HEIGHT + font.size) / 2,
                camera.is_ok());
        }
        catch (...) {}
    }

    /** Draws a control in its embedding content - Delay Control. */
    void ControlView::_CtrlDelay::draw(avt::ImageType& view_image) noexcept
    {
        /***/
        avt::ImageType img;
        if (enabled)
            img = active ? ICON_ON : ICON_OFF;
        else
            img = ICON_DISABLED;

        const int x_ = (view_image.cols - SIZE) / 2;
        const int y_ = y + 1;
        img.copyTo(avt::ImageType(view_image, cv::Rect(x_, y_, SIZE, SIZE)));

        //##font.draw_text( view, Point(self.x + 5, self.y + self._FONT_SIZE), "Delay" );
        //slider.draw(*this);

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
        : _CtrlBase{},
          x{ (view_width - ControlView::_CtrlExit::WIDTH) / 2 },
          y{ view_height - ControlView::_CtrlExit::HEIGHT - 12 }
    {}

    /** Draws a control in its embedding content - Exit Control. */
    void ControlView::_CtrlExit::draw(avt::ImageType& view_image) noexcept
    {
        try {
            ICON_EXIT.draw(view_image, x, y);
        }
        catch (...) {}
        /*** /
        try:
            view.content[ self.y:self.y+self.height,
                            self.x:self.x+self.width , : ] = self._ICON_EXIT[ :, :, : ]
        except:
            pass
        /***/
    }

    /** Draws a control in its embedding content - Lines Control. */
    void ControlView::_CtrlLines::draw(avt::ImageType& view_image) noexcept
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

        x_ = x + 13;
        y_ = y + ControlView::ICON_HEIGHT / 2;
        start_pt = { x_, y_ };
        end_pt = { x_ + LINE_LENGTH, y_ };
        cv::line(view_image, start_pt + diagonal_offset, end_pt + diagonal_offset, color / 2, LINE_THICKNESS, cv::LINE_AA);
        cv::line(view_image, start_pt, end_pt, color, LINE_THICKNESS, cv::LINE_AA);

        x_ = (ControlView::WIDTH + x_ + LINE_LENGTH) / 2;
        y_ = (y + ControlView::ICON_HEIGHT - LINE_LENGTH) / 2;
        start_pt = { x_, y_ };
        end_pt = { x_ + LINE_LENGTH, y_ };
        cv::line(view_image, start_pt + diagonal_offset, end_pt + diagonal_offset, color / 2, LINE_THICKNESS, cv::LINE_AA);
        cv::line(view_image, start_pt, end_pt, color, LINE_THICKNESS, cv::LINE_AA);
    }

    /** Draws a control in its embedding content - Match Control. */
    void ControlView::_CtrlMatch::draw(avt::ImageType& view_image) noexcept
    {
        avt::ImageType img;
        if (enabled)
            img = active ? ICON_ON : ICON_OFF;
        else
            img = ICON_DISABLED;

        const int x_ = (view_image.rows - SIZE) / 2;
        const int y_ = y + 1;
        img.copyTo(avt::ImageType(view_image, cv::Rect(x_, y_, SIZE, SIZE)));
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
    void ControlView::_CtrlOverlays::draw(avt::ImageType& view_image) noexcept
    {
        Icon img;
        if (enabled)
            img = active ? ICON_ON : ICON_OFF;
        else
            img = ICON_DISABLED;

        const int x_ = (view_image.rows - ControlView::_CtrlDelay::SIZE) / 2;
        const int y_ = y + 5;
        img.draw(view_image, x_, y_);
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
    void ControlView::_CtrlRecord::draw(avt::ImageType& view_image) noexcept
    {
        Icon img;
        Font font;

        const std::string cursor_text = ""; //std::format("{}", m_slider.value);
        const size_t cursor_text_length = cursor_text.length(); // length as the contained chars count

        if (enabled) {
            if (active) {
                img = ICON_ON;
                font = cursor_text_length < 3 ? FONT_2_ON : FONT_3_ON;
            }
            else {
                img = ICON_OFF;
                font = cursor_text_length < 3 ? FONT_2_OFF : FONT_3_OFF;
            }
        }
        else {
            img = ICON_DISABLED;
            font = cursor_text.length() < 3 ? FONT_2_DISABLED : FONT_3_DISABLED;
        }

        int x_ = (view_image.cols - ICON_SIZE) / 2;
        int y_ = y + 1;
        img.draw(view_image, x_, y_);

        //const int cursor_text_width = font.gettext_width(cursor_text);  // width as a pixels count
        //x_ = (view_image.width() - cursor_text_width) / 2 + 1;
        //y_ = y + (ICON_SIZE + FONT_SIZE) / 2 - (cursor_text_length < 3 ? 2 : 4);

        //font.draw_text(view_image, cv::Point(x, y), cursor_text, true);
        //##font.draw_text(view_image, cv::Point(x + 5, y + FONT_SIZE), "Delay");
        //m_slider.draw(view_image);
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
    void ControlView::_CtrlReplay::draw(avt::ImageType& image_view) noexcept
    {
        std::array<Icon, 5> icons;
        if (enabled) {
            if (active)
                icons = std::array<Icon, 5>{ ICON_STEP_BW_ON,
                                             ICON_STEP_FW_ON,
                                             ICON_PLAY_ON,
                                             ICON_FBW_ON,
                                             ICON_FFW_ON };
            else
                icons = std::array<Icon, 5>{ ICON_STEP_BW_OFF,
                                             ICON_STEP_FW_OFF,
                                             ICON_PLAY_OFF,
                                             ICON_FBW_OFF,
                                             ICON_FFW_OFF };
        }
        else
            icons = std::array<Icon, 5>{ ICON_STEP_BW_DISABLED,
                                         ICON_STEP_FW_DISABLED,
                                         ICON_PLAY_DISABLED,
                                         ICON_FBW_DISABLED,
                                         ICON_FFW_DISABLED };

        const int x0 = x + 5;
        const int y0 = y + 23;
        const int x1 = x0 + SIZE;
        const int x2 = x1 + SIZE;
        const int y1 = y0 + SIZE / 2 + 2;
        const int y2 = y0 + SIZE + 3;

        //icons[0].insert(image_view, x0, y0, SIZE, SIZE);
        //icons[1].insert(image_view, x2, y0, SIZE, SIZE);
        //icons[2].insert(image_view, x1, y1, SIZE, SIZE);
        //icons[3].insert(image_view, x0, y2, SIZE, SIZE);
        //icons[4].insert(image_view, x2, y2, SIZE, SIZE);
    }

    /** Draws a control in its embedding content - Target Control. */
    void ControlView::_CtrlTarget::draw(avt::ImageType& view_image) noexcept
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
    void ControlView::_CtrlTime::draw(avt::ImageType& view_image) noexcept
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
    void ControlView::_CtrlTimer::draw(avt::ImageType& view_image) noexcept
    {
        Icon img;
        if (enabled)
            img = active ? ICON_ON : ICON_OFF;
        else
            img = ICON_DISABLED;

        const int x_ = (view_image.cols - SIZE) / 2;
        const int y_ = y + 1;
        img.draw(view_image, x_, y_);
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