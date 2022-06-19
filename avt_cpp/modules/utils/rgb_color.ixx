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

#include <array>
#include <vector>

export module avt.utils;

//===========================================================================
namespace avt::utils
{
    //=======================================================================
    /** \brief The class of RGB components colors.
    *
    * Library OpenCV uses a default BGR coding for colors which is not the 
    * usual RGB one.  To help developments, class RGBColor codes colors in 
    * a usual way while the BGR conversion is internally automated for its
    * use with library OpenCV.
    */
    export class RGBColor
    {
    public:
        //---   Constructors / Destructor   ---------------------------------
        /** \brief Default constructor. */
        inline RGBColor() noexcept
        {
            set(0, 0, 0);
        }

        /** \brief Valued Constructor (three R, G, B bytes). */
        template<typename T, typename U, typename V>
            requires std::is_arithmetic_v<T> && std::is_arithmetic_v<U> && std::is_arithmetic_v<V>
        inline RGBColor(const T r, const U g, const V b) noexcept
        {
            set(r, g, b);
        }

        /** \brief Valued Constructor (one gray value). */
        template<typename T>
            requires std::is_arithmetic_v<T>
        inline RGBColor(const T value) noexcept
        {
            set(value, value, value);
        }

        /** \brief Valued Constructor (one 3-bytes buffer). */
        template<typename T>
            requires std::is_arithmetic_v<T>
        inline RGBColor(const unsigned char buffer[3]) noexcept
        {
            set(buffer);
        }

        /** \brief Valued Constructor (std::vector). */
        template<typename T>
            requires std::is_arithmetic_v<T>
        inline RGBColor(const std::vector<T>& vect) noexcept(false)
        {
            set(vect);
        }

        /** \brief Valued Constructor (std::array). */
        template<typename T>
            requires std::is_arithmetic_v<T>
        inline RGBColor(const std::array<T, 3>& arr) noexcept
        {
            set(arr);
        }

        /** \brief Copy and Move constructors. */
        RGBColor(const RGBColor&) noexcept = default;
        RGBColor(RGBColor&&) noexcept = default;

        /** \brief Destructor. */
        ~RGBColor() noexcept = default;


        //---   Assignments   -----------------------------------------------
        /** \brief Copy and Move assignments. */
        RGBColor& operator= (const RGBColor&) noexcept = default;
        RGBColor& operator= (RGBColor&&) noexcept = default;

        /** \brief Assignment operator (one 3-bytes buffer). */
        template<typename T>
            requires std::is_arithmetic_v<T>
        inline RGBColor& operator= (const unsigned char buffer[3]) noexcept
        {
            set(buffer);
            return *this;
        }

        /** \brief Assignment operator (std::vector). */
        template<typename T>
            requires std::is_arithmetic_v<T>
        inline RGBColor& operator= (const std::vector<T>& vect) noexcept(false)
        {
            set(vect);
            return *this;
        }

        /** \brief Assignment operator (std::array). */
        template<typename T>
            requires std::is_arithmetic_v<T>
        inline RGBColor& operator= (const std::array<T, 3>& arr) noexcept
        {
            set(arr);
            return *this;
        }


        //---   Set color   -------------------------------------------------
        /** \brief Sets this color to BLACK. */
        inline void clr() noexcept
        {
            set(BLACK);
        }

        /** \brief Sets color (copy). */
        inline void set(const RGBColor& copy) noexcept
        {
            r = copy.r;
            g = copy.g;
            b = copy.b;
        }

        /** \brief Sets color (three R, G, B bytes). */
        template<typename T, typename U, typename V>
            requires std::is_arithmetic_v<T>&& std::is_arithmetic_v<U>&& std::is_arithmetic_v<V>
        inline void set(const T r, const U g, const V b) noexcept
        {
            this->r = _clipped(r);
            this->g = _clipped(g);
            this->b = _clipped(b);
        }

        /** \brief Sets color (one 3-bytes buffer). */
        template<typename T>
            requires std::is_arithmetic_v<T>
        inline void set(const unsigned char buffer[3]) noexcept
        {
            set(buffer[0], buffer[1], buffer[2]);
        }

