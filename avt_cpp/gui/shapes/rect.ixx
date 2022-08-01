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

#include <opencv2/imgproc.hpp>
#include <opencv2/core/types.hpp>

#include "utils/types.h"


export module gui.shapes.rect;

import utils.coords2d;
import video.frame;
import gui.shapes.line;
import utils.rgba_color;
import gui.shapes.shape;
import utils;


//===========================================================================
export namespace avt::gui::shapes
{
    //=======================================================================
    /** @brief The class of graphical rectangles.
    *
    * Rectangles are defined by their top-left and bottom-right corners  for
    * which coordinates are resp.: '.x', '.y' (top-left corner) and '.end.x' 
    * and 'end.y' (bottom-right corner).
    * Meanwhile,  accessors and mutators '.top_left()' and '.bottom_right()' 
    * give access to the related corner position.
    */
    class Rect : public avt::gui::shapes::Line
    {
    public:
        //--- Wrappers ------------------------------------------------------
        using MyBaseType = avt::gui::shapes::Line;


        //--- Constructors / Destructors ------------------------------------
        /** @brief Default constructor. */
        inline Rect() noexcept
            : MyBaseType{}
        {}

        /** @brief Constructor. */
        inline Rect(const avt::utils::Coords2D& top_left,
                    const avt::utils::Coords2D& bottom_right,
                    const avt::utils::RGBAColor& _color,
                    const int                    _thickness = 1) noexcept
            : MyBaseType{ top_left, bottom_right, _color, _thickness }
        {}

        /** @brief Constructor. */
        template<typename P1, typename P2>
            requires avt::is_pair_type_v<P1>&& avt::is_pair_type_v<P2>
        inline Rect(const P1 top_left,
                    const P2 bottom_right,
                    const avt::utils::RGBAColor& _color) noexcept(false)
            : MyBaseType{ top_left, bottom_right, _color, 1 }
        {}

        /** @brief Constructor. */
        template<typename P1, typename P2>
            requires avt::is_pair_type_v<P1>&& avt::is_pair_type_v<P2>
        inline Rect(const P1 top_left,
                    const P2 bottom_right,
                    const avt::utils::RGBAColor& _color,
                    const int _thickness) noexcept(false)
            : MyBaseType{ top_left, bottom_right, _color, _thickness }
        {}

        /** @brief Constructor from line. */
        inline explicit Rect(const avt::gui::shapes::Line& line) noexcept
            : MyBaseType(line)
        {}

        /** @brief Default Copy constructor. */
        Rect(const Rect&) noexcept = default;

        /** @brief Default Move coonstructor. */
        Rect(Rect&&) noexcept(false) = default;

        /** @brief Default destructor. */
        virtual ~Rect() noexcept = default;


        //---   Accesors / Mutators   ---------------------------------------
        /** @brief Accessor to top-left corner. */
        inline const avt::utils::Coords2D top_left() const noexcept
        {
            return avt::utils::Coords2D{ x, y };
        }

        /** @brief Accessor to bottom-right corner. */
        inline const avt::utils::Coords2D& bottom_right() const noexcept
        {
            return end;
        }

        /** @brief "Mutator" on top-left corner (two scalar coordinates). */
        template<typename X, typename Y>
            requires std::is_arithmetic_v<X> && std::is_arithmetic_v<Y>
        inline void top_left(const X new_x, const Y new_y)
        {
            x = new_x;
            y = new_y;
        }

        /** @brief "Mutator" on top-left corner (one 2D-coords). */
        inline void top_left(const avt::utils::Coords2D& new_pos)
        {
            x = new_pos.x;
            y = new_pos.y;
        }

        /** @brief "Mutator" on top-left corner (one pair of coordinates). */
        template<typename P>
            requires avt::is_pair_type_v<P>
        inline void top_left(const P& new_pos)
        {
            x = new_pos[0];
            y = new_pos[1];
        }

        /** @brief Mutator on bottom-right corner. */
        inline avt::utils::Coords2D& bottom_right() noexcept
        {
            return end;
        }


        //---   Assignments   -----------------------------------------------
        /** @brief Default Copy assignment. */
        Rect& operator=(const Rect&) noexcept = default;

        /** @brief Default Move assignment. */
        Rect& operator= (Rect&&) noexcept = default;

        /** @brief New position assignment. */
        Rect& operator= (const avt::utils::Coords2D& new_pos) noexcept
        {
            move_at(new_pos.x, new_pos.y);
            return *this;
        }

        /** @brief New position assignment. */
        template<typename P>
            requires avt::is_pair_type_v<P>
        Rect& operator= (const P& new_pos) noexcept
        {
            move_at(new_pos[0], new_pos[1]);
            return *this;
        }

        /** @brief Line reference assignment. */
        Rect& operator= (const avt::gui::shapes::Line& line) noexcept
        {
            return *this = avt::gui::shapes::Rect(line);
        }


        //---   Casts   -----------------------------------------------------
        /** @brief Casts to cv::Rect_<short>. */
        inline operator avt::CVRect() noexcept
        {
            return avt::CVRect(avt::CVPoint{ x, y }, avt::CVPoint{ end.x, end.y });
        }


        //---   Operations   ------------------------------------------------
        /** @brief Draws this line in the specified frame. */
        virtual inline void draw(avt::video::Frame& frame)
        {
            draw(frame, thickness);
        }

        /** @brief Draws this line in the specified frame with the specified radius. */
        inline void draw(avt::video::Frame& frame, const int thickness)
        {
            cv::rectangle(frame, *this, end, (cv::Scalar)color, thickness, cv::LINE_8, 0);
        }
    };

}