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
#include <vector>
#include "utils/types.h"

export module utils.rgb_color;


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


        //---   Assignments   -----------------------------------------------
        /** @brief Copy and Move assignments. */
        virtual RGBColor& operator= (const RGBColor&) noexcept = default;
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


        //---   Accessing R, G and B components   ---------------------------
        /** @brief R accessor. */
        inline const avt::Byte r() const noexcept
        {
            return rgb.R;
        }

        /** @brief R mutator. */
        inline avt::Byte& r() noexcept
        {
            return rgb.R;
        }

        /** @brief G accessor. */
        inline const avt::Byte g() const noexcept
        {
            return rgb.G;
        }

        /** @brief G mutator. */
        inline avt::Byte& g() noexcept
        {
            return rgb.G;
        }

        /** @brief B accessor. */
        inline const avt::Byte b() const noexcept
        {
            return rgb.B;
        }

        /** @brief B mutator. */
        inline avt::Byte& b() noexcept
        {
            return rgb.B;
        }


        //---   Set color   -------------------------------------------------
        /** @brief Sets this color to BLACK. */
        virtual inline void clr() noexcept
        {
            rgb.R = rgb.G = rgb.B = avt::Byte{ 0 };
        }

        /** @brief Sets color (copy). */
        virtual inline void set(const RGBColor& copy) noexcept
        {
            rgb.R = copy.rgb.R;
            rgb.G = copy.rgb.G;
            rgb.B = copy.rgb.B;
        }

        /** @brief Sets color (three R, G, B bytes). */
        template<typename T, typename U, typename V>
            requires std::is_arithmetic_v<T>&& std::is_arithmetic_v<U>&& std::is_arithmetic_v<V>
        inline void set(const T r, const U g, const V b) noexcept
        {
            rgb.R = _clipped(r);
            rgb.G = _clipped(g);
            rgb.B = _clipped(b);
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
            return avt::Byte(0.299f * rgb.R + 0.587f * rgb.G + 0.114f * rgb.B + 0.5f);
        }

        /** @brief Instantiates a new RGB color with the gray value of this RGB color. */
        inline RGBColor to_gray_color() const noexcept
        {
            const avt::Byte lum = get_lum();
            return RGBColor(lum, lum, lum);
        }


        //---   Comparisons   -----------------------------------------------
        /** @brief Returns true if each component of both colors are the same at the same place. */
        virtual inline const bool operator== (const RGBColor& rhs) const
        {
            return rgb.R == rhs.rgb.R && rgb.G == rhs.rgb.G && rgb.B == rhs.rgb.B;
        }

        /** @brief Returns true if any components are not the same at the same place. */
        virtual inline const bool operator!= (const RGBColor& rhs) const
        {
            return !(*this == rhs);
        }


        //---   Adding colors   ---------------------------------------------
        /** @brief In-place adds one RGB color. */
        inline RGBColor& operator+= (const RGBColor& rhs) noexcept
        {
            set(rgb.R + rhs.rgb.R, rgb.G + rhs.rgb.G, rgb.B + rhs.rgb.B);
            return *this;
        }

        /** @brief In-place adds one single term. */
        template<typename T>
            requires std::is_arithmetic_v<T>
        RGBColor& operator+= (const T value) noexcept
        {
            set(rgb.R + value, rgb.G + value, rgb.B + value);
            return *this;
        }

        /** @brief In-place adds one std::vector. */
        template<typename T>
            requires std::is_arithmetic_v<T>
        RGBColor& operator+= (const std::vector<T>& rhs) noexcept(false)
        {
            assert(rhs.size() == 3);
            set(rgb.R + rhs[0], rgb.G + rhs[1], rgb.B + rhs[2]);
            return *this;
        }

        /** @brief In-place adds one std::array. */
        template<typename T>
            requires std::is_arithmetic_v<T>
        RGBColor& operator+= (const std::array<T, 3>& rhs) noexcept
        {
            set(rgb.R + rhs[0], rgb.G + rhs[1], rgb.B + rhs[2]);
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
            return RGBColor(rgb.R + value, rgb.G + value, rgb.B + value);
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
            set(rgb.R - rhs.rgb.R, rgb.G - rhs.rgb.G, rgb.B - rhs.rgb.B);
            return *this;
        }

        /** @brief In-place subtracts one single term. */
        template<typename T>
            requires std::is_arithmetic_v<T>
        RGBColor& operator-= (const T value) noexcept
        {
            set(rgb.R - value, rgb.G - value, rgb.B - value);
            return *this;
        }

        /** @brief In-place subtracts one std::vector. */
        template<typename T>
            requires std::is_arithmetic_v<T>
        RGBColor& operator-= (const std::vector<T>& rhs) noexcept(false)
        {
            assert(rhs.size() == 3);
            set(rgb.R - rhs[0], rgb.G - rhs[1], rgb.B - rhs[2]);
            return *this;
        }

        /** @brief In-place subtracts one std::array. */
        template<typename T>
            requires std::is_arithmetic_v<T>
        RGBColor& operator-= (const std::array<T, 3>& rhs) noexcept
        {
            set(rgb.R - rhs[0], rgb.G - rhs[1], rgb.B - rhs[2]);
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
            return RGBColor(rgb.R - value, rgb.G - value, rgb.B - value);
        }

        /** @brief Subtracts one single term - RGBColor. */
        template<typename T>
            requires std::is_arithmetic_v<T>
        friend inline RGBColor operator- (const T value, const RGBColor rhs) noexcept
        {
            return RGBColor(value - rhs.rgb.R, value - rhs.rgb.G, value - rhs.rgb.B);
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
            set(rgb.R * value, rgb.G * value, rgb.B * value);
            return *this;
        }

        /** @brief In-place multiplies one std::vector. */
        template<typename T>
            requires std::is_arithmetic_v<T>
        RGBColor& operator*= (const std::vector<T>& rhs) noexcept(false)
        {
            assert(rhs.size() == 3);
            set(rgb.R * rhs[0], rgb.G * rhs[1], rgb.B * rhs[2]);
            return *this;
        }

        /** @brief In-place multiplies one std::array. */
        template<typename T>
            requires std::is_arithmetic_v<T>
        RGBColor& operator*= (const std::array<T, 3>& rhs) noexcept
        {
            set(rgb.R * rhs[0], rgb.G * rhs[1], rgb.B * rhs[2]);
            return *this;
        }

        /** @brief Multiplies RGBColor * one single term. */
        template<typename T>
            requires std::is_arithmetic_v<T>
        inline RGBColor operator* (const T value) const noexcept
        {
            return RGBColor(rgb.R * value, rgb.G * value, rgb.B * value);
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
            set(_div(rgb.R, value), _div(rgb.G, value), _div(rgb.B, value));
            return *this;
        }

        /** @brief In-place divides one std::vector. */
        template<typename T>
            requires std::is_arithmetic_v<T>
        RGBColor& operator/= (const std::vector<T>& rhs) noexcept(false)
        {
            assert(rhs.size() == 3);
            set(_div(rgb.R, rhs[0]), _div(rgb.G, rhs[1]), _div(rgb.B, rhs[2]));
            return *this;
        }

        /** @brief In-place divides one std::array. */
        template<typename T>
            requires std::is_arithmetic_v<T>
        RGBColor& operator/= (const std::array<T, 3>& rhs) noexcept(false)
        {
            set(_div(rgb.R, rhs[0]), _div(rgb.G, rhs[1]), _div(rgb.B, rhs[2]));
            return *this;
        }

        /** @brief Divides RGBColor / one single term. */
        template<typename T>
            requires std::is_arithmetic_v<T>
        inline RGBColor operator/ (const T value) const noexcept(false)
        {
            return RGBColor(_div(rgb.R, value), _div(rgb.G, value), _div(rgb.B, value));
        }

        /** @brief Divides one single term / RGBColor. */
        template<typename T>
            requires std::is_arithmetic_v<T>
        friend inline RGBColor operator/ (const T value, const RGBColor rhs) noexcept(false)
        {
            return RGBColor(rhs._div(value, rhs.rgb.R), rhs._div(value, rhs.rgb.G), rhs._div(value, rhs.rgb.B));
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
            return RGBColor(rhs._div(lhs[0], rhs.rgb.R), rhs._div(lhs[1], rhs.rgb.G), rhs._div(lhs[2], rhs.rgb.B));
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
            return RGBColor(rhs._div(lhs[0], rhs.rgb.R), rhs._div(lhs[1], rhs.rgb.G), rhs._div(lhs[2], rhs.rgb.B));
        }


        //---   Data   ------------------------------------------------------
        union {
            avt::Byte data[3];
            struct {
                avt::Byte B;
                avt::Byte G;
                avt::Byte R;
            } rgb;
        };


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
            return avt::Byte(std::clamp(value, T(0), T(255)));
            //return (value >= 255) ? 255 : ((value <= 0) ? 0 : (avt::Byte)value);
        }


    private:
        /** @brief Returns the rounding value of a division. */
        template<typename T, typename U>
            requires std::is_arithmetic_v<T>&& std::is_arithmetic_v<U>
        inline static const avt::Byte _div(const T num, const U den) noexcept(false)
        {
            assert(den > 0);
            return (avt::Byte)(float(num) / float(den));
        }

    };

    //-----------------------------------------------------------------------
    const RGBColor RGBColor::ANTHRACITE{ 31, 31, 31 };
    const RGBColor RGBColor::BLACK{ 0, 0, 0 };
    const RGBColor RGBColor::BLUE{ 0, 0, 255 };
    const RGBColor RGBColor::BROWN{ 95, 47, 0 };
    const RGBColor RGBColor::DARK_RED{ 127, 0, 0 };
    const RGBColor RGBColor::DEEP_GRAY{ 63, 63, 63 };
    const RGBColor RGBColor::DEEP_GREEN{ 0, 95, 0 };
    const RGBColor RGBColor::GRAY{ 127, 127, 127 };
    const RGBColor RGBColor::LIGHT_BLUE{ 0, 255, 255 };
    const RGBColor RGBColor::LIGHT_GRAY{ 191, 191, 191 };
    const RGBColor RGBColor::LIGHT_GREEN{ 0, 255, 0 };
    const RGBColor RGBColor::NAVY_BLUE{ 0, 0, 63 };
    const RGBColor RGBColor::ORANGE{ 255, 127, 0 };
    const RGBColor RGBColor::RED{ 255, 0, 0 };
    const RGBColor RGBColor::YELLOW{ 255, 255, 0 };
    const RGBColor RGBColor::WHITE{ 255, 255, 255 };

    const RGBColor RGBColor::TARGET_WHITE{ 255, 255, 255 };
    const RGBColor RGBColor::TARGET_BLACK{ 0, 0, 0 };
    const RGBColor RGBColor::TARGET_BLUE{ 65, 181, 200 };
    const RGBColor RGBColor::TARGET_RED{ 255, 37, 21 };
    const RGBColor RGBColor::TARGET_GOLD{ 255, 245, 55 };

    const RGBColor RGBColor::TARGET_BLUE_6{ 17, 165, 255 };
    const RGBColor RGBColor::TARGET_BLUE_NFAA{ 63, 63, 95 };

}