        /** \brief Sets color (std::vector). */
        template<typename T>
            requires std::is_arithmetic_v<T>
        inline void set(const std::vector<T>& vect) noexcept(false)
        {
            assert(vect.size() == 3);
            set(vect[0], vect[1], vect[2]);
        }

        /** \brief Sets color (std::array). */
        template<typename T>
            requires std::is_arithmetic_v<T>
        inline void set(const std::array<T, 3>& arr) noexcept
        {
            set(arr[0], arr[1], arr[2]);
        }


        //---   Gray Value   ------------------------------------------------
        /** \brief Returns the intensity of the luminance for this RGB color. */
        inline const unsigned char get_lum() const noexcept
        {
            return unsigned char(0.299f * r + 0.587f * g + 0.114f * b + 0.5f);
        }

        /** \brief Instantiates a new RGB color with the gray value of this RGB color. */
        inline RGBColor to_gray_color() const noexcept
        {
            const unsigned char lum = get_lum();
            return RGBColor(lum, lum, lum);
        }


        //---   Comparisons   -----------------------------------------------
        /** \brief Returns true if each component of both colors are the same at the same place. */
        inline const bool operator== (const RGBColor& rhs) const
        {
            return r == rhs.r && g == rhs.g && b == rhs.b;
        }

        /** \brief Returns true if any components are not the same at the same place. */
        inline const bool operator!= (const RGBColor& rhs) const
        {
            return !(*this == rhs);
        }


        //---   Adding colors   ---------------------------------------------
        /** \brief In-place adds one RGB color. */
        inline RGBColor& operator+= (const RGBColor& rhs) noexcept
        {
            set(r + rhs.r, g + rhs.g, b + rhs.b);
            return *this;
        }

        /** \brief In-place adds one single term. */
        template<typename T>
            requires std::is_arithmetic_v<T>
        RGBColor& operator+= (const T value) noexcept
        {
            set(r + value, g + value, b + value);
            return *this;
        }

        /** \brief In-place adds one 3-bytes buffer. */
        template<typename T>
            requires std::is_arithmetic_v<T>
        RGBColor& operator+= (const T rhs[3]) noexcept
        {
           set(r + rhs[0], g + rhs[1], b + rhs[2]);
            return *this;
        }

        /** \brief In-place adds one std::vector. */
        template<typename T>
            requires std::is_arithmetic_v<T>
        RGBColor& operator+= (const std::vector<T>& rhs) noexcept(false)
        {
            assert(rhs.size() == 3);
            set(r + rhs.r, g + rhs.g, b + rhs.b);
            return *this;
        }

        /** \brief In-place adds one std::array. */
        template<typename T>
            requires std::is_arithmetic_v<T>
        RGBColor& operator+= (const std::array<T, 3>& rhs) noexcept
        {
            set(r + rhs.r, g + rhs.g, b + rhs.b);
            return *this;
        }

        /** \brief Adds RGBColor + RGBColor. */
        friend inline RGBColor operator+ (RGBColor lhs, const RGBColor& rhs) noexcept
        {
            return lhs += rhs;
        }

        /** \brief Adds RGBColor + one single term. */
        template<typename T>
            requires std::is_arithmetic_v<T>
        inline RGBColor operator+ (const T value) const noexcept
        {
            return RGBColor(r + value, g + value, b + value);
        }

        /** \brief Adds one single term + RGBColor. */
        template<typename T>
            requires std::is_arithmetic_v<T>
        friend inline RGBColor operator+ (const T value, const RGBColor rhs) noexcept
        {
            return rhs + value;
        }

        /** \brief Adds RGBColor + one 3-bytes buffer. */
        template<typename T>
            requires std::is_arithmetic_v<T>
        friend inline RGBColor operator+ (const RGBColor& lhs, const unsigned char rhs[3]) noexcept
        {
            return RGBColor(lhs.r + rhs[0], lhs.g + rhs[1], lhs.b + rhs[2]);
        }

