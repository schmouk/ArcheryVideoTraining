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

#include "utils/types.h"

export module gui.shapes.shape;

import utils.coords2d;
import video.frame;
import utils.rgba_color;
import utils;


//===========================================================================
export namespace avt::gui::shapes
{
    //=======================================================================
    /** @brief The base class for all graphical shapes. */
    class Shape : public avt::utils::Coords2D
    {
    public:
        //--- Wrappers ------------------------------------------------------
        using MyBaseType = avt::utils::Coords2D;


        //--- Constructors / Destructors ------------------------------------
        /** @brief Default constructor. */
        inline Shape() noexcept
            : MyBaseType{}, color{}
        {}

        /** @brief Constructor (2 values). */
        inline Shape(const MyBaseType& _coords, const avt::utils::RGBAColor& _color) noexcept
            : MyBaseType{ _coords }, color{ _color }
        {}

        /** @brief Constructor (2-components container + color). */
        template<typename P>
            requires avt::is_pair_type_v<P>
        inline Shape(const P _pair, const avt::utils::RGBAColor& _color) noexcept(false)
            : MyBaseType{ _pair }, color{ _color }
        {}

        /** @brief Constructor (3 values). */
        template<typename X, typename Y>
            requires std::is_arithmetic_v<X> && std::is_arithmetic_v<Y>
        inline Shape(const X _x, const Y _y, const avt::utils::RGBAColor& _color) noexcept
            : MyBaseType( _x, _y ), color{ _color }
        {}

        /** @brief Default Copy constructor. */
        Shape(const Shape&) noexcept = default;

        /** @brief Default Move coonstructor. */
        Shape(Shape&&) noexcept(false) = default;

        /** @brief Default destructor. */
        virtual ~Shape() noexcept = default;


        //---   Assignments   -----------------------------------------------
        /** @brief Default Copy assignment. */
        Shape& operator=(const Shape&) noexcept = default;

        /** @brief Default Move assignment. */
        Shape& operator=(Shape&&) noexcept = default;

        /** @brief New position assignment. */
        Shape& operator=(const avt::utils::Coords2D& pos)
        {
            x = pos.x;
            y = pos.y;
            return *this;
        }


        //---   Operations   ------------------------------------------------
        /** @brief Draws this shape in the specified frame.
        *
        * In this base class, no drawing takes place. This
        * method MUST BE overriden in inheriting classes.
        */
        virtual inline void draw(avt::video::Frame&)
        {}


        //---   Attributes   ------------------------------------------------
        avt::utils::RGBAColor color;
    };

}