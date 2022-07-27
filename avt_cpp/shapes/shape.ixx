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


//===========================================================================
export namespace avt::shapes
{
    //=======================================================================
    /** @brief The base class for all graphical shapes. */
    class Shape
    {
    public:
        //--- Constructors / Destructors ------------------------------------
        /** @brief Default constructor. */
        inline Shape() noexcept
        {}

        /** @brief Constructor (2 values). */
        inline Shape(const avt::utils::Coords2D& coords, const avt::utils::RGBColor& color) noexcept
            : base_coords{ coords },
              base_color{ color }
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
        virtual inline [[nodiscard]] void draw(avt::video::Frame&)
        {}

        /** @brief Relative move of the base coordinantes of this shape (2 offsets). */
        virtual inline void move(const CoordsType dx, const CoordsType dy)
        {
            base_coords += avt::utils::Coords2D(dx, dy);
        }

        /** @brief Relative move of the base coordinantes of this shape (2-components container). */
        template<typename P>
            requires avt::is_pair_type_v<P>
        inline void move(const P& rhs) noexcept(false)
        {
            base_coords += rhs;
        }

        /** @brief Absolute move of the base coordinantes of this shape (2 offsets). */
        virtual inline void move_at(const CoordsType new_x, const CoordsType new_y)
        {
            base_coords = avt::utils::Coords2D(new_x, new_y);
        }

        /** @brief Relative move of the base coordinantes of this shape (2-components container). */
        template<typename P>
            requires avt::is_pair_type_v<P>
        inline void move_at(const P& new_pos) noexcept(false)
        {
            base_coords = new_pos;
        }


        //---   Attributes   ------------------------------------------------
        avt::utils::Coords2D base_coords;
        avt::utils::RGBColor base_color;
    };

}