        /** \brief Adds one 3-bytes buffer + RGBColor. */
        template<typename T>
            requires std::is_arithmetic_v<T>
        friend inline RGBColor operator+ (const unsigned char lhs[3], const RGBColor& rhs) noexcept
        {
            return RGBColor(rhs.r + lhs[0], rhs.g + lhs[1], rhs.b + lhs[2]);
        }

        /** \brief Adds RGBColor + one std::vector. */
        template<typename T>
            requires std::is_arithmetic_v<T>
        friend inline RGBColor operator+ (RGBColor lhs, const std::vector<T>& rhs) noexcept(false)
        {
            assert(rhs.size() == 3);
            return lhs += rhs;
        }

        /** \brief Adds one std::vector + RGBColor. */
        template<typename T>
            requires std::is_arithmetic_v<T>
        friend inline RGBColor operator+ (const std::vector<T>& lhs, RGBColor rhs) noexcept(false)
        {
            assert(lhs.size() == 3);
            return rhs += lhs;
        }

        /** \brief Adds RGBColor + one std::array. */
        template<typename T>
            requires std::is_arithmetic_v<T>
        friend inline RGBColor operator+ (RGBColor lhs, const std::array<T, 3>& rhs) noexcept
        {
            return lhs += rhs;
        }

        /** \brief Adds one std::array + RGBColor. */
        template<typename T>
            requires std::is_arithmetic_v<T>
        friend inline RGBColor operator+ (const std::array<T, 3>& lhs, RGBColor rhs) noexcept
        {
            return rhs += lhs;
        }


        //---   Subtracting colors   ----------------------------------------
        /** \brief In-place subtracts one RGB color. */
        inline RGBColor& operator-= (const RGBColor& rhs) noexcept
        {
            set(r - rhs.r, g - rhs.g, b - rhs.b);
            return *this;
        }

        /** \brief In-place subtracts one single term. */
        template<typename T>
            requires std::is_arithmetic_v<T>
        RGBColor& operator-= (const T value) noexcept
        {
            set(r - value, g - value, b - value);
            return *this;
        }

        /** \brief In-place subtracts one 3-bytes buffer. */
        template<typename T>
            requires std::is_arithmetic_v<T>
        RGBColor& operator-= (const T rhs[3]) noexcept
        {
            set(r - rhs[0], g - rhs[1], b - rhs[2]);
            return *this;
        }

        /** \brief In-place subtracts one std::vector. */
        template<typename T>
            requires std::is_arithmetic_v<T>
        RGBColor& operator-= (const std::vector<T>& rhs) noexcept(false)
        {
            assert(rhs.size() == 3);
            set(r - rhs.r, g - rhs.g, b - rhs.b);
            return *this;
        }

        /** \brief In-place subtracts one std::array. */
        template<typename T>
            requires std::is_arithmetic_v<T>
        RGBColor& operator-= (const std::array<T, 3>& rhs) noexcept
        {
            set(r - rhs.r, g - rhs.g, b - rhs.b);
            return *this;
        }

        /** \brief Subtracts RGBColor - RGBColor. */
        friend inline RGBColor operator- (RGBColor lhs, const RGBColor& rhs) noexcept
        {
            return lhs -= rhs;
        }

        /** \brief Subtracts RGBColor - one single term. */
        template<typename T>
            requires std::is_arithmetic_v<T>
        inline RGBColor operator- (const T value) const noexcept
        {
            return RGBColor(r - value, g - value, b - value);
        }

        /** \brief Subtracts one single term - RGBColor. */
        template<typename T>
            requires std::is_arithmetic_v<T>
        friend inline RGBColor operator- (const T value, const RGBColor rhs) noexcept
        {
            return rhs - value;
        }

        /** \brief Subtracts RGBColor - one 3-bytes buffer. */
        template<typename T>
            requires std::is_arithmetic_v<T>
        friend inline RGBColor operator- (const RGBColor& lhs, const unsigned char rhs[3]) noexcept
        {
            return RGBColor(lhs.r - rhs[0], lhs.g - rhs[1], lhs.b - rhs[2]);
        }

