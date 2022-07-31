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

#include <opencv2/core/types.hpp>

#include "utils/types.h"


export module utils.coords2d;

import utils;


//===========================================================================
export namespace avt::utils
{
    //=======================================================================
    /** @brief The class of 2D short integer coordinates. */
    class Coords2D : public cv::Point_<avt::CoordsType>
    {
    public:

        //-------------------------------------------------------------------
        using MyBaseType = cv::Point_<avt::CoordsType>; //!< wrapper to the base class


        //--- Constructors / Destructors ------------------------------------
        /** @brief Default constructor. */
        inline Coords2D() noexcept
            : MyBaseType{}
        {}

        /** @brief Constructor (2 values). */
        template<typename X, typename Y>
            requires std::is_arithmetic_v<X> && std::is_arithmetic_v<Y>
        inline Coords2D(const X x, const Y y) noexcept
            : MyBaseType{ avt::utils::clamp<MyBaseType::value_type, X>(x),
                          avt::utils::clamp<MyBaseType::value_type, Y>(y) }
        {}

        /** @brief Constructor (2-components container). */
        template<typename P>
            requires avt::is_pair_type_v<P>
        inline Coords2D(const P pair) noexcept(false)
            : MyBaseType{ avt::utils::clamp<MyBaseType::value_type, decltype(pair[0])>(pair[0]),
                          avt::utils::clamp<MyBaseType::value_type, decltype(pair[1])>(pair[1])}
        {}

        /** @brief Default Copy constructor. */
        Coords2D(const Coords2D&) noexcept = default;

        /** @brief Default Move coonstructor. */
        Coords2D(Coords2D&&) noexcept(false) = default;

        /** @brief Default destructor. */
        virtual ~Coords2D() noexcept = default;


        //---   Assignments   -----------------------------------------------
        /** @brief Default Copy assignment. */
        Coords2D& operator=(const Coords2D&) noexcept = default;

        /** @brief Default Move assignment. */
        Coords2D& operator=(Coords2D&&) noexcept = default;

        /** @brief Copy Assignment (2-components container). */
        template<typename P>
            requires avt::is_pair_type_v<P>
        Coords2D& operator= (const P& pair) noexcept
        {
            x = avt::utils::clamp<MyBaseType::value_type, decltype(pair[0])>(pair[0]);
            y = avt::utils::clamp<MyBaseType::value_type, decltype(pair[1])>(pair[1]);
            return *this;
        }


        //---   Comparisons   -----------------------------------------------
        /** @brief Returns true if coords are the same, or false otherwise. */
        inline const bool operator==(const Coords2D& rhs) const noexcept
        {
            return x == rhs.x && y == rhs.y;
        }

        /** @brief Returns true if coords are the same, or false otherwise. */
        template<typename P>
            requires avt::is_pair_type_v<P>
        inline const bool operator==(const P& rhs) const noexcept
        {
            return x == rhs[0] && y == rhs[1];
        }

        /** @brief Returns true if coords are not the same, or false otherwise. */
        inline const bool operator!=(const Coords2D& rhs) const noexcept
        {
            return !(*this == rhs);
        }

        /** @brief Returns true if coords are not the same, or false otherwise. */
        template<typename P>
            requires avt::is_pair_type_v<P>
        inline const bool operator!=(const P& rhs) const noexcept
        {
            return !(*this == rhs);
        }


        //---   Moving   ----------------------------------------------------
        /** @brief Relative move of this position (two scalar offsets). */
        template<typename X, typename Y>
            requires std::is_arithmetic_v<X> && std::is_arithmetic_v<Y>
        inline void move(const X dx, const Y dy) noexcept
        {
            x = avt::utils::clamp_s(_ConvertType(x) + dx);
            y = avt::utils::clamp_s(_ConvertType(y) + dy);
        }

        /** @brief Relative move of this position (one Coords2D offset). */
        inline void move(const avt::utils::Coords2D& offset) noexcept
        {
            move(offset.x, offset.y);
        }

        /** @brief Relative move of this position (one 2-D container offset). */
        template<typename P>
            requires avt::is_pair_type_v<P>
        inline void move(const P& offset) noexcept
        {
            move(offset.x, offset.y);
        }

        /** @brief Absolute move of this position (two scalar new coordinates). */
        template<typename X, typename Y>
            requires std::is_arithmetic_v<X>&& std::is_arithmetic_v<Y>
        inline void move_at(const X new_x, const Y new_y)
        {
            x = avt::utils::clamp_s(new_x);
            y = avt::utils::clamp_s(new_y);
        }

