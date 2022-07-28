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

export module shapes.shape;

import utils.coords2d;
import video.frame;
import utils.rgb_color;
import utils;


//===========================================================================
export namespace avt::shapes
{
    //=======================================================================
    /** @brief The base class for all graphical shapes. */
    class Shape : public avt::utils::Coords2D
    {
    public:
        //--- Constructors / Destructors ------------------------------------
        /** @brief Default constructor. */
        inline Shape() noexcept
            : avt::utils::Coords2D{},
              color{},
              transparency{ avt::Byte(255) }
        {}

        /** @brief Constructor (2 values). */
        template<typename T = avt::Byte>
            requires std::is_arithmetic_v<T>
        inline Shape(const avt::utils::Coords2D& _coords,
                     const avt::utils::RGBColor& _color,
                     const T                     _transparency = T(255)) noexcept
            : avt::utils::Coords2D{ _coords },
              color{ _color },
              transparency{ avt::utils::clamp_b<T>(_transparency) }
        {}

        /** @brief Constructor (2-components container + color). */
        template<typename P, typename T = avt::Byte>
            requires avt::is_pair_type_v<P> && std::is_arithmetic_v<T>
        inline Shape(const P                     _pair,
                     const avt::utils::RGBColor& _color,
                     const T                     _transparency = T(255)) noexcept(false)
            : avt::utils::Coords2D{ _pair },
              color{ _color },
              transparency{ avt::utils::clamp_b<T>(_transparency) }
        {}

        /** @brief Constructor (3 values). */
        template<typename X, typename Y>
            requires std::is_arithmetic_v<X> && std::is_arithmetic_v<Y>
        inline Shape(const X _x, const Y _y,
                     const avt::utils::RGBColor& _color,
                     const avt::Byte _transparency = 255) noexcept
            : avt::utils::Coords2D( _x, _y ),
              color{ _color },
              transparency{ _transparency }
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


        //---   Operations   ------------------------------------------------
        /** @brief Draws this shape in the specified frame.
        *
        * In this base class, no drawing takes place. This
        * method MUST BE overriden in inheriting classes.
        */
        virtual inline void draw(avt::video::Frame&)
        {}

        /** @brief Relative move of the base coordinantes of this shape (2 offsets). */
        template<typename X, typename Y>
            requires std::is_arithmetic_v<X> && std::is_arithmetic_v<Y>
        inline void move(const X dx, const Y dy)
        {
            *this += avt::utils::Coords2D(dx, dy);
        }

        /** @brief Relative move of the base coordinantes of this shape (2-components container). */
        template<typename P>
            requires avt::is_pair_type_v<P>
        inline void move(const P& offset) noexcept(false)
        {
            *this += offset;
        }

        /** @brief Absolute move of the base coordinantes of this shape (2 offsets). */
        template<typename X, typename Y>
            requires std::is_arithmetic_v<X> && std::is_arithmetic_v<Y>
        inline void move_at(const X new_x, const Y new_y)
        {
            x = avt::utils::clamp<MyBaseType::value_type, X>(new_x);
            y = avt::utils::clamp<MyBaseType::value_type, Y>(new_y);

        }

        /** @brief Relative move of the base coordinantes of this shape (2-components container). */
        template<typename P>
            requires avt::is_pair_type_v<P>
        inline void move_at(const P& new_pos) noexcept(false)
        {
            x = avt::utils::clamp<MyBaseType::value_type, decltype(new_pos[0])>(new_pos[0]);
            y = avt::utils::clamp<MyBaseType::value_type, decltype(new_pos[1])>(new_pos[1]);
        }


        //---   Attributes   ------------------------------------------------
        avt::utils::RGBColor color;
        avt::Byte            transparency;
    };

}