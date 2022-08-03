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

#include <algorithm>
#include <opencv2/core/mat.hpp>

#include "utils/types.h"


export module gui.views.view;

import avt.config;
import utils.coords2d;
import video.frame;
import utils.rgba_color;
import utils.size;
import utils;


//===========================================================================
export namespace avt::gui::views
{
    //=======================================================================
    /** @brief The base class for displayed items. */
    class View : public avt::utils::Coords2D, public avt::utils::Size
    {
    public:
        //---   Wrappers   --------------------------------------------------
        using PosType  = avt::utils::Coords2D;  //!< wrapper to the Coords2D base class
        using SizeType = avt::utils::Size;      //!< wrapper to the Size base class


        //---   Constructors / Destructors   --------------------------------
        /** @brief Value Constructor (4 scalars + 1 color). */
        template<typename X, typename Y, typename H, typename W>
            requires std::is_arithmetic_v<X> && std::is_arithmetic_v<Y> && std::is_arithmetic_v<H> && std::is_arithmetic_v<H>
        inline View(const View* parent_view,
                    const X x,
                    const Y y,
                    const W width,
                    const H height,
                    const avt::utils::RGBAColor& bg_color = avt::config::DEFAULT_BACKGROUND) noexcept
            : p_view(parent_view),
              PosType(x, y),
              SizeType(width, height),
              content(height, width, CV_8UC3, (avt::CVScalarByte)bg_color)
        {}

        /** @brief Value Constructor (1 pos + 1 size + 1 color). */
        inline View(const View*                  parent_view,
                    const avt::utils::Coords2D&  top_left,
                    const avt::utils::Size&      size,
                    const avt::utils::RGBAColor& bg_color = avt::config::DEFAULT_BACKGROUND) noexcept
            : p_view(parent_view),
              PosType(top_left),
              SizeType(size),
              content((avt::CVSize)size, CV_8UC3, (avt::CVScalarByte)bg_color)
        {}

        /** @brief Value Constructor (1 rect + 1 color). */
        inline View(const View*                  parent_view,
                    const avt::CVRect&           rect,
                    const avt::utils::RGBAColor& bg_color = avt::config::DEFAULT_BACKGROUND) noexcept
            : p_view(parent_view),
              PosType(rect.x, rect.y),
              SizeType(rect.width, rect.height),
              content(rect.size(), CV_8UC3, (avt::CVScalarByte)bg_color)
        {}

        /** @brief Default Copy Constructor. */
        View(const View&) noexcept = default;

        /** @brief Default Move COnstructor. */
        View(View&&) noexcept = default;

        /** @brief Default Destructor. */
        ~View() noexcept = default;


        //---   Assignments   -----------------------------------------------
        /** @brief Default Copy assignment. */
        View& operator= (const View&) noexcept = default;

        /** @brief Default Move assignment. */
        View& operator= (View&&) noexcept = default;


        //---   Operations   ------------------------------------------------
        /** @brief Draws this view into the specified video frame.
        *
        * Caution: this is not thread safe.
        */
        inline void draw(avt::video::Frame& frame) noexcept
        {
            const avt::utils::Coords2D abs_pos    = get_absolute_pos();
            const avt::utils::Size     final_size = m_clipping_size(abs_pos, size(), frame);
            content.copyTo(frame(avt::CVRect(abs_pos, final_size)));
        }

        /** @brief Returns the absolute position of this view in the root View. */
        inline avt::utils::Coords2D get_absolute_pos() noexcept
        {
            return m_get_abs_pos(this);
        }

        /** @brief Returns a reference to the avt::utils::Size size of this view. */
        inline avt::utils::Size size() noexcept
        {
            return (avt::utils::Size)*this;
        }

        /** @brief Returns a const reference to the avt::utils::Size size of this view. */
        inline avt::utils::Size size() const noexcept
        {
            return (avt::utils::Size)(*this);
        }


        //---   Attributes   ------------------------------------------------
        const avt::gui::views::View* p_view;
        cv::Mat                      content;


    private:
        /** @brief Evaluates the clipped size of this view when displayed in a frame. */
        avt::utils::Size m_clipping_size(const avt::utils::Coords2D& abs_pos,
                                          const avt::utils::Size& size,
                                          avt::video::Frame& frame)
        {
            const avt::DimsType width  = std::max(0, std::min(int(size.width) , frame.cols - abs_pos.x));
            const avt::DimsType height = std::max(0, std::min(int(size.height), frame.rows - abs_pos.y));
            return avt::utils::Size(width, height);
        }

        /** @brief Evaluates the absolute position of this view within the root View. */
        avt::utils::Coords2D m_get_abs_pos(const View* p_current_view) noexcept
        {
            if (p_current_view->p_view == nullptr) {
                return *p_current_view;
            }
            else {
                return *p_current_view + m_get_abs_pos(p_current_view->p_view);
            }
        }

    };

}