        /** \brief Subtracts one 3-bytes buffer - RGBColor. */
        template<typename T>
            requires std::is_arithmetic_v<T>
        friend inline RGBColor operator- (const unsigned char lhs[3], const RGBColor& rhs) noexcept
        {
            return RGBColor(rhs.r - lhs[0], rhs.g - lhs[1], rhs.b - lhs[2]);
        }

        /** \brief Subtracts RGBColor - one std::vector. */
        template<typename T>
            requires std::is_arithmetic_v<T>
        friend inline RGBColor operator- (RGBColor lhs, const std::vector<T>& rhs) noexcept(false)
        {
            assert(rhs.size() == 3);
            return lhs -= rhs;
        }

        /** \brief Subtracts one std::vector - RGBColor. */
        template<typename T>
            requires std::is_arithmetic_v<T>
        friend inline RGBColor operator- (const std::vector<T>& lhs, RGBColor rhs) noexcept(false)
        {
            assert(lhs.size() == 3);
            return rhs -= lhs;
        }

        /** \brief Subtracts RGBColor - one std::array. */
        template<typename T>
            requires std::is_arithmetic_v<T>
        friend inline RGBColor operator- (RGBColor lhs, const std::array<T, 3>& rhs) noexcept
        {
            return lhs -= rhs;
        }

        /** \brief Subtracts one std::array - RGBColor. */
        template<typename T>
            requires std::is_arithmetic_v<T>
        friend inline RGBColor operator- (const std::array<T, 3>& lhs, RGBColor rhs) noexcept
        {
            return rhs -= lhs;
        }


        //---   Magnifying colors   -----------------------------------------
        /** \brief In-place multiplies (one single factor). */
        template<typename T>
            requires std::is_arithmetic_v<T>
        RGBColor& operator*= (const T value) noexcept
        {
            set(r * value, g * value, b * value);
            return *this;
        }

        /** \brief In-place multiplies one 3*bytes buffer. */
        template<typename T>
            requires std::is_arithmetic_v<T>
        RGBColor& operator*= (const T rhs[3]) noexcept
        {
            set(r * rhs[0], g * rhs[1], b * rhs[2]);
            return *this;
        }

        /** \brief In-place multiplies one std::vector. */
        template<typename T>
            requires std::is_arithmetic_v<T>
        RGBColor& operator*= (const std::vector<T>& rhs) noexcept(false)
        {
            assert(rhs.size() == 3);
            set(r * rhs.r, g * rhs.g, b * rhs.b);
            return *this;
        }

        /** \brief In-place multiplies one std::array. */
        template<typename T>
            requires std::is_arithmetic_v<T>
        RGBColor& operator*= (const std::array<T, 3>& rhs) noexcept
        {
            set(r * rhs.r, g * rhs.g, b * rhs.b);
            return *this;
        }

        /** \brief Multiplies RGBColor * one single term. */
        template<typename T>
            requires std::is_arithmetic_v<T>
        inline RGBColor operator* (const T value) const noexcept
        {
            return RGBColor(r * value, g * value, b * value);
        }

        /** \brief Multiplies one single term * RGBColor. */
        template<typename T>
            requires std::is_arithmetic_v<T>
        friend inline RGBColor operator* (const T value, const RGBColor rhs) noexcept
        {
            return rhs * value;
        }

        /** \brief Multiplies RGBColor * one 3*bytes buffer. */
        template<typename T>
            requires std::is_arithmetic_v<T>
        friend inline RGBColor operator* (const RGBColor& lhs, const unsigned char rhs[3]) noexcept
        {
            return RGBColor(lhs.r * rhs[0], lhs.g * rhs[1], lhs.b * rhs[2]);
        }

        /** \brief Multiplies one 3*bytes buffer * RGBColor. */
        template<typename T>
            requires std::is_arithmetic_v<T>
        friend inline RGBColor operator* (const unsigned char lhs[3], const RGBColor& rhs) noexcept
        {
            return RGBColor(rhs.r * lhs[0], rhs.g * lhs[1], rhs.b * lhs[2]);
        }

