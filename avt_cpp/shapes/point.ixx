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

#include "utils/types.h"


export module shapes.point;

import utils.coords2d;
import video.frame;
import utils.rgb_color;
import shapes.shape;
import utils;


//===========================================================================
export namespace avt::shapes
{
    //=======================================================================
    /** @brief The base class for all graphical shapes. */
    class Point : public avt::shapes::Shape
    {
    public:
        //--- Constructors / Destructors ------------------------------------
        /** @brief Default constructor. */
        inline Point() noexcept
            : avt::shapes::Shape{}, radius{ 1 }
        {}

        /** @brief Constructor (2 values). */
        inline Point(const avt::utils::Coords2D& _coords,
                     const avt::utils::RGBColor& _color,
                     const avt::CoordsType       _radius = 1) noexcept
            : avt::shapes::Shape( _coords, _color ),
              radius{ _radius }
        {}

        /** @brief Constructor (2-components container + color). */
        template<typename P>
            requires avt::is_pair_type_v<P>
        inline Point(const P                     _pair,
                     const avt::utils::RGBColor& _color,
                     const avt::CoordsType       _radius = 1) noexcept(false)
            : avt::shapes::Shape( _pair, _color ),
              radius{ _radius }
        {}

        /** @brief Constructor (3 values). */
        template<typename X, typename Y>
            requires std::is_arithmetic_v<X>&& std::is_arithmetic_v<Y>
        inline Point(const X x, const Y y, 
                     const avt::utils::RGBColor& _color,
                     const avt::CoordsType       _radius = 1) noexcept
            : avt::shapes::Shape(x, y, _color ),
              radius{ _radius }
        {}

        /** @brief Default Copy constructor. */
        Point(const Point&) noexcept = default;

        /** @brief Default Move coonstructor. */
        Point(Point&&) noexcept(false) = default;

        /** @brief Default destructor. */
        virtual ~Point() noexcept = default;


        //---   Assignments   -----------------------------------------------
        /** @brief Default Copy assignment. */
        Point& operator=(const Point&) noexcept = default;

        /** @brief Default Move assignment. */
        Point& operator=(Point&&) noexcept = default;


        //---   Operations   ------------------------------------------------
        /** @brief Draws this point in the specified frame. */
        virtual inline [[nodiscard]] void draw(avt::video::Frame& frame)
        {
            cv::circle(frame,
                       *this,
                       radius,
                       avt::CVScalarByte(color),
                       cv::FILLED,
                       cv::LINE_8,
                       0);
        }

        /** @breif Draws this point in the specified frame with the specified radius. */
        inline void draw(avt::video::Frame& frame, const avt::CoordsType radius)
        {
            cv::circle(frame,
                       *this,
                       std::max<avt::CoordsType>(1, radius),
                       avt::CVScalarByte(color),
                       cv::FILLED,
                       cv::LINE_8,
                       0);
        }


        //---     Attributes   ----------------------------------------------
        avt::CoordsType radius;
    };

}