        /** @brief Absolute move of this position (one Coords2D offset). */
        inline void move_at(const avt::utils::Coords2D& offset) noexcept
        {
            move_at(offset.x, offset.y);
        }

        /** @brief Absolute move of this position (one 2-D container offset). */
        template<typename P>
            requires avt::is_pair_type_v<P>
        inline void move_at(const P& offset) noexcept
        {
            move_at(offset.x, offset.y);
        }


        //---   Adding   ----------------------------------------------------
        /** @brief In-place adds a 2D-coords. */
        inline Coords2D& operator+= (const Coords2D& rhs) noexcept
        {
            move(rhs);
            return *this;
        }

        /** @brief In-place adds a 2-components container. */
        template<typename P>
            requires avt::is_pair_type_v<P>
        inline Coords2D& operator+= (const P& rhs) noexcept(false)
        {
            return *this += Coords2D(rhs[0], rhs[1]);
        }

        /** @brief Adds Coords2D + Coords2D. */
        friend inline Coords2D operator+ (Coords2D lhs, const Coords2D& rhs) noexcept
        {
            return lhs += rhs;
        }

        /** @brief Adds Coords2D + a 2-components container. */
        template<typename P>
            requires avt::is_pair_type_v<P>
        friend inline Coords2D operator+ (Coords2D lhs, const P& rhs) noexcept(false)
        {
            return lhs += rhs;
        }

        /** @brief Adds a 2-components container + Coords2D. */
        template<typename P>
            requires avt::is_pair_type_v<P>
        friend inline Coords2D operator+ (const P& lhs, Coords2D rhs) noexcept(false)
        {
            return rhs += lhs;
        }


        //---   Subtracting   -----------------------------------------------
        /** @brief In-place subtracts a 2D-components. */
        inline Coords2D& operator-= (const Coords2D& rhs) noexcept
        {
            move(-rhs.x, -rhs.y);
            return *this;
        }

        /** @brief In-place subtracts a 2-components container. */
        template<typename P>
            requires avt::is_pair_type_v<P>
        inline Coords2D& operator-= (const P& rhs) noexcept(false)
        {
            return *this -= Coords2D(rhs[0], rhs[1]);
        }

        /** @brief subtracts Coords2D - Coords2D. */
        friend inline Coords2D operator- (Coords2D lhs, const Coords2D& rhs) noexcept
        {
            return lhs -= rhs;
        }

        /** @brief subtracts Coords2D - a 2-components container. */
        template<typename P>
            requires avt::is_pair_type_v<P>
        friend inline Coords2D operator- (Coords2D lhs, const P& rhs) noexcept(false)
        {
            return lhs -= rhs;
        }

        /** @brief subtracts a 2-components container - Coords2D. */
        template<typename P>
            requires avt::is_pair_type_v<P>
        friend inline Coords2D operator- (const P& lhs, const Coords2D& rhs) noexcept(false)
        {
            using T = decltype(lhs[0]);
            return Coords2D(lhs[0] - T(rhs.x), lhs[1] - T(rhs.y));
        }


        //---   Magnifying   ------------------------------------------------
        /** @brief In-place multiplies (one single factor). */
        template<typename T>
            requires std::is_arithmetic_v<T>
        Coords2D& operator*= (const T factor) noexcept
        {
            const long xl = std::lround(double(x) * double(factor));
            const long yl = std::lround(double(y) * double(factor));
            move_at(xl, yl);
            return *this;
        }

        /** @brief Mulitplies by a factor (post). */
        template<typename T>
            requires std::is_arithmetic_v<T>
        inline friend Coords2D operator* (Coords2D lhs, const T factor) noexcept
        {
            return lhs *= factor;
        }

        /** @brief Mulitplies by a factor (pre-). */
        template<typename T>
            requires std::is_arithmetic_v<T>
        inline friend Coords2D operator* (const T factor, Coords2D rhs) noexcept
        {
            return rhs *= factor;
        }


        //---   Dividing   --------------------------------------------------
        /** @brief In-place divides (one single factor). */
        template<typename T>
            requires std::is_arithmetic_v<T>
        inline Coords2D operator/= (const T factor) noexcept(false)
        {
            assert(factor > 0);
            return *this *= 1.0 / double(factor);
        }

        /** @brief Divides by a factor (post-). */
        template<typename T>
            requires std::is_arithmetic_v<T>
        inline friend Coords2D operator/ (const Coords2D lhs, const T rhs) noexcept(false)
        {
            Coords2D tmp{ lhs };
            return tmp /= rhs;
        }


    protected:
        using _ConvertType = long;  //!< type for the conversion of coordinates on internal operations.

    };

}