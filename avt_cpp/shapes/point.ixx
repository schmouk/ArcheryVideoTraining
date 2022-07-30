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
import utils.rgba_color;
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
        /** @brief Default constructor (1/7). */
        inline Point() noexcept
            : avt::shapes::Shape{}, radius{ 0 }
        {}

        /** @brief Constructor (2/7). */
        inline Point(const avt::utils::Coords2D&  _coords,
                     const avt::utils::RGBAColor& _color) noexcept
            : avt::shapes::Shape(_coords, _color), radius{ avt::Byte(0) }
        {}

        /** @brief Constructor (3/7). */
        template<typename R>
            requires std::is_arithmetic_v<R>
        inline Point(const avt::utils::Coords2D& _coords,
                     const avt::utils::RGBAColor& _color,
                     const R                      _radius) noexcept
            : avt::shapes::Shape(_coords, _color), radius{ avt::utils::clamp_s(_radius) }
        {}

        /** @brief Constructor (4/7). */
        template<typename P>
            requires avt::is_pair_type_v<P>
        inline Point(const P _pair, const avt::utils::RGBAColor& _color) noexcept(false)
            : avt::shapes::Shape(_pair, _color), radius{ avt::Byte(0) }
        {}

        /** @brief Constructor (5/7). */
        template<typename P, typename R>
            requires avt::is_pair_type_v<P> && std::is_arithmetic_v<R>
        inline Point(const P                      _pair,
                     const avt::utils::RGBAColor& _color,
                     const R                      _radius) noexcept(false)
            : avt::shapes::Shape(_pair, _color), radius{ _radius }
        {}

        /** @brief Constructor (6/7). */
        template<typename X, typename Y>
            requires std::is_arithmetic_v<X> && std::is_arithmetic_v<Y>
        inline Point(const X x, const Y y, const avt::utils::RGBAColor& _color) noexcept
            : avt::shapes::Shape(x, y, _color ), radius{ avt::Byte(0) }
        {}

        /** @brief Constructor (7/7). */
        template<typename X, typename Y, typename R>
            requires std::is_arithmetic_v<X> && std::is_arithmetic_v<Y> && std::is_arithmetic_v<R>
        inline Point(const X x, const Y y,
                     const avt::utils::RGBAColor& _color,
                     const avt::CoordsType        _radius) noexcept
            : avt::shapes::Shape(x, y, _color), radius{ _radius }
        {}

        /** @brief Default Copy constructor. */
        Point(const Point&) noexcept = default;

        /** @brief Default Move coonstructor. */
        Point(Point&&) noexcept(false) = default;

        /** @brief Default destructor. */
        virtual ~Point() noexcept = default;


        //---   Assignments   -----------------------------------------------
        /** @brief Default Copy assignment. */
        Point& operator= (const Point&) noexcept = default;

        /** @brief Default Move assignment. */
        Point& operator= (Point&&) noexcept = default;

        /** @brief Assignment from Coords2D. */
        inline Point& operator= (const avt::utils::Coords2D& pos) noexcept
        {
            x = pos.x;
            y = pos.y;
            return *this;
        }


        //---   Positive Moving   -------------------------------------------
        /** @brief In-place positive move by an offset. */
        inline Point& operator+= (const avt::utils::Coords2D& rhs) noexcept
        {
            x = avt::utils::clamp_s(_ConvertType(x) + _ConvertType(rhs.x));
            y = avt::utils::clamp_s(_ConvertType(y) + _ConvertType(rhs.y));
            return *this;
        }

        /** @brief In-place positive move by a 2-components container offset. */
        template<typename P>
            requires avt::is_pair_type_v<P>
        inline Point& operator+= (const P& rhs) noexcept(false)
        {
            return *this += avt::utils::Coords2D(rhs[0], rhs[1]);
        }

        /** @brief Positive move by a Coords2D offset (post-). */
        friend inline Point operator+ (Point lhs, const avt::utils::Coords2D& rhs) noexcept
        {
            return lhs += rhs;
        }

        /** @brief Positive move by a Coords2D offset (pre-). */
        friend inline Point operator+ (const avt::utils::Coords2D& lhs, Point rhs) noexcept
        {
            return rhs += lhs;
        }

        /** @brief Positive move by a 2-components container offset (post-). */
        template<typename P>
            requires avt::is_pair_type_v<P>
        friend inline Point operator+ (Point lhs, const P& rhs) noexcept(false)
        {
            return lhs += rhs;
        }

        /** @brief Positive move by a 2-components container offset (pre-). */
        template<typename P>
            requires avt::is_pair_type_v<P>
        friend inline Point operator+ (const P& lhs, Point rhs) noexcept(false)
        {
            return rhs += lhs;
        }


        //---   Negative Moving   -------------------------------------------
        /** @brief In-place negative move by an offset. */
        inline Point& operator-= (const avt::utils::Coords2D& rhs) noexcept
        {
            x = avt::utils::clamp_s(_ConvertType(x) - _ConvertType(rhs.x));
            y = avt::utils::clamp_s(_ConvertType(y) - _ConvertType(rhs.y));
            return *this;
        }

        /** @brief In-place negative move by a 2-components container offset. */
        template<typename P>
            requires avt::is_pair_type_v<P>
        inline Point& operator-= (const P& rhs) noexcept(false)
        {
            return *this -= avt::utils::Coords2D(rhs[0], rhs[1]);
        }

        /** @brief Negative move by an offset. */
        friend inline Point operator- (Point lhs, const avt::utils::Coords2D& rhs) noexcept
        {
            return lhs -= rhs;
        }

        /** @brief Negative move by a 2-components container offset (post-). */
        template<typename P>
            requires avt::is_pair_type_v<P>
        friend inline Point operator- (Point lhs, const P& rhs) noexcept(false)
        {
            return lhs -= rhs;
        }

        /** @brief Negative move by a 2-components container offset (pre-). */
        template<typename P>
            requires avt::is_pair_type_v<P>
        friend inline Point operator- (const P& lhs, const Point& rhs) noexcept(false)
        {
            using T = decltype(lhs[0]);
            return Point(lhs[0] - T(rhs.x), lhs[1] - T(rhs.y));
        }


        //---   Magnifying   ------------------------------------------------
        /** @brief In-place multiplies position (one single factor). */
        template<typename T>
            requires std::is_arithmetic_v<T>
        inline Point& operator*= (const T factor) noexcept
        {
            return *this = avt::utils::Coords2D(x, y) * factor;
        }

        /** @brief Mulitplies position by a factor (post-). */
        template<typename T>
            requires std::is_arithmetic_v<T>
        inline friend Point operator* (Point lhs, const T factor) noexcept
        {
            return lhs *= factor;
        }

        /** @brief Mulitplies position by a factor (pre-). */
        template<typename T>
            requires std::is_arithmetic_v<T>
        inline friend Point operator* (const T factor, Point rhs) noexcept
        {
            return rhs *= factor;
        }


        //---   Dividing   --------------------------------------------------
        /** @brief In-place divides position (one single factor). */
        template<typename T>
            requires std::is_arithmetic_v<T>
        inline Point operator/= (const T factor) noexcept(false)
        {
            assert(factor > 0);
            return *this *= 1.0 / double(factor);
        }

        /** @brief Divides position by a factor (post-). */
        template<typename T>
            requires std::is_arithmetic_v<T>
        inline friend Point operator/ (Point lhs, const T rhs) noexcept(false)
        {
            return lhs /= rhs;
        }


        //---   Operations   ------------------------------------------------
        /** @brief Draws this point in the specified frame. */
        virtual inline void draw(avt::video::Frame& frame)
        {
            draw(frame, radius);
        }

        /** @brief Draws this point in the specified frame with the specified radius. */
        inline void draw(avt::video::Frame& frame, const avt::CoordsType radius)
        {
            if (radius > 1)
                cv::circle(frame,
                    *this,
                    radius,
                    (cv::Scalar)color,
                    cv::FILLED,
                    cv::LINE_8,
                    0);
            else
                frame.at<cv::Vec3b>(y,x) = (cv::Vec3b)color;
        }


        //---     Attributes   ----------------------------------------------
        avt::CoordsType radius;
    };

}