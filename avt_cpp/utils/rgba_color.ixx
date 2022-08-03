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
#include <array>
#include <cassert>
#include <format>
#include <string>
#include <vector>

#include <opencv2/core/matx.hpp>
#include <opencv2/core/types.hpp>

#include "utils/types.h"

export module utils.rgba_color;

import utils;
import utils.rgb_color;


//===========================================================================
export namespace avt::utils
{
    //=======================================================================
    /** @brief The class of RGBA components colors.
    *
    * Library OpenCV uses a default BGR coding for colors which is not the
    * usual RGB one.  To help developments, class RGBColor codes colors in
    * a usual way while the BGR conversion is internally automated for its
    * use with library OpenCV.
    */
    class RGBAColor : public avt::utils::RGBColor
    {
    public:
        //---   Class data definition   ------------------------------------
        static constexpr avt::Byte FULL_OPAQUE       = avt::Byte(255);
        static constexpr avt::Byte FULL_TRANSPARENCY = avt::Byte(0);

        //---   Constructors / Destructor   ---------------------------------
        /** @brief Default constructor. */
        inline RGBAColor() noexcept
            : avt::utils::RGBColor{}, a{ avt::Byte(255) }
        {}

        /** @brief Constructor (from RGBColor). */
        inline RGBAColor(const avt::utils::RGBColor& rgb_color)
            : avt::utils::RGBColor(rgb_color), a{ avt::Byte(255) }
        {}

        /** @brief Valued Constructor (R, G, B bytes). */
        template<typename T, typename U, typename V>
            requires std::is_arithmetic_v<T> && std::is_arithmetic_v<U> && std::is_arithmetic_v<V>
        inline RGBAColor(const T r, const U g, const V b) noexcept
            : avt::utils::RGBColor(r, g, b), a{ avt::Byte(avt::utils::RGBAColor::FULL_OPAQUE) }
        {}

        /** @brief Valued Constructor (R, G, B, A bytes). */
        template<typename T, typename U, typename V, typename A>
            requires std::is_arithmetic_v<T>&& std::is_arithmetic_v<U>&& std::is_arithmetic_v<V>&& std::is_arithmetic_v<A>
        inline RGBAColor(const T r, const U g, const V b, const A a) noexcept
            : avt::utils::RGBColor(r, g, b), a{ avt::utils::clamp_b(a) }
        {}

        /** @brief Valued Constructor (one gray value). */
        template<typename T, typename A>
            requires std::is_arithmetic_v<T> && std::is_arithmetic_v<A>
        inline RGBAColor(const T value, const A a = A(255)) noexcept
            : avt::utils::RGBColor(value), a{ avt::utils::clamp_b(a) }
        {}

        /** @brief Valued Constructor (one 4-bytes buffer). */
        template<typename T>
            requires std::is_arithmetic_v<T>
        inline RGBAColor(const T buffer[4]) noexcept
            : avt::utils::RGBColor(buffer[0], buffer[1], buffer[2]), a{ avt::utils::clamp_b(buffer[3]) }
        {}

        /** @brief Valued Constructor (std::vector). */
        template<typename T>
            requires std::is_arithmetic_v<T>
        inline RGBAColor(const std::vector<T>& vect) noexcept(false)
            : avt::utils::RGBColor(vect), a{ avt::utils::RGBAColor::FULL_OPAQUE }
        {
            if (vect.size() > 3)
                a = _clipped(vect[3]);
        }

        /** @brief Valued Constructor (std::array). */
        template<typename T>
            requires std::is_arithmetic_v<T>
        inline RGBAColor(const std::array<T, 3>& arr) noexcept
            : avt::utils::RGBColor(arr), a{ avt::utils::RGBAColor::FULL_OPAQUE }
        {}

        /** @brief Valued Constructor (std::array). */
        template<typename T>
            requires std::is_arithmetic_v<T>
        inline RGBAColor(const std::array<T, 4>& arr) noexcept
            : avt::utils::RGBColor(arr[0], arr[1], arr[2]), a{ avt::utils::clamp_b(arr[3]) }
        {}

        /** @brief Copy and Move constructors. */
        RGBAColor(const RGBAColor&) noexcept = default;
        RGBAColor(RGBAColor&&) noexcept = default;

        /** @brief Destructor. */
        virtual ~RGBAColor() noexcept = default;


        //---   Casting operators   -----------------------------------------
        /** @brief Returns a string describing this color components (hexadecimal form). */
       virtual inline operator std::string() const
        {
            return std::format("#{:02X}{:02X}{:02X}{:02X}", r, g, b, a);
        }

        /** @brief Casts this RGBA color to a cv::Scalar (i.e. 4 doubles). */
        virtual inline operator cv::Scalar()
        {
            return cv::Scalar(double(b), double(g), double(r), double(a));
        }

        /** @brief Casts this RGBA color to a cv::Scalar_<avt::Byte> (i.e. 4 bytes). */
        virtual inline operator avt::CVScalarByte()
        {
            return avt::CVScalarByte(b, g, r, a);
        }

        /** @brief Casts this RGBA color to a const cv::Scalar_<avt::Byte> (i.e. 4 bytes). */
        virtual inline operator avt::CVScalarByte() const
        {
            return avt::CVScalarByte(b, g, r, a);
        }


        //---   Assignments   -----------------------------------------------
        /** @brief Copy assignment. */
        virtual RGBAColor& operator= (const RGBAColor&) noexcept = default;

        /** @brief Move assignment. */
        virtual RGBAColor& operator= (RGBAColor&&) noexcept = default;

