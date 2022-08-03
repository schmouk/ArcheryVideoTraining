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

export module utils.rgb_color;

import utils;


//===========================================================================
export namespace avt::utils
{
    //=======================================================================
    /** @brief The class of RGB components colors.
    *
    * Library OpenCV uses a default BGR coding for colors which is not the
    * usual RGB one.  To help developments, class RGBColor codes colors in
    * a usual way while the BGR conversion is internally automated for its
    * use with library OpenCV.
    */
    class RGBColor
    {
    public:
        //---   Constructors / Destructor   ---------------------------------
        /** @brief Default constructor. */
        inline RGBColor() noexcept
        {
            set(0, 0, 0);
        }

        /** @brief Valued Constructor (three R, G, B bytes). */
        template<typename T, typename U, typename V>
            requires std::is_arithmetic_v<T>&& std::is_arithmetic_v<U>&& std::is_arithmetic_v<V>
        inline RGBColor(const T r, const U g, const V b) noexcept
        {
            set(r, g, b);
        }

        /** @brief Valued Constructor (one gray value). */
        template<typename T>
            requires std::is_arithmetic_v<T>
        inline RGBColor(const T value) noexcept
        {
            set(value, value, value);
        }

        /** @brief Valued Constructor (one 3-bytes buffer). */
        template<typename T>
            requires std::is_arithmetic_v<T>
        inline RGBColor(const T buffer[3]) noexcept
        {
            set(buffer);
        }

        /** @brief Valued Constructor (std::vector). */
        template<typename T>
            requires std::is_arithmetic_v<T>
        inline RGBColor(const std::vector<T>& vect) noexcept(false)
        {
            set(vect);
        }

        /** @brief Valued Constructor (std::array). */
        template<typename T>
            requires std::is_arithmetic_v<T>
        inline RGBColor(const std::array<T, 3>& arr) noexcept
        {
            set(arr);
        }

        /** @brief Copy and Move constructors. */
        RGBColor(const RGBColor&) noexcept = default;
        RGBColor(RGBColor&&) noexcept = default;

        /** @brief Destructor. */
        virtual ~RGBColor() noexcept = default;


        //---   Casting operators   -----------------------------------------
        /** @brief Returns a string describing this color components (hexadecimal form). */
        virtual inline operator std::string() const
        {
            return std::format("#{:02X}{:02X}{:02X}", r, g, b);
        }

        /** @brief Casts this RGB color to a cv::Vec3b instance. */
        inline operator cv::Vec3b()
        {
            return cv::Vec3b(bgr);
        }

        /** @brief Casts this RGB color to a cv::Scalar (i.e. 4 doubles, the fourth one being 255.0). */
        virtual inline operator cv::Scalar()
        {
            return cv::Scalar(double(b), double(g), double(r), 255.0);
        }

        /** @brief Casts this RGB color to a cv::Scalar_<avt::Byte> (i.e. 4 bytes, the fourth one being 255). */
        virtual inline operator avt::CVScalarByte()
        {
            return avt::CVScalarByte(b, g, r, avt::Byte(255));
        }

        /** @brief Casts this RGB color to a cv::Scalar_<avt::Byte> (i.e. 4 bytes, the fourth one being 255). */
        virtual inline operator avt::CVScalarByte() const
        {
            return avt::CVScalarByte(b, g, r, avt::Byte(255));
        }

        /** @brief Creates an instance of cv::Scalar with transparency. */
        template<typename T>
            requires std::is_arithmetic_v<T>
        inline cv::Scalar to_cv_scalar(const T transparency) const
        {
            return cv::Scalar(double(b), double(g), double(r), double(avt::utils::clamp_b(transparency)));
        }

        /** @brief Creates an instance of cv::Scalar_<Byte> with transparency. */
        template<typename T>
            requires std::is_arithmetic_v<T>
        inline avt::CVScalarByte to_avt_scalar_byte(const T transparency) const
        {
            return avt::CVScalarByte(b, g, r, avt::utils::clamp_b(transparency));
        }


        //---   Assignments   -----------------------------------------------
        /** @brief Copy assignment. */
        virtual RGBColor& operator= (const RGBColor&) noexcept = default;

        /** @brief Move assignment. */
        virtual RGBColor& operator= (RGBColor&&) noexcept = default;

        /** @brief Assignment operator (one 3-bytes buffer). */
        template<typename T>
            requires std::is_arithmetic_v<T>
        inline RGBColor& operator= (const T buffer[3]) noexcept
        {
            set(buffer);
            return *this;
        }

