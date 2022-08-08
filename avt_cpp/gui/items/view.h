#pragma once
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


import avt.config;
import utils.coords2d;
import video.frame;
import utils.rgb_color;
import utils.size;
import utils;


//===========================================================================
namespace avt::gui::items
{
    //=======================================================================
    /** @brief The base class for all displayed items. */
    class View : public cv::Mat3b
    {
    public:
        //---   Wrappers   --------------------------------------------------
        using MyBaseType = cv::Mat3b;             //!< wrapper to the base class
        using PosType    = avt::utils::Coords2D;  //!< wrapper to the Coords2D class
        using SizeType   = avt::utils::Size;      //!< wrapper to the Size class


        //---   Constructors / Destructors   --------------------------------
        /** @brief Value Constructor (4 scalars + 1 color). */
        template<typename X, typename Y, typename H, typename W>
            requires std::is_arithmetic_v<X>&& std::is_arithmetic_v<Y>&& std::is_arithmetic_v<H>&& std::is_arithmetic_v<W>
        inline View(const View* parent_view,
                    const X x,
                    const Y y,
                    const W width,
                    const H height,
                    const avt::utils::RGBColor& bg_color = avt::config::DEFAULT_BACKGROUND) noexcept
            : MyBaseType(height, width, (cv::Vec3b)bg_color),
              p_view(parent_view),
              pos(x, y),
              size(width, height)
        {}

        /** @brief Value Constructor (1 pos + 1 size + 1 color). */
        inline View(const View* parent_view,
                    const avt::utils::Coords2D& top_left,
                    const avt::utils::Size& size,
                    const avt::utils::RGBColor& bg_color = avt::config::DEFAULT_BACKGROUND) noexcept
            : MyBaseType(size.height, size.width, (cv::Vec3b)bg_color),
              p_view(parent_view),
              pos(top_left),
              size(size)
        {}

        /** @brief Value Constructor (1 rect + 1 color). */
        inline View(const View* parent_view,
                    const avt::CVRect& rect,
                    const avt::utils::RGBColor& bg_color = avt::config::DEFAULT_BACKGROUND) noexcept
            : MyBaseType(rect.height, rect.width, (cv::Vec3b)bg_color),
              p_view(parent_view),
              pos(rect.tl()),
              size(rect.size())
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
        void draw(avt::video::Frame& frame) noexcept;

        /** @brief Returns the absolute position of this view in the root View. */
        inline avt::utils::Coords2D get_absolute_pos() const noexcept
        {
            return m_get_abs_pos(this);
        }

        /** @brief Moves this view within its parent view (relative offset). */
        template<typename X, typename Y>
            requires std::is_arithmetic_v<X> && std::is_arithmetic_v<Y>
        inline void move(const X dx, const Y dy)
        {
            pos.move(dx, dy);
        }

        /** @brief Moves this view within its parent view (relative offset). */
        template<typename P>
            requires avt::is_pair_type_v<P>
        inline void move(const P& offset)
        {
            pos.move(offset);
        }

        /** @brief Moves this view within its parent view (absolute pos). */
        template<typename X, typename Y>
            requires std::is_arithmetic_v<X> && std::is_arithmetic_v<Y>
        inline void move_at(const X new_x, const Y new_y)
        {
            pos.move_at(new_x, new_y);
        }

        /** @brief Moves this view within its parent view (absolute pos). */
        inline void move_at(const PosType& new_pos)
        {
            pos.move_at(new_pos);
        }

        /** @brief Moves this view within its parent view (absolute pos). */
        template<typename P>
            requires avt::is_pair_type_v<P>
        inline void move_at(const P& new_pos)
        {
            pos.move_at(new_pos);
        }

        /** @brief Resizes this view. */
        template<typename W, typename H>
            requires std::is_arithmetic_v<W> && std::is_arithmetic_v<H>
        inline void resize(const W new_width, const H new_height)
        {
            size.width = new_width;
            size.height = new_height;
        }

        /** @brief Resizes this view with multiplying factor - negative values for scaling factor throw exception Size::ScalingValueException. */
        template<typename T>
            requires std::is_arithmetic_v<T>
        inline void resize(const T factor) noexcept(false)
        {
            size *= factor;
        }

        /** @brief Scales this view - negative values for scaling factor throw exception Size::ScalingValueException. */
        template<typename T>
            requires std::is_arithmetic_v<T>
        inline void scale(const T factor) noexcept(false)
        {
            pos  *= factor;
            size *= factor;
        }


        //---   Attributes   ------------------------------------------------
        cv::Mat3b                    content;
        PosType                      pos;
        SizeType                     size;
        const avt::gui::items::View* p_view{ nullptr };


    private:
        /** @brief Evaluates the clipped size of this view when displayed in a frame. */
        avt::utils::Size m_clipping_size(const PosType&     abs_pos,
                                         const SizeType&    size,
                                         avt::video::Frame& frame) const noexcept;

        /** @brief Evaluates the absolute position of this view within the root View. */
        avt::utils::Coords2D m_get_abs_pos(const View* p_current_view) const noexcept;
    };

}
