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


export module gui.avt_window;

import avt.config;
import gui.items.cursor;
import video.frame;
import mtmp.mutex;
import utils.rgb_color;
import utils.size;
import gui.items.view;


//===========================================================================
export namespace avt::gui
{
    //=======================================================================
    /** @brief The class of AVT windows. */
    class AVTWindow
    {
    public:
        //---   Constructors / Destructors   --------------------------------
        /** @brief Empty constructor. */
        AVTWindow() noexcept
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
        AVTWindow(const std::string&          window_id,
                  const avt::utils::RGBColor& bg_color = avt::config::DEFAULT_BACKGROUND) noexcept
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
        AVTWindow(const cv::String&           window_id,
                  const cv::String&           window_title,
                  const avt::utils::RGBColor& bg_color = avt::config::DEFAULT_BACKGROUND) noexcept
            : window_content{},
              window_id{ window_id },
              size{},
              mutex{},
              bg_color{ bg_color },
              b_full_screen{ true },
              b_fixed_size{ true }
        {
            m_create_window();
            set_title(window_title);
        }

        /** @brief Constructor with ID and dims. */
        template<typename W, typename H>
            requires std::is_arithmetic_v<W> && std::is_arithmetic_v<H>
        AVTWindow(const cv::String&           window_id,
                  const W                     width,
                  const H                     height,
                  const avt::utils::RGBColor& bg_color = avt::config::DEFAULT_BACKGROUND) noexcept
            : window_id{ window_id },
              size(width, height),
              mutex{},
              bg_color{ bg_color },
              b_full_screen{ false },
              b_fixed_size{ true }
        {
            m_create_window();
            m_set_default_title();
        }

        /** @brief Constructor with ID and size. */
        AVTWindow(const cv::String&           window_id,
                  const utils::Size&          size,
                  const avt::utils::RGBColor& bg_color = avt::config::DEFAULT_BACKGROUND) noexcept
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

        /** @brief Constructor with ID, title and dims. */
        template<typename W, typename H>
            requires std::is_arithmetic_v<W> && std::is_arithmetic_v<H>
        AVTWindow(const cv::String&           window_id,
                  const cv::String&           window_title,
                  const W                     width,
                  const H                     height,
                  const avt::utils::RGBColor& bg_color = avt::config::DEFAULT_BACKGROUND) noexcept
            : window_id{ window_id },
              size(width, height),
              mutex{},
              bg_color{ bg_color },
              b_full_screen{ false },
              b_fixed_size{ true }
        {
            m_create_window();
            set_title(window_title);
        }

        /** @brief Constructor with ID, title and size. */
        AVTWindow(const cv::String&           window_id,
                  const cv::String&           window_title,
                  const utils::Size&          size,
                  const avt::utils::RGBColor& bg_color = avt::config::DEFAULT_BACKGROUND) noexcept
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

        /** Deleted Copy constructor. */
        AVTWindow(const AVTWindow&) noexcept = delete;

        /** Deleted Move constructor. */
        AVTWindow(AVTWindow&&) noexcept = delete;

        /** @brief Destructor. */
        inline ~AVTWindow() noexcept
        {
            cv::destroyWindow(window_id);
        }


        //---   Assignments   -----------------------------------------------
        /** @brief Deleted Copy assignment. */
        AVTWindow& operator= (const AVTWindow&) = delete;

        /** @brief Deleted Move assignment. */
        AVTWindow& operator= (AVTWindow&&) = delete;


        //---   Operations   ------------------------------------------------
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
        const int draw(const bool b_forced = false,
                       const int  hit_delay_ms = 1) noexcept
        {
            cv::imshow(window_id, window_content);
            return cv::waitKey(hit_delay_ms);
        }


        /** @brief Draws a specified View in this window content. */
        inline void draw_view(const avt::gui::items::View& view) noexcept
        {
            view.draw(window_content);
        }


        /** @brief Returns the current (x, y) position of this window, expressed in pixels. */
        inline avt::utils::Coords2D get_pos() const noexcept
        {
            const cv::Rect window_rect{ get_rect() };
            return avt::utils::Coords2D{ window_rect.x, window_rect.y };
        }


        /** @brief Returns the current rectangle (x, y, width, height) of this window, expressed in pixels. */
        inline cv::Rect get_rect() const noexcept
        {
            return cv::getWindowImageRect(window_id);
        }


        /** @brief Returns the current (width, height) of this window, expressed in pixels. */
        inline avt::utils::Size get_size() const noexcept
        {
            const cv::Rect window_rect{ get_rect() };
            return  avt::utils::Size(window_rect.width, window_rect.height);
        }


        /** @brief Returns True as long as this window has not been closed. */
        const bool is_visible() noexcept
        {
            return cv::getWindowProperty(window_id, cv::WND_PROP_VISIBLE) >= 1;
        }


        /** @brief Sets the window title as displayed in the top bar of this AVT window. */
        inline void set_title(const std::string& new_title) noexcept
        {
            cv::setWindowTitle(window_id, new_title);
        }


        //---   Attributes   ------------------------------------------------
        avt::video::Frame       window_content;
        avt::utils::Size        size;
        avt::mtmp::Mutex        mutex;
        cv::String              window_id;
        cv::String              title;
        avt::utils::RGBColor    bg_color;
        bool                    b_full_screen;
        bool                    b_fixed_size;


    private:
        static inline std::atomic<long> m_windows_count = 0;

        /** @brief Creates the OpenCV window. */
        void m_create_window() noexcept
        {
            if (b_full_screen) {
                cv::namedWindow(window_id, cv::WINDOW_FULLSCREEN);
                size = get_size();
            }
            else {
                cv::namedWindow(window_id, cv::WINDOW_NORMAL | cv::WINDOW_KEEPRATIO | cv::WINDOW_GUI_EXPANDED);
                cv::resizeWindow(window_id, size.width, size.height);
            }

            window_content = avt::video::Frame{ size, CV_8UC3, avt::config::DEFAULT_BACKGROUND };

            avt::gui::items::Cursor_NORMAL.activate();
        }

        /** @brief Sets a default ID for this AVT window. */
        inline void m_set_default_id()
        {
            window_id = cv::String(std::format("AVT-Window-{:03d}", ++m_windows_count).c_str());
        }

        /** @brief Sets a default title for this AVT window. */
        inline void m_set_default_title()
        {
            title = window_id;
        }
    };

}