        /** @brief Assignment operator (std::vector). */
        template<typename T>
            requires std::is_arithmetic_v<T>
        inline RGBColor& operator= (const std::vector<T>& vect) noexcept(false)
        {
            set(vect);
            return *this;
        }

        /** @brief Assignment operator (std::array). */
        template<typename T>
            requires std::is_arithmetic_v<T>
        inline RGBColor& operator= (const std::array<T, 3>& arr) noexcept
        {
            set(arr);
            return *this;
        }


        //---   Set color   -------------------------------------------------
        /** @brief Sets this color to BLACK. */
        virtual inline void clr() noexcept
        {
            r = g = b = avt::Byte{ 0 };
        }

        /** @brief Sets color (copy). */
        virtual inline void set(const RGBColor& copy) noexcept
        {
            r = copy.r;
            g = copy.g;
            b = copy.b;
        }

        /** @brief Sets color (three R, G, B bytes). */
        template<typename T, typename U, typename V>
            requires std::is_arithmetic_v<T> && std::is_arithmetic_v<U> && std::is_arithmetic_v<V>
        inline void set(const T r_, const U g_, const V b_) noexcept
        {
            r = _clipped(r_);
            g = _clipped(g_);
            b = _clipped(b_);
        }

        /** @brief Sets color (one 3-bytes buffer). */
        template<typename T>
            requires std::is_arithmetic_v<T>
        inline void set(const T buffer[3]) noexcept
        {
            set(buffer[0], buffer[1], buffer[2]);
        }

        /** @brief Sets color (std::vector). */
        template<typename T>
            requires std::is_arithmetic_v<T>
        inline void set(const std::vector<T>& vect) noexcept(false)
        {
            assert(vect.size() == 3);
            set(vect[0], vect[1], vect[2]);
        }

        /** @brief Sets color (std::array). */
        template<typename T>
            requires std::is_arithmetic_v<T>
        inline void set(const std::array<T, 3>& arr) noexcept
        {
            set(arr[0], arr[1], arr[2]);
        }


        //---   Gray Value   ------------------------------------------------
        /** @brief Returns the intensity of the luminance for this RGB color. */
        inline const avt::Byte get_lum() const noexcept
        {
            return avt::Byte(0.299f * r + 0.587f * g + 0.114f * b + 0.5f);
        }

        /** @brief Instantiates a new RGB color with the gray value of this RGB color. */
        inline RGBColor to_gray_color() const noexcept
        {
            const avt::Byte lum = get_lum();
            return RGBColor(lum, lum, lum);
        }


        //---   Comparisons   -----------------------------------------------
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
        /** @brief In-place adds one RGB color. */
        inline RGBColor& operator+= (const RGBColor& rhs) noexcept
        {
            set(r + rhs.r, g + rhs.g, b + rhs.b);
            return *this;
        }

        /** @brief In-place adds one single term. */
        template<typename T>
            requires std::is_arithmetic_v<T>
        RGBColor& operator+= (const T value) noexcept
        {
            set(r + value, g + value, b + value);
            return *this;
        }

        /** @brief In-place adds one std::vector. */
        template<typename T>
            requires std::is_arithmetic_v<T>
        RGBColor& operator+= (const std::vector<T>& rhs) noexcept(false)
        {
            assert(rhs.size() == 3);
            set(r + rhs[0], g + rhs[1], b + rhs[2]);
            return *this;
        }

        /** @brief In-place adds one std::array. */
        template<typename T>
            requires std::is_arithmetic_v<T>
        RGBColor& operator+= (const std::array<T, 3>& rhs) noexcept
        {
            set(r + rhs[0], g + rhs[1], b + rhs[2]);
            return *this;
        }

        /** @brief Adds RGBColor + RGBColor. */
        friend inline RGBColor operator+ (RGBColor lhs, const RGBColor& rhs) noexcept
        {
            return lhs += rhs;
        }

        /** @brief Adds RGBColor + one single term. */
        template<typename T>
            requires std::is_arithmetic_v<T>
        inline RGBColor operator+ (const T value) const noexcept
        {
            return RGBColor(r + value, g + value, b + value);
        }

        /** @brief Adds one single term + RGBColor. */
        template<typename T>
            requires std::is_arithmetic_v<T>
        friend inline RGBColor operator+ (const T value, const RGBColor rhs) noexcept
        {
            return rhs + value;
        }

        /** @brief Adds RGBColor + one std::vector. */
        template<typename T>
            requires std::is_arithmetic_v<T>
        friend inline RGBColor operator+ (RGBColor lhs, const std::vector<T>& rhs) noexcept(false)
        {
            assert(rhs.size() == 3);
            return lhs += rhs;
        }

