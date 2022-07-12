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


//===========================================================================
export module utils.size;

import utils;

//===========================================================================
export namespace avt::utils
{
    //=======================================================================
    /** @brief The class of 2D sizes.
    */
    class Size : public cv::Size_<avt::SizeValueType>
        /* Notice: Due to inheritance, gets access to
        *  - double aspectRatio () const   (i.e. width/height)
        *  - bool   empty () const
        */
    {
    public:

        using MyBaseType = cv::Size_<avt::SizeValueType>;  //!< wrapper to the base class.
        using ValueType = unsigned short;                  //!< wrapper to the width and height type.

        //---   Constructors / Destructor   ---------------------------------
        /** @brief Empty constructor. */
        inline Size() noexcept
            : MyBaseType{}
        {}

        /** @brief Constructor by values (width, height). */
        template<typename X, typename Y>
            requires std::is_arithmetic_v<X> && std::is_arithmetic_v<Y>
        inline Size(const X width, const Y height) noexcept
            : MyBaseType{ avt::utils::clamp<MyBaseType::value_type, X>(width),
                          avt::utils::clamp<MyBaseType::value_type, Y>(height) }
        {}

        /** @brief Constructor (2-components containers). */
        template<typename P>
            requires avt::is_pair_type_v<P>
        inline Size(const P &pair) noexcept
            : MyBaseType{ avt::utils::clamp_us(pair[0]), avt::utils::clamp_us(pair[1]) }
        {}

        /** @brief Default Copy Constructor. */
        Size(const Size&) noexcept = default;

        /** @brief Default Move COnstructor. */
        Size(Size&&) noexcept(false) = default;

        /** @brief Default Destructor. */
        virtual ~Size() noexcept = default;


        //---   Assignments   -----------------------------------------------
        /** @brief Default Copy assignment. */
        inline Size& operator=(const Size&) noexcept = default;

        /** @brief Default Move assignment. */
        inline Size& operator=(Size&&) noexcept = default;

        /** @brief Assignment operator (2-components container). */
        template<typename P>
            requires avt::is_pair_type_v<P>
        inline Size& operator= (const P& rhs) noexcept(false)
        {
            width = avt::utils::clamp<MyBaseType::value_type, decltype(rhs[0])>(rhs[0]);
            height = avt::utils::clamp<MyBaseType::value_type, decltype(rhs[1])>(rhs[1]);
            return *this;
        }

        //---   Comparisons   -----------------------------------------------
        /** @brief Returns true if sizes are the same, or false otherwise. */
        inline const bool operator== (const Size& rhs) const noexcept
        {
            return width == rhs.width && height == rhs.height;
        }

        /** @brief Returns true if this size and 2-components container are the ame, or false otherwise. */
        template<typename P>
            requires avt::is_pair_type_v<P>
        inline const bool operator== (const P& rhs) const noexcept(false)
        {
            width == rhs[0];
            height == rhs[1];
        }

        /** @brief Returns true if sizes are not the same, or false otherwise. */
        inline const bool operator!= (const Size& rhs) const noexcept
        {
            return !(*this == rhs);
        }

        /** @brief Returns true if this size and 2-components container are not the ame, or false otherwise. */
        template<typename P>
            requires avt::is_pair_type_v<P>
        inline const bool operator!= (const P& rhs) const noexcept(false)
        {
            return !(*this == rhs);
        }


        //---   Adding   ----------------------------------------------------
        /** @brief In-place adds a 2D-coords. */
        inline Size& operator+= (const Size& rhs) noexcept
        {
            width = avt::utils::clamp_us(_ConvertType(width) + _ConvertType(rhs.width));
            height = avt::utils::clamp_us(_ConvertType(height) + _ConvertType(rhs.height));
            return *this;
        }

        /** @brief In-place adds a 2-components container. */
        template<typename P>
            requires avt::is_pair_type_v<P>
        Size& operator+= (const P& rhs) noexcept(false)
        {
            using T = decltype(rhs[0]);
            if (std::is_integral_v<T>) {
                width = avt::utils::clamp_us(_ConvertType(width) + _ConvertType(rhs[0]));
                height = avt::utils::clamp_us(_ConvertType(height) + _ConvertType(rhs[1]));
            }
            else {
                const T w = rhs[0] + (rhs[0] >= T(0) ? T(0.5) :T(-0.5));
                width = avt::utils::clamp_us(_ConvertType(width) + _ConvertType(w));
                const T h = rhs[1] + (rhs[1] >= T(0) ? T(0.5) : T(-0.5));
                height = avt::utils::clamp_us(_ConvertType(height) + _ConvertType(h));
            }
            return *this;
        }

        /** @brief Adds Size + Size. */
        friend inline Size operator+ (Size lhs, const Size& rhs) noexcept
        {
            return lhs += rhs;
        }

        /** @brief Adds Size + a 2-components container. */
        template<typename P>
            requires avt::is_pair_type_v<P>
        friend inline Size operator+ (Size lhs, const P& rhs) noexcept(false)
        {
            return lhs += rhs;
        }

        /** @brief Adds a 2-components container + Size. */
        template<typename P>
            requires avt::is_pair_type_v<P>
        friend inline Size operator+ (const P& lhs, Size rhs) noexcept(false)
        {
            return rhs += lhs;
        }


        //---   Subtracting   -----------------------------------------------
        /** @brief In-place subtracts a 2D-components. */
        inline Size& operator-= (const Size& rhs) noexcept
        {
            width = avt::utils::clamp_us(_ConvertType{ width } - _ConvertType{ rhs.width });
            height = avt::utils::clamp_us(_ConvertType{ height } - _ConvertType{ rhs.height });
            return *this;
        }

        /** @brief In-place subtracts a 2-components container. */
        template<typename P>
            requires avt::is_pair_type_v<P>
        Size& operator-= (const P& rhs) noexcept(false)
        {
            using T = decltype(rhs[0]);
            if (std::is_integral_v<T>) {
                width = avt::utils::clamp_us(_ConvertType(width) - _ConvertType(rhs[0]));
                height = avt::utils::clamp_us(_ConvertType(height) - _ConvertType(rhs[1]));
            }
            else {
                const T w = rhs[0] + (rhs[0] >= T(0) ? T(0.5) : T(-0.5));
                width = avt::utils::clamp_us(_ConvertType(width) - _ConvertType(w));
                const T h = rhs[1] + (rhs[1] >= T(0) ? T(0.5) : T(-0.5));
                height = avt::utils::clamp_us(_ConvertType(height) - _ConvertType(h));
            }
            return *this;
        }

        /** @brief subtracts Size - Size. */
        friend inline Size operator- (Size lhs, const Size& rhs) noexcept
        {
            return lhs -= rhs;
        }

        /** @brief subtracts Size - a 2-components container. */
        template<typename P>
            requires avt::is_pair_type_v<P>
        friend inline Size operator- (Size lhs, const P& rhs) noexcept(false)
        {
            return lhs -= rhs;
        }

        /** @brief subtracts a 2-components container - Size. */
        template<typename P>
            requires avt::is_pair_type_v<P>
        friend inline Size operator- (const P& lhs, const Size& rhs) noexcept(false)
        {
            return Size(lhs) -= rhs;
        }


        //---   Magnifying   ------------------------------------------------
        /** @brief In-place multiplies (one single factor). */
        template<typename T>
            requires std::is_arithmetic_v<T>
        Size& operator*= (const T factor) noexcept
        {
            const long xf = std::lround(double(width) * double(factor));
            const long yf = std::lround(double(height) * double(factor));
            width = avt::utils::clamp<MyBaseType::value_type, long>(xf);
            height = avt::utils::clamp<MyBaseType::value_type, long>(yf);
            return *this;
        }

        /** @brief Mulitplies by a factor (post). */
        template<typename T>
            requires std::is_arithmetic_v<T>
        inline friend Size operator* (const Size& lhs, const T factor) noexcept
        {
            Size tmp{ lhs };
            return tmp *= factor;
        }

        /** @brief Mulitplies by a factor (pre-). */
        template<typename T>
            requires std::is_arithmetic_v<T>
        inline friend Size operator* (const T factor, const Size& rhs) noexcept
        {
            return rhs * factor;
        }


        //---   Dividing   --------------------------------------------------
        /** @brief In-place divides (one single factor). */
        template<typename T>
            requires std::is_arithmetic_v<T>
        inline Size operator/= (const T factor) noexcept(false)
        {
            assert(factor > 0);
            return *this *= 1.0 / factor;;
        }

        /** @brief Divides by a factor (post-). */
        template<typename T>
            requires std::is_arithmetic_v<T>
        inline friend Size operator/ (const Size lhs, const T rhs) noexcept(false)
        {
            Size tmp{ lhs };
            return tmp /= rhs;
        }


    private:
        using _ConvertType = long;  //!< type for the conversion of coordinates on internal operations.

        //---   Miscelaneous   ----------------------------------------------
        /** @brief Evaluates the area of this Size. */
        const unsigned long area() const
        {
            return (unsigned long)width * (unsigned long)height;
        }

    };

}