        /** \brief Multiplies RGBColor * one std::vector. */
        template<typename T>
            requires std::is_arithmetic_v<T>
        friend inline RGBColor operator* (RGBColor lhs, const std::vector<T>& rhs) noexcept(false)
        {
            assert(rhs.size() == 3);
            return lhs *= rhs;
        }

        /** \brief Multiplies one std::vector * RGBColor. */
        template<typename T>
            requires std::is_arithmetic_v<T>
        friend inline RGBColor operator* (const std::vector<T>& lhs, RGBColor rhs) noexcept(false)
        {
            assert(lhs.size() == 3);
            return rhs *= lhs;
        }

        /** \brief Multiplies RGBColor * one std::array. */
        template<typename T>
            requires std::is_arithmetic_v<T>
        friend inline RGBColor operator* (RGBColor lhs, const std::array<T, 3>& rhs) noexcept
        {
            return lhs *= rhs;
        }

        /** \brief Multiplies one std::array * RGBColor. */
        template<typename T>
            requires std::is_arithmetic_v<T>
        friend inline RGBColor operator* (const std::array<T, 3>& lhs, RGBColor rhs) noexcept
        {
            return rhs *= lhs;
        }


        //---   Dividing colors   -------------------------------------------
        /** \brief In-place divides (one single factor). */
        template<typename T>
            requires std::is_arithmetic_v<T>
        RGBColor& operator/= (const T value) noexcept(false)
        {
            assert(value > 0);
            set(_div(r, value), _div(g, value), _div(b, value));
            return *this;
        }

        /** \brief In-place divides one 3*bytes buffer. */
        template<typename T>
            requires std::is_arithmetic_v<T>
        RGBColor& operator/= (const T rhs[3]) noexcept(false)
        {
            set(_div(r, rhs[0]), _div(g, rhs[1]), _div(b, rhs[2]));
            return *this;
        }

        /** \brief In-place divides one std::vector. */
        template<typename T>
            requires std::is_arithmetic_v<T>
        RGBColor& operator/= (const std::vector<T>& rhs) noexcept(false)
        {
            assert(rhs.size() == 3);
            set(_div(r, rhs[0]), _div(g, rhs[1]), _div(b, rhs[2]));
            return *this;
        }

        /** \brief In-place divides one std::array. */
        template<typename T>
            requires std::is_arithmetic_v<T>
        RGBColor& operator/= (const std::array<T, 3>& rhs) noexcept(false)
        {
            set(_div(r, rhs[0]), _div(g, rhs[1]), _div(b, rhs[2]));
            return *this;
        }

        /** \brief Divides RGBColor / one single term. */
        template<typename T>
            requires std::is_arithmetic_v<T>
        inline RGBColor operator/ (const T value) const noexcept(false)
        {
            return RGBColor(_div(r, value), _div(g, value), _div(b, value));
        }

        /** \brief Divides one single term / RGBColor. */
        template<typename T>
            requires std::is_arithmetic_v<T>
        friend inline RGBColor operator/ (const T value, const RGBColor rhs) noexcept(false)
        {
            return RGBColor(_div(r, rhs.r), _div(g, rhs.g), _div(b, rhs.b));
        }

        /** \brief Divides RGBColor / one 3*bytes buffer. */
        template<typename T>
            requires std::is_arithmetic_v<T>
        friend inline RGBColor operator/ (const RGBColor& lhs, const unsigned char rhs[3]) noexcept(false)
        {
            return RGBColor(_div(lhs.r, rhs[0]), _div(lhs.g, rhs[1]), _div(lhs.b, rhs[2]));
        }

        /** \brief Divides one 3*bytes buffer / RGBColor. */
        template<typename T>
            requires std::is_arithmetic_v<T>
        friend inline RGBColor operator/ (const unsigned char lhs[3], const RGBColor& rhs) noexcept(false)
        {
            return RGBColor(_div(lhs[0], rhs.r), _div(lhs[1], rhs.g), _div(lhs[2], rhs.b));
        }