        /** @brief Assignment operator (one 3-bytes buffer). */
        template<typename T>
            requires std::is_arithmetic_v<T>
        inline RGBAColor& operator= (const T buffer[4]) noexcept
        {
            set(buffer);
            return *this;
        }

        /** @brief Assignment operator (std::vector). */
        template<typename T>
            requires std::is_arithmetic_v<T>
        inline RGBAColor& operator= (const std::vector<T>& vect) noexcept(false)
        {
            set(vect);
            return *this;
        }

        /** @brief Assignment operator (std::array). */
        template<typename T>
            requires std::is_arithmetic_v<T>
        inline RGBAColor& operator= (const std::array<T, 4>& arr) noexcept
        {
            set(arr);
            return *this;
        }


        //---   Set color   -------------------------------------------------
        /** @brief Sets this color to BLACK. */
        virtual inline void clr() noexcept
        {
            r = g = b = a = avt::Byte{ 0 };
        }

        /** @brief Sets color (copy). */
        virtual inline void set(const RGBAColor& copy) noexcept
        {
            r = copy.r;
            g = copy.g;
            b = copy.b;
            a = copy.a;
        }

        /** @brief Sets color (four R, G, B, A bytes). */
        template<typename T, typename U, typename V, typename A>
            requires std::is_arithmetic_v<T> && std::is_arithmetic_v<U> && std::is_arithmetic_v<V> && std::is_arithmetic_v<A>
        inline void set(const T r_, const U g_, const V b_, const A a_) noexcept
        {
            r = _clipped(r_);
            g = _clipped(g_);
            b = _clipped(b_);
            a = _clipped(a_);
        }

        /** @brief Sets color (one 4-bytes buffer). */
        template<typename T>
            requires std::is_arithmetic_v<T>
        inline void set(const T buffer[4]) noexcept
        {
            set(buffer[0], buffer[1], buffer[2], buffer[3]);
        }

        /** @brief Sets color (std::vector). */
        template<typename T>
            requires std::is_arithmetic_v<T>
        inline void set(const std::vector<T>& vect) noexcept(false)
        {
            assert(vect.size() > 3);
            set(vect[0], vect[1], vect[2], vect[3]);
        }

        /** @brief Sets color (std::array). */
        template<typename T>
            requires std::is_arithmetic_v<T>
        inline void set(const std::array<T, 4>& arr) noexcept
        {
            set(arr[0], arr[1], arr[2], arr[3]);
        }


        //---   Gray Value   ------------------------------------------------
        /** @brief Returns the intensity of the luminance for this RGB color. */
        inline const avt::Byte get_lum() const noexcept
        {
            return avt::Byte(0.299f * r + 0.587f * g + 0.114f * b + 0.5f);
        }

        /** @brief Instantiates a new RGBA color with the gray value of this RGBA color and same A channel value. */
        inline RGBAColor to_gray_color() const noexcept
        {
            const avt::Byte lum = get_lum();
            return RGBAColor(lum, lum, lum, a);
        }


        //---   Comparisons   -----------------------------------------------
        /** @brief Returns true if each component of both colors are the same at the same place. */
        inline const bool operator== (const RGBAColor& rhs) const
        {
            return r == rhs.r && g == rhs.g && b == rhs.b && a == rhs.a;
        }

        /** @brief Returns true if any components are not the same at the same place. */
        inline const bool operator!= (const RGBAColor& rhs) const
        {
            return !(*this == rhs);
        }

        /** @brief Returns true if each component of both colors are the same at the same place. */
        inline const bool operator== (const RGBColor& rhs) const
        {
            return r == rhs.r && g == rhs.g && b == rhs.b;
        }

        /** @brief Returns true if any components are not the same at the same place. */
        inline const bool operator!= (const RGBColor& rhs) const
        {
            return !(*this == rhs);
        }


        //---   Adding colors   ---------------------------------------------
        /** @brief In-place adds one RGBA color. */
        RGBAColor& operator+= (const RGBAColor& rhs) noexcept
        {
            const double my_alpha = a / 255.0;
            const double rhs_alpha = rhs.a / 255.0;
            const double blending_coeff = my_alpha / (my_alpha + rhs_alpha);

            r = _clipped(blending_coeff * r + (1.0 - blending_coeff) * rhs.r);
            g = _clipped(blending_coeff * g + (1.0 - blending_coeff) * rhs.g);
            b = _clipped(blending_coeff * b + (1.0 - blending_coeff) * rhs.b);

            return *this;
        }

        /** @brief Adds RGBColor + RGBColor. */
        friend inline RGBAColor operator+ (RGBAColor lhs, const RGBAColor& rhs) noexcept
        {
            return lhs += rhs;
        }


        //---   Subtracting colors   ----------------------------------------
        /** @brief In-place subtracts one RGB color. */
        inline RGBAColor& operator-= (const RGBAColor& rhs) noexcept
        {
            const double my_alpha = a / 255.0;
            const double rhs_alpha = rhs.a / 255.0;
            const double blending_coeff = my_alpha / (my_alpha + rhs_alpha);

            r = _clipped(blending_coeff * r - (1.0 - blending_coeff) * rhs.r);
            g = _clipped(blending_coeff * g - (1.0 - blending_coeff) * rhs.g);
            b = _clipped(blending_coeff * b - (1.0 - blending_coeff) * rhs.b);

            return *this;
        }

        /** @brief Subtracts RGBColor - RGBColor. */
        friend inline RGBAColor operator- (RGBAColor lhs, const RGBAColor& rhs) noexcept
        {
            return lhs -= rhs;
        }


        //---   Attributes   ------------------------------------------------
        avt::Byte a;
    };

}