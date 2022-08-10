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
#include <algorithm>
#include <opencv2/core/mat.hpp>
#include <opencv2/core/types.hpp>

#include "utils/types.h"

#include "gui/views/view.h"


import avt.config;
import utils.coords2d;
import video.frame;
import utils.rgb_color;
import utils.size;
import utils;


//===========================================================================
namespace avt::gui::views
{
    /** Draws this view into the specified video frame. */
    void View::draw(avt::video::Frame& frame) noexcept
    {
        const avt::utils::Coords2D abs_pos = get_absolute_pos();
        const avt::utils::Size     final_size = m_clipping_size(abs_pos, size(), frame);
        copyTo(frame(avt::CVRect(abs_pos, final_size)));
    }

    /** Evaluates the clipped size of this view when displayed in a frame. */
    avt::utils::Size View::m_clipping_size(const avt::utils::Coords2D& abs_pos_,
                                           const avt::utils::Size&     size_,
                                           avt::video::Frame&          frame_) const noexcept
    {
        const avt::DimsType width  = std::max(0, std::min(int(size_.width) , frame_.cols - abs_pos_.x));
        const avt::DimsType height = std::max(0, std::min(int(size_.height), frame_.rows - abs_pos_.y));
        return avt::utils::Size(width, height);
    }

    /** @brief Evaluates the absolute position of this view within the root View. */
    avt::utils::Coords2D View::m_get_abs_pos(const View* p_current_view) const noexcept
    {
        if (p_current_view->p_view == nullptr) {
            return p_current_view->pos;
        }
        else {
            return p_current_view->pos + m_get_abs_pos(p_current_view->p_view);
        }
    }

    /** @brief Resizes the content of this view according to new size specification. */
    void View::resize(const avt::utils::Size& new_size) noexcept
    {
        if (size() == new_size)
            return;

        MyBaseType dst;
        if (new_size.area() < area())
            cv::resize(*this, dst, cv::Size(new_size.width, new_size.height), 0.0, 0.0, cv::INTER_LINEAR);
        else
            cv::resize(*this, dst, cv::Size(new_size.width, new_size.height), 0.0, 0.0, cv::INTER_LANCZOS4);
        dst.copyTo(*this);
    }

}