        /** \brief Divides RGBColor * one std::vector. */
        template<typename T>
            requires std::is_arithmetic_v<T>
        friend inline RGBColor operator/ (RGBColor lhs, const std::vector<T>& rhs) noexcept(false)
        {
            assert(rhs.size() == 3);
            return lhs /= rhs;
        }

        /** \brief Divides one std::vector * RGBColor. */
        template<typename T>
            requires std::is_arithmetic_v<T>
        friend inline RGBColor operator/ (const std::vector<T>& lhs, RGBColor rhs) noexcept(false)
        {
            assert(lhs.size() == 3);
            return RGBColor(_div(lhs[0], rhs.r), _div(lhs[1], rhs.g), _div(lhs[2], rhs.b));
        }

        /** \brief Divides RGBColor * one std::array. */
        template<typename T>
            requires std::is_arithmetic_v<T>
        friend inline RGBColor operator/ (RGBColor lhs, const std::array<T, 3>& rhs) noexcept(false)
        {
            return lhs /= rhs;
        }

        /** \brief Divides one std::array * RGBColor. */
        template<typename T>
            requires std::is_arithmetic_v<T>
        friend inline RGBColor operator/ (const std::array<T, 3>& lhs, RGBColor rhs) noexcept(false)
        {
            return RGBColor(_div(lhs[0], rhs.r), _div(lhs[1], rhs.g), _div(lhs[2], rhs.b));
        }


        //---   Data   ------------------------------------------------------
        union {
            unsigned char comp[3];
            struct {
                unsigned char b;
                unsigned char g;
                unsigned char r;
            };
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


    private:
        /** \brief Returns the clipped value acccording to color components type. */
        template<typename T>
            requires std::is_arithmetic_v<T>
        inline static const unsigned char _clipped(const T value) noexcept
        {
            return (value >= 255) ? 255 : ((value <= 0) ? 0 : (unsigned char)value);
        }

        /** \brief Returns the rounding value of a division. */
        template<typename T, typename U>
            requires std::is_arithmetic_v<T> && std::is_arithmetic_v<U>
        inline static const unsigned char _div(const T num, const U den) noexcept(false)
        {
            assert(den > 0);
            return (unsigned char)(float(num) / float(den));
        }

    };


    const RGBColor RGBColor::ANTHRACITE(31, 31, 31);
    const RGBColor RGBColor::BLACK(0, 0, 0);
    const RGBColor RGBColor::BLUE(0, 0, 255);
    const RGBColor RGBColor::BROWN(95, 47, 0);
    const RGBColor RGBColor::DARK_RED(127, 0, 0);
    const RGBColor RGBColor::DEEP_GRAY(63, 63, 63);
    const RGBColor RGBColor::DEEP_GREEN(0, 95, 0);
    const RGBColor RGBColor::GRAY(127, 127, 127);
    const RGBColor RGBColor::LIGHT_BLUE(0, 255, 255);
    const RGBColor RGBColor::LIGHT_GRAY(191, 191, 191);
    const RGBColor RGBColor::LIGHT_GREEN(0, 255, 0);
    const RGBColor RGBColor::NAVY_BLUE(0, 0, 63);
    const RGBColor RGBColor::ORANGE(255, 127, 0);
    const RGBColor RGBColor::RED(255, 0, 0);
    const RGBColor RGBColor::YELLOW(255, 255, 0);
    const RGBColor RGBColor::WHITE(255, 255, 255);

    const RGBColor RGBColor::TARGET_WHITE(255, 255, 255);
    const RGBColor RGBColor::TARGET_BLACK(0, 0, 0);
    const RGBColor RGBColor::TARGET_BLUE(65, 181, 200);
    const RGBColor RGBColor::TARGET_RED(255, 37, 21);
    const RGBColor RGBColor::TARGET_GOLD(255, 245, 55);
    
    const RGBColor RGBColor::TARGET_BLUE_6(17, 165, 255);
    const RGBColor RGBColor::TARGET_BLUE_NFAA(63, 63, 95);
}