        /** @brief Adds one std::vector + RGBColor. */
        template<typename T>
            requires std::is_arithmetic_v<T>
        friend inline RGBColor operator+ (const std::vector<T>& lhs, RGBColor rhs) noexcept(false)
        {
            assert(lhs.size() == 3);
            return rhs += lhs;
        }

        /** @brief Adds RGBColor + one std::array. */
        template<typename T>
            requires std::is_arithmetic_v<T>
        friend inline RGBColor operator+ (RGBColor lhs, const std::array<T, 3>& rhs) noexcept
        {
            return lhs += rhs;
        }

        /** @brief Adds one std::array + RGBColor. */
        template<typename T>
            requires std::is_arithmetic_v<T>
        friend inline RGBColor operator+ (const std::array<T, 3>& lhs, RGBColor rhs) noexcept
        {
            return rhs += lhs;
        }


        //---   Subtracting colors   ----------------------------------------
        /** @brief In-place subtracts one RGB color. */
        inline RGBColor& operator-= (const RGBColor& rhs) noexcept
        {
            set(r - rhs.r, g - rhs.g, b - rhs.b);
            return *this;
        }

        /** @brief In-place subtracts one single term. */
        template<typename T>
            requires std::is_arithmetic_v<T>
        RGBColor& operator-= (const T value) noexcept
        {
            set(r - value, g - value, b - value);
            return *this;
        }

        /** @brief In-place subtracts one std::vector. */
        template<typename T>
            requires std::is_arithmetic_v<T>
        RGBColor& operator-= (const std::vector<T>& rhs) noexcept(false)
        {
            assert(rhs.size() == 3);
            set(r - rhs[0], g - rhs[1], b - rhs[2]);
            return *this;
        }

        /** @brief In-place subtracts one std::array. */
        template<typename T>
            requires std::is_arithmetic_v<T>
        RGBColor& operator-= (const std::array<T, 3>& rhs) noexcept
        {
            set(r - rhs[0], g - rhs[1], b - rhs[2]);
            return *this;
        }

        /** @brief Subtracts RGBColor - RGBColor. */
        friend inline RGBColor operator- (RGBColor lhs, const RGBColor& rhs) noexcept
        {
            return lhs -= rhs;
        }

        /** @brief Subtracts RGBColor - one single term. */
        template<typename T>
            requires std::is_arithmetic_v<T>
        inline RGBColor operator- (const T value) const noexcept
        {
            return RGBColor(r - value, g - value, b - value);
        }

        /** @brief Subtracts one single term - RGBColor. */
        template<typename T>
            requires std::is_arithmetic_v<T>
        friend inline RGBColor operator- (const T value, const RGBColor rhs) noexcept
        {
            return RGBColor(value - rhs.r, value - rhs.g, value - rhs.b);
        }

        /** @brief Subtracts RGBColor - one std::vector. */
        template<typename T>
            requires std::is_arithmetic_v<T>
        friend inline RGBColor operator- (RGBColor lhs, const std::vector<T>& rhs) noexcept(false)
        {
            assert(rhs.size() == 3);
            return lhs -= rhs;
        }

        /** @brief Subtracts one std::vector - RGBColor. */
        template<typename T>
            requires std::is_arithmetic_v<T>
        friend inline RGBColor operator- (const std::vector<T>& lhs, RGBColor rhs) noexcept(false)
        {
            assert(lhs.size() == 3);
            return RGBColor(lhs) -= rhs;
        }

        /** @brief Subtracts RGBColor - one std::array. */
        template<typename T>
            requires std::is_arithmetic_v<T>
        friend inline RGBColor operator- (RGBColor lhs, const std::array<T, 3>& rhs) noexcept
        {
            return lhs -= rhs;
        }

        /** @brief Subtracts one std::array - RGBColor. */
        template<typename T>
            requires std::is_arithmetic_v<T>
        friend inline RGBColor operator- (const std::array<T, 3>& lhs, RGBColor rhs) noexcept
        {
            return RGBColor(lhs) -= rhs;
        }


        //---   Magnifying colors   -----------------------------------------
        /** @brief In-place multiplies (one single factor). */
        template<typename T>
            requires std::is_arithmetic_v<T>
        RGBColor& operator*= (const T value) noexcept
        {
            set(r * value, g * value, b * value);
            return *this;
        }

