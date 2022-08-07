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
#include <opencv2/core/types.hpp>

#include "utils/types.h"


export module gui.items.view;

import avt.config;
import utils.coords2d;
import video.frame;
import utils.rgb_color;
import utils.size;
import utils;


//===========================================================================
export namespace avt::gui::items
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
            requires std::is_arithmetic_v<X>&& std::is_arithmetic_v<Y>&& std::is_arithmetic_v<H>&& std::is_arithmetic_v<W>
        inline View(const View* parent_view,
                    const X x,
                    const Y y,
                    const W width,
                    const H height,
                    const avt::utils::RGBColor& bg_color = avt::config::DEFAULT_BACKGROUND) noexcept
            : p_view(parent_view),
              PosType(x, y),
              SizeType(width, height)
        {
            m_create_content();
        }

        /** @brief Value Constructor (1 pos + 1 size + 1 color). */
        inline View(const View* parent_view,
                    const avt::utils::Coords2D& top_left,
                    const avt::utils::Size& size,
                    const avt::utils::RGBColor& bg_color = avt::config::DEFAULT_BACKGROUND) noexcept
            : p_view(parent_view),
              PosType(top_left),
              SizeType(size)
        {
            m_create_content();
        }

        /** @brief Value Constructor (1 rect + 1 color). */
        inline View(const View* parent_view,
                    const avt::CVRect& rect,
                    const avt::utils::RGBColor& bg_color = avt::config::DEFAULT_BACKGROUND) noexcept
            : p_view(parent_view),
              PosType(rect.tl()),
              SizeType(rect.size())
        {
            m_create_content();
        }

        /** @brief Default Copy Constructor. */
        View(const View&) noexcept = delete; // default;

        /** @brief Default Move COnstructor. */
        View(View&&) noexcept = delete; //default;

        /** @brief Default Destructor. */
        ~View() noexcept = default;


        //---   Assignments   -----------------------------------------------
        /** @brief Default Copy assignment. */
        View& operator= (const View&) noexcept = delete; //default;

        /** @brief Default Move assignment. */
        View& operator= (View&&) noexcept = delete; //default;


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

        /** @brief Returns the avt::utils::Coords2D position of this view in its parent view. */
        inline avt::utils::Coords2D pos() noexcept
        {
            return *dynamic_cast<avt::utils::Coords2D*>(this);
        }

        /** @brief Returns the const avt::utils::Coords2D position of this view in its parent view. */
        inline avt::utils::Coords2D pos() const noexcept
        {
            return *dynamic_cast<const avt::utils::Coords2D*>(this);
        }

        /** @brief Returns the avt::utils::Size size of this view. */
        inline avt::utils::Size size() noexcept
        {
            return *dynamic_cast<avt::utils::Size*>(this);
        }

        /** @brief Returns the const avt::utils::Size size of this view. */
        inline avt::utils::Size size() const noexcept
        {
            return *dynamic_cast<const avt::utils::Size*>(this);
        }


        //---   Attributes   ------------------------------------------------
        const avt::gui::items::View* p_view{ nullptr };
        //avt::video::Frame            content;


    private:
        /** @brief Evaluates the clipped size of this view when displayed in a frame. */
        avt::utils::Size m_clipping_size(const avt::utils::Coords2D& abs_pos,
                                         const avt::utils::Size&     size,
                                         avt::video::Frame&          frame) const noexcept;

        /** @brief Internally creates the content for this view. */
        void m_create_content();

        /** @brief Evaluates the absolute position of this view within the root View. */
        avt::utils::Coords2D m_get_abs_pos(const View* p_current_view) const noexcept;
    };

}
