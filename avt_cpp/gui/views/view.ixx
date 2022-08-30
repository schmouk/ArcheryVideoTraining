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

#include <opencv2/imgproc.hpp>
#include <opencv2/core/mat.hpp>
#include <opencv2/core/types.hpp>

#include "utils/types.h"


export module gui.views.view;


import avt.config;
import utils.coords2d;
import utils.rgb_color;
import utils.size;
import utils;


//===========================================================================
export namespace avt::gui::views
{
    //=======================================================================
    /** @brief The base class for all displayed items. */
    class View : public avt::ImageType
    {
    private:
        using RGBColor = avt::utils::RGBColor;  //!< internal wrapper to the class of colors.


    public:
        //---   Wrappers   --------------------------------------------------
        using MyBaseType = avt::ImageType;        //!< wrapper to the base class
        using PosType    = avt::utils::Coords2D;  //!< wrapper to the Coords2D class
        using SizeType   = avt::utils::Size;      //!< wrapper to the Size class


        //---   Constructors / Destructors   --------------------------------
        /** @brief Value Constructor (4 scalars + 1 color). */
        template<typename X, typename Y, typename H, typename W>
            requires std::is_arithmetic_v<X> && std::is_arithmetic_v<Y> && std::is_arithmetic_v<H> && std::is_arithmetic_v<W>
        inline View(View* p_parent_view,
            const X x,
            const Y y,
            const W width,
            const H height,
            const RGBColor& bg_color = avt::config::DEFAULT_BACKGROUND) noexcept
            : MyBaseType(height, width, (cv::Vec3b)bg_color),
              bg_color{ bg_color },
              p_parent_view(p_parent_view),
              pos(x, y)
        {}

        /** @brief Value Constructor (1 pos + 1 size + 1 color). */
        inline View(View* p_parent_view,
                    const avt::utils::Coords2D& top_left,
                    const avt::utils::Size& size,
                    const RGBColor& bg_color = avt::config::DEFAULT_BACKGROUND) noexcept
            : MyBaseType(size.height, size.width, (cv::Vec3b)bg_color),
              bg_color{ bg_color },
              p_parent_view(p_parent_view),
              pos(top_left)
        {}

        /** @brief Value Constructor (1 rect + 1 color). */
        inline View(View* p_parent_view,
                    const avt::CVRect& rect,
                    const RGBColor& bg_color = avt::config::DEFAULT_BACKGROUND) noexcept
            : MyBaseType(rect.height, rect.width, (cv::Vec3b)bg_color),
              bg_color{ bg_color },
              p_parent_view(p_parent_view),
              pos(rect.tl())
        {}

        /** @brief Main View Constructor (2 scalars + 1 color). */
        template<typename H, typename W>
            requires std::is_arithmetic_v<H>&& std::is_arithmetic_v<W>
        inline View(const W width,
                    const H height,
                    const RGBColor& bg_color = avt::config::DEFAULT_BACKGROUND) noexcept
            : MyBaseType(height, width, (cv::Vec3b)bg_color),
              bg_color{ bg_color },
              p_parent_view(nullptr),
              pos(0, 0)
        {}

        /** @brief Value Constructor (1 size + 1 color). */
        inline View(const avt::utils::Size& size,
                    const RGBColor& bg_color = avt::config::DEFAULT_BACKGROUND) noexcept
            : MyBaseType(size.height, size.width, (cv::Vec3b)bg_color),
              bg_color{ bg_color },
              p_parent_view(nullptr),
              pos(0, 0)
        {}

        /** @brief Default Empty Constructor. */
        View() noexcept = default;

        /** @brief Default Copy Constructor. */
        View(const View&) noexcept = default;

        /** @brief Default Move Constructor. */
        View(View&&) noexcept = default;

        /** @brief Default Destructor. */
        virtual ~View() noexcept = default;


        //---   Assignments   -----------------------------------------------
        /** @brief Default Copy assignment. */
        View& operator= (const View&) noexcept = default;

        /** @brief Default Move assignment. */
        View& operator= (View&&) noexcept = default;


        //---   Operations   ------------------------------------------------
        /** @brief Fills this view background with its background color. */
        inline void clear() noexcept
        {
            cv::rectangle(*this, cv::Rect(0, 0, width(), height()), cv::Vec3b(bg_color), cv::FILLED);
        }
        
        /** @brief Draws this view into the parent view.
        *
        * Caution: this is not thread safe.
        */
        inline void draw() noexcept
        {
            draw(*p_parent_view);
        }

        /** @brief Draws this view into the specified image.
        *
        * Caution: this is not thread safe.
        */
        void draw(avt::ImageType& image) noexcept;

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

        /** @brief Resizes the content of this view according to new size specification. */
        void resize(const avt::utils::Size& new_size) noexcept;

        /** @brief Resizes this view. */
        template<typename W, typename H>
            requires std::is_arithmetic_v<W> && std::is_arithmetic_v<H>
        inline void resize(const W new_width, const H new_height)
        {
            resize(avt::utils::Size{ new_width, new_height });
        }

        /** @brief Resizes this view with multiplying factor - negative values for scaling factor throw exception Size::ScalingValueException. */
        template<typename T>
            requires std::is_arithmetic_v<T>
        inline void resize(const T factor) noexcept(false)
        {
            resize(size() * factor);
        }

        /** @brief Scales this view - negative values for scaling factor throw exception Size::ScalingValueException. */
        template<typename T>
            requires std::is_arithmetic_v<T>
        inline void scale(const T factor) noexcept(false)
        {
            resize(factor);
            pos *= factor;
        }

        /** @brief Sets position and parent view (2 coordinates). */
        template<typename X, typename Y>
            requires std::is_arithmetic_v<X> && std::is_arithmetic_v<Y>
        inline void set(const X x_, const Y y_, View* p_parent_view_ = nullptr)
        {
            move_at(x_, y_);
            p_parent_view = p_parent_view_;
        }

        /** @brief Sets position and parent view (1 position). */
        inline void set(const avt::utils::Coords2D& pos, View* p_parent_view = nullptr)
        {
            set(pos.x, pos.y, p_parent_view);
        }


        //---   Accessors   -------------------------------------------------
        /** @brief Returns the number of pixels contained in this view. */
        inline const size_t area() const noexcept
        {
            return total();
        }

        /** @brief Returns the height of this view (unit: pixels). */
        inline const int height() const noexcept
        {
            return rows;
        }

        /** @brief Returns true if this View is NULL_VIEW, or false otherwise. */
        inline const bool is_null() const noexcept
        {
            return empty();
        }

        /** @brief Returns the size of this view. */
        inline const avt::utils::Size size() const noexcept
        {
            return avt::utils::Size(cols, rows);
        }

        /** @brief Returns the width of this view (unit: pixels). */
        inline const int width() const noexcept
        {
            return cols;
        }


        //---   Attributes   ------------------------------------------------
        RGBColor bg_color;       //!< the background color for this view.
        PosType  pos;            //!< the position in the parent view of this view's top-left corner 
        View*    p_parent_view;  //!< a pointer to this view's parent view


    private:
        /** @brief Evaluates the clipped size of this view when displayed in a frame. */
        avt::utils::Size m_clipping_size(const PosType& abs_pos,
                                         const avt::utils::Size& size,
                                         avt::ImageType& image) const noexcept;

        /** @brief Evaluates the absolute position of this view within the root View. */
        avt::utils::Coords2D m_get_abs_pos(const View* p_current_view) const noexcept;
    };

}
