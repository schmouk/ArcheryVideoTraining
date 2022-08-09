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
#include <format>

#include <opencv2/core/cvstd.hpp>
#include <opencv2/highgui.hpp>

#include "gui/items/view.h"


module gui.avt_window;

import avt.config;
import utils.coords2d;
import gui.items.cursor;
import video.frame;
import mtmp.mutex;
import utils.rgb_color;
import utils.size;



//===========================================================================
namespace avt::gui
{
    /** @brief Empty constructor. */
    AVTWindow::AVTWindow() noexcept
        : size{},
          mutex{},
          bg_color{ avt::config::DEFAULT_BACKGROUND },
          b_full_screen{ true },
          b_fixed_size{ true }
    {
        m_set_default_id();
        m_create_window();
        m_set_default_title();
    }

    /** @brief Constructor with specified ID - full screen size. */
    AVTWindow::AVTWindow(const std::string&          window_id,
                         const avt::utils::RGBColor& bg_color) noexcept
        : window_id{ window_id },
          size{},
          mutex{},
          bg_color{ bg_color },
          b_full_screen{ true },
          b_fixed_size{ true }
    {
        m_create_window();
        m_set_default_title();
    }

    /** @brief Constructor with specified ID and title - full screen size. */
    AVTWindow::AVTWindow(const cv::String&           window_id,
                         const cv::String&           window_title,
                         const avt::utils::RGBColor& bg_color) noexcept
        : window_id{ window_id },
          size{},
          mutex{},
          bg_color{ bg_color },
          b_full_screen{ true },
          b_fixed_size{ true }
    {
        m_create_window();
        set_title(window_title);
    }

    /** @brief Constructor with ID and size. */
    AVTWindow::AVTWindow(const cv::String&           window_id,
                            const utils::Size&          size,
                            const avt::utils::RGBColor& bg_color) noexcept
        : window_id{ window_id },
          size{ size },
          mutex{},
          bg_color{ bg_color },
          b_full_screen{ false },
          b_fixed_size{ true }
    {
        m_create_window();
        m_set_default_title();
    }

    /** @brief Constructor with ID, title and size. */
    AVTWindow::AVTWindow(const cv::String&           window_id,
                            const cv::String&           window_title,
                            const utils::Size&          size,
                            const avt::utils::RGBColor& bg_color) noexcept
        : window_id{ window_id },
          size{ size },
          mutex{},
          bg_color{ bg_color },
          b_full_screen{ false },
          b_fixed_size{ true }
    {
        m_create_window();
        set_title(window_title);
    }


    /** @brief Draws a content into this window.

        Notice:  the content will automatically be resized to the  window
        current  size  if  this  window has been created with a specified
        size.

        @arg b_forced : bool
        Set this to True to get immediate drawing of  this  view  content
        in  the  embedding window.  Set it to False if delayed drawing is
        acceptable. Defaults to false.

        @arg hit_delay_ms : int
        A delay,  expressed in milliseconds,  used for  the  scanning  of
        keyboards hits.  If any key is hit within this delay, its integer
        code is immediately  returned  as  the  result  of  this  method,
        otherwise  -1  is  returned.   If  delay  is  0 or negative,  the
        application is stopped until a key  is  hit.  Defaults  to  1 ms,
        which  is  the shortest delay that is available before completing
        this method.

        @return The integer code of the key that was hit while displaying
        this content, or -1 if no key was hit after expressed delay.
    */
    const int AVTWindow::draw(const bool b_forced, const int  hit_delay_ms) noexcept
    {
        cv::imshow(window_id, *p_main_view);
        return cv::waitKey(hit_delay_ms);
    }


    /** @brief Draws a specified View in this window content. */
    /** /
    inline void AVTWindow::draw_view(avt::gui::items::View& view) noexcept
    {
        view.draw(window_content);
    }
    /**/

    /** @brief Creates the OpenCV window. */
    void AVTWindow::m_create_window() noexcept
    {
        // sets size and creates the associated OpenCV named window
        if (b_full_screen) {
            cv::namedWindow(window_id, cv::WINDOW_FULLSCREEN);
            size = get_size();
        }
        else {
            cv::namedWindow(window_id, cv::WINDOW_NORMAL | cv::WINDOW_KEEPRATIO | cv::WINDOW_GUI_EXPANDED);
            cv::resizeWindow(window_id, size.width, size.height);
        }

        // creates the associated main view
        p_main_view = new avt::gui::items::View(nullptr, avt::utils::Coords2D{ 0, 0 }, size, avt::config::DEFAULT_BACKGROUND);

        // creates the emebedded views
        m_create_subviews();

        // activates the normal cursor (left-up pointing arrow)
        avt::gui::items::Cursor_NORMAL.activate();
    }

    /** @brief Creates all the embedded sub-views. */
    void AVTWindow::m_create_subviews() noexcept
    {
        // TODO: Implement this private method
    }


}