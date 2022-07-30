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
#include "utils/types.h"


export module shapes.line;

import utils.coords2d;
import video.frame;
import utils.rgba_color;
import shapes.shape;
import utils;


//===========================================================================
export namespace avt::shapes
{
    //=======================================================================
    /** @brief The class of graphical libes.
    *
    * Lines get a starting point, which coordinates are '.x' and '.y',
    * and an ending point which corrdinates are 'end.x' and 'end.y'.
    */
    class Line : public avt::shapes::Shape
    {
    public:
        //--- Wrappers ------------------------------------------------------
        using MyBaseType = avt::shapes::Shape;


        //--- Constructors / Destructors ------------------------------------
        /** @brief Default constructor. */
        inline Line() noexcept
            : MyBaseType{}, end{}, thickness{ 1 }
        {}

        /** @brief Constructor. */
        inline Line(const avt::utils::Coords2D&  _start,
                    const avt::utils::Coords2D&  _end,
                    const avt::utils::RGBAColor& _color,
                    const int                    _thickness = 1) noexcept
            : MyBaseType{ _start, _color }, end{ _end }, thickness{ _thickness }
        {}

        /** @brief Constructor). */
        template<typename P1, typename P2>
            requires avt::is_pair_type_v<P1> && avt::is_pair_type_v<P2>
        inline Line(const P1 _start, const P2 _end, const avt::utils::RGBAColor& _color) noexcept(false)
            : MyBaseType{ _start, _color }, end{ _end }, thickness{ 1 }
        {}

        /** @brief Constructor. */
        template<typename P1, typename P2>
            requires avt::is_pair_type_v<P1>&& avt::is_pair_type_v<P2>
        inline Line(const P1 _start, const P2 _end, const avt::utils::RGBAColor& _color, const int _thickness) noexcept(false)
            : MyBaseType{ _start, _color }, end{ _end }, thickness{ _thickness }
        {}

        /** @brief Default Copy constructor. */
        Line(const Line&) noexcept = default;

        /** @brief Default Move coonstructor. */
        Line(Line&&) noexcept(false) = default;

        /** @brief Default destructor. */
        virtual ~Line() noexcept = default;


        //---   Assignments   -----------------------------------------------
        /** @brief Default Copy assignment. */
        Line& operator=(const Line&) noexcept = default;

        /** @brief Default Move assignment. */
        Line& operator=(Line&&) noexcept = default;


        //---   Comparisons   -----------------------------------------------
        /** @brief Returns true if coords are the same, or false otherwise. */
        inline const bool operator==(const Line& rhs) const noexcept
        {
            return x == rhs.x && y == rhs.y && end.x == rhs.end.x && end.y == rhs.end.y && thickness == rhs.thickness;
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
            cv::line(frame, *this, end, (cv::Scalar)color, thickness, cv::LINE_8, 0);
        }

        /** @brief Relative move of the base coordinantes of this shape (2 offsets). */
        template<typename X, typename Y>
            requires std::is_arithmetic_v<X> && std::is_arithmetic_v<Y>
        void move(const X dx, const Y dy)
        {
            MyBaseType::move(dx, dy);  // moves start point
            end += {dx, dy};           // moves end point
        }

        /** @brief Absolute move of the base coordinates of this shape (2 offsets). */
        template<typename X, typename Y>
            requires std::is_arithmetic_v<X>&& std::is_arithmetic_v<Y>
        inline void move_at(const X new_x, const Y new_y)
        {
            move(new_x - x, new_y - y);
        }

        /** @brief Relative move of the base coordinates of this shape (2-components container). */
        template<typename P>
            requires avt::is_pair_type_v<P>
        inline void move_at(const P& new_pos) noexcept(false)
        {
            move_at(new_pos[0], new_pos[1]);
        }


        //---   Operators   -------------------------------------------------
        /** @brief Moves line by a 2D-coords offset. */
        inline Line& operator+= (const Coords2D& rhs) noexcept
        {
            move(rhs.x, rhs.y);
            return *this;
        }

        /** @brief Moves Line by a 2-components container offset. */
        template<typename P>
            requires avt::is_pair_type_v<P>
        friend inline Line operator+ (Line lhs, const P& rhs) noexcept(false)
        {
            return lhs += rhs;
        }

        /** @brief Moves line by a 2-components container offset. */
        template<typename P>
            requires avt::is_pair_type_v<P>
        friend inline Line operator+ (const P& lhs, Line rhs) noexcept(false)
        {
            return rhs += lhs;
        }

        /** @brief Moves line by a negative 2-components container. */
        template<typename P>
            requires avt::is_pair_type_v<P>
        friend inline Line operator- (Line lhs, const P& rhs) noexcept(false)
        {
            return lhs -= rhs;
        }


        /** @brief In-place multiplies (one single factor). */
        template<typename T>
            requires std::is_arithmetic_v<T>
        Line& operator*= (const T factor) noexcept
        {
            avt::utils::Coords2D start(x, y);
            start *= factor;
            x = start.x;
            y = start.y;
            end *= factor;
            return *this;
        }

        /** @brief Mulitplies by a factor (post). */
        template<typename T>
            requires std::is_arithmetic_v<T>
        inline friend Line operator* (Line lhs, const T factor) noexcept
        {
            return lhs *= factor;
        }

        /** @brief Mulitplies by a factor (pre-). */
        template<typename T>
            requires std::is_arithmetic_v<T>
        inline friend Line operator* (const T factor, Line rhs) noexcept
        {
            return rhs *= factor;
        }



        //---   Attributes   ------------------------------------------------
        avt::utils::Coords2D end;
        int thickness;
    };

}