        /** @brief In-place multiplies one std::vector. */
        template<typename T>
            requires std::is_arithmetic_v<T>
        RGBColor& operator*= (const std::vector<T>& rhs) noexcept(false)
        {
            assert(rhs.size() == 3);
            set(r * rhs[0], g * rhs[1], b * rhs[2]);
            return *this;
        }

        /** @brief In-place multiplies one std::array. */
        template<typename T>
            requires std::is_arithmetic_v<T>
        RGBColor& operator*= (const std::array<T, 3>& rhs) noexcept
        {
            set(r * rhs[0], g * rhs[1], b * rhs[2]);
            return *this;
        }

        /** @brief Multiplies RGBColor * one single term. */
        template<typename T>
            requires std::is_arithmetic_v<T>
        inline RGBColor operator* (const T value) const noexcept
        {
            return RGBColor(r * value, g * value, b * value);
        }

        /** @brief Multiplies one single term * RGBColor. */
        template<typename T>
            requires std::is_arithmetic_v<T>
        friend inline RGBColor operator* (const T value, const RGBColor rhs) noexcept
        {
            return rhs * value;
        }

        /** @brief Multiplies RGBColor * one std::vector. */
        template<typename T>
            requires std::is_arithmetic_v<T>
        friend inline RGBColor operator* (RGBColor lhs, const std::vector<T>& rhs) noexcept(false)
        {
            assert(rhs.size() == 3);
            return lhs *= rhs;
        }

        /** @brief Multiplies one std::vector * RGBColor. */
        template<typename T>
            requires std::is_arithmetic_v<T>
        friend inline RGBColor operator* (const std::vector<T>& lhs, RGBColor rhs) noexcept(false)
        {
            assert(lhs.size() == 3);
            return rhs *= lhs;
        }

        /** @brief Multiplies RGBColor * one std::array. */
        template<typename T>
            requires std::is_arithmetic_v<T>
        friend inline RGBColor operator* (RGBColor lhs, const std::array<T, 3>& rhs) noexcept
        {
            return lhs *= rhs;
        }

        /** @brief Multiplies one std::array * RGBColor. */
        template<typename T>
            requires std::is_arithmetic_v<T>
        friend inline RGBColor operator* (const std::array<T, 3>& lhs, RGBColor rhs) noexcept
        {
            return rhs *= lhs;
        }


        //---   Dividing colors   -------------------------------------------
        /** @brief In-place divides (one single factor). */
        template<typename T>
            requires std::is_arithmetic_v<T>
        RGBColor& operator/= (const T value) noexcept(false)
        {
            assert(value > 0);
            set(_div(r, value), _div(g, value), _div(b, value));
            return *this;
        }

        /** @brief In-place divides one std::vector. */
        template<typename T>
            requires std::is_arithmetic_v<T>
        RGBColor& operator/= (const std::vector<T>& rhs) noexcept(false)
        {
            assert(rhs.size() == 3);
            set(_div(r, rhs[0]), _div(g, rhs[1]), _div(b, rhs[2]));
            return *this;
        }

        /** @brief In-place divides one std::array. */
        template<typename T>
            requires std::is_arithmetic_v<T>
        RGBColor& operator/= (const std::array<T, 3>& rhs) noexcept(false)
        {
            set(_div(r, rhs[0]), _div(g, rhs[1]), _div(b, rhs[2]));
            return *this;
        }

        /** @brief Divides RGBColor / one single term. */
        template<typename T>
            requires std::is_arithmetic_v<T>
        inline RGBColor operator/ (const T value) const noexcept(false)
        {
            return RGBColor(_div(r, value), _div(g, value), _div(b, value));
        }

        /** @brief Divides one single term / RGBColor. */
        template<typename T>
            requires std::is_arithmetic_v<T>
        friend inline RGBColor operator/ (const T value, const RGBColor rhs) noexcept(false)
        {
            return RGBColor(rhs._div(value, rhs.r), rhs._div(value, rhs.g), rhs._div(value, rhs.b));
        }

        /** @brief Divides RGBColor * one std::vector. */
        template<typename T>
            requires std::is_arithmetic_v<T>
        friend inline RGBColor operator/ (RGBColor lhs, const std::vector<T>& rhs) noexcept(false)
        {
            assert(rhs.size() == 3);
            return lhs /= rhs;
        }

        /** @brief Divides one std::vector * RGBColor. */
        template<typename T>
            requires std::is_arithmetic_v<T>
        friend inline RGBColor operator/ (const std::vector<T>& lhs, RGBColor rhs) noexcept(false)
        {
            assert(lhs.size() == 3);
            return RGBColor(rhs._div(lhs[0], rhs.r), rhs._div(lhs[1], rhs.g), rhs._div(lhs[2], rhs.b));
        }

