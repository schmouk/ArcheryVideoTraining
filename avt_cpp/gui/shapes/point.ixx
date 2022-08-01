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


export module gui.shapes.point;

import utils.coords2d;
import video.frame;
import utils.rgba_color;
import gui.shapes.shape;
import utils;


//===========================================================================
export namespace avt::gui::shapes
{
    //=======================================================================
    /** @brief The base class for all graphical shapes. */
    class Point : public avt::gui::shapes::Shape
    {
    public:
        //--- Wrappers ------------------------------------------------------
        using MyBaseType = avt::gui::shapes::Shape;


        //--- Constructors / Destructors ------------------------------------
        /** @brief Default constructor (1/7). */
        inline Point() noexcept
            : MyBaseType{}, radius{ 0 }
        {}

        /** @brief Constructor (2/7). */
        inline Point(const avt::utils::Coords2D&  _coords,
                     const avt::utils::RGBAColor& _color) noexcept
            : MyBaseType(_coords, _color), radius{ avt::Byte(0) }
        {}

        /** @brief Constructor (3/7). */
        template<typename R>
            requires std::is_arithmetic_v<R>
        inline Point(const avt::utils::Coords2D& _coords,
                     const avt::utils::RGBAColor& _color,
                     const R                      _radius) noexcept
            : MyBaseType(_coords, _color), radius{ avt::utils::clamp_s(_radius) }
        {}

        /** @brief Constructor (4/7). */
        template<typename P>
            requires avt::is_pair_type_v<P>
        inline Point(const P _pair, const avt::utils::RGBAColor& _color) noexcept(false)
            : MyBaseType(_pair, _color), radius{ avt::Byte(0) }
        {}

        /** @brief Constructor (5/7). */
        template<typename P, typename R>
            requires avt::is_pair_type_v<P> && std::is_arithmetic_v<R>
        inline Point(const P                      _pair,
                     const avt::utils::RGBAColor& _color,
                     const R                      _radius) noexcept(false)
            : MyBaseType(_pair, _color), radius{ _radius }
        {}

        /** @brief Constructor (6/7). */
        template<typename X, typename Y>
            requires std::is_arithmetic_v<X> && std::is_arithmetic_v<Y>
        inline Point(const X x, const Y y, const avt::utils::RGBAColor& _color) noexcept
            : MyBaseType(x, y, _color ), radius{ avt::Byte(0) }
        {}

        /** @brief Constructor (7/7). */
        template<typename X, typename Y, typename R>
            requires std::is_arithmetic_v<X> && std::is_arithmetic_v<Y> && std::is_arithmetic_v<R>
        inline Point(const X x, const Y y,
                     const avt::utils::RGBAColor& _color,
                     const avt::CoordsType        _radius) noexcept
            : MyBaseType(x, y, _color), radius{ _radius }
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