        /** @brief Divides RGBColor * one std::array. */
        template<typename T>
            requires std::is_arithmetic_v<T>
        friend inline RGBColor operator/ (RGBColor lhs, const std::array<T, 3>& rhs) noexcept(false)
        {
            return lhs /= rhs;
        }

        /** @brief Divides one std::array * RGBColor. */
        template<typename T>
            requires std::is_arithmetic_v<T>
        friend inline RGBColor operator/ (const std::array<T, 3>& lhs, RGBColor rhs) noexcept(false)
        {
            return RGBColor(rhs._div(lhs[0], rhs.r), rhs._div(lhs[1], rhs.g), rhs._div(lhs[2], rhs.b));
        }


        //---   Attributes   ------------------------------------------------
        union {
            struct {
                avt::Byte b;  //!< the Blue component of this color
                avt::Byte g;  //!< the Green component of this color
                avt::Byte r;  //!< the Red component of this color
            };
            avt::Byte bgr[3];
        };

        static inline int R{ 2 };  //!< to be used when indexing '.bgr'
        static inline int G{ 1 };  //!< to be used when indexing '.bgr'
        static inline int B{ 0 };  //!< to be used when indexing '.bgr'


        //---   Predefined colors   -----------------------------------------
        static const RGBColor
            ANTHRACITE,
            BLACK,
            BLUE,
            BROWN,
            DARK_RED,
            DEEP_GRAY,
            DEEP_GREEN,
            GRAY,
            LIGHT_BLUE,
            LIGHT_GRAY,
            LIGHT_GREEN,
            NAVY_BLUE,
            ORANGE,
            RED,
            YELLOW,
            WHITE,

            TARGET_WHITE,
            TARGET_BLACK,
            TARGET_BLUE,
            TARGET_RED,
            TARGET_GOLD,

            TARGET_BLUE_6,
            TARGET_BLUE_NFAA;


    protected:
        /** @brief Returns the clipped value acccording to color components type. */
        template<typename T>
            requires std::is_arithmetic_v<T>
        inline static const avt::Byte _clipped(const T value) noexcept
        {
            return avt::utils::clamp_b(value);
        }


    private:
        /** @brief Returns the rounding value of a division. */
        template<typename T, typename U>
            requires std::is_arithmetic_v<T>&& std::is_arithmetic_v<U>
        inline static const avt::Byte _div(const T num, const U den) noexcept(false)
        {
            assert(den > 0);
            return avt::utils::clamp_b(float(num) / float(den));
        }

    };

    //-----------------------------------------------------------------------
    const RGBColor RGBColor::ANTHRACITE       {  31,  31,  31 };
    const RGBColor RGBColor::BLACK            {   0,   0,   0 };
    const RGBColor RGBColor::BLUE             {   0,   0, 255 };
    const RGBColor RGBColor::BROWN            {  95,  47,   0 };
    const RGBColor RGBColor::DARK_RED         { 127,   0,   0 };
    const RGBColor RGBColor::DEEP_GRAY        {  63,  63,  63 };
    const RGBColor RGBColor::DEEP_GREEN       {   0,  95,   0 };
    const RGBColor RGBColor::GRAY             { 127, 127, 127 };
    const RGBColor RGBColor::LIGHT_BLUE       {   0, 255, 255 };
    const RGBColor RGBColor::LIGHT_GRAY       { 191, 191, 191 };
    const RGBColor RGBColor::LIGHT_GREEN      {   0, 255,   0 };
    const RGBColor RGBColor::NAVY_BLUE        {   0,   0,  63 };
    const RGBColor RGBColor::ORANGE           { 255, 127,   0 };
    const RGBColor RGBColor::RED              { 255,   0,   0 };
    const RGBColor RGBColor::YELLOW           { 255, 255,   0 };
    const RGBColor RGBColor::WHITE            { 255, 255, 255 };

    const RGBColor RGBColor::TARGET_WHITE     { 255, 255, 255 };
    const RGBColor RGBColor::TARGET_BLACK     {   0,   0,   0 };
    const RGBColor RGBColor::TARGET_BLUE      {  65, 181, 200 };
    const RGBColor RGBColor::TARGET_RED       { 255,  37,  21 };
    const RGBColor RGBColor::TARGET_GOLD      { 255, 245,  55 };

    const RGBColor RGBColor::TARGET_BLUE_6    {  17, 165, 255 };
    const RGBColor RGBColor::TARGET_BLUE_NFAA {  63,  63,  95 };

}