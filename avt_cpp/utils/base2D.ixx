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
#include <cmath>
#include <limits>
#include <vector>

export module utils.base2d;


//===========================================================================
export namespace avt::utils
{
    //=======================================================================
    /** @brief The base class for most of 2D objects.
    */
    class Base2D
    {
    public:
        using ValueType = short;  //!< The generic type for any 2D components

        //---   Constructors / Destructor   ---------------------------------
        /** @brief Default constructor. */
        inline Base2D() noexcept
            : x{ ValueType{0} },
              y{ ValueType{0} }
        {}

        /** @brief Valued constructor. */
        template<typename X, typename Y>
            requires std::is_arithmetic_v<X> && std::is_arithmetic_v<Y>
        inline Base2D(const X x_, const Y y_)  noexcept
            : x{ ValueType{0} },
              y{ ValueType{0} }
        {
            x = _clipped(x_);
            y = _clipped(y_);
        }

        /** @brief Constructor (2-components buffer). */
        template<typename T>
            requires std::is_arithmetic_v<T>
        inline Base2D(const T buffer[2]) noexcept
            : x{ ValueType{0} },
              y{ ValueType{0} }
        {
            x = _clipped(buffer[0]);
            y = _clipped(buffer[1]);
        }

        /** @brief Constructor (std::vector). */
        template<typename T>
            requires std::is_arithmetic_v<T>
        inline Base2D(const std::vector<T> vect) noexcept(false)
            : x{ ValueType{0} },
              y{ ValueType{0} }
        {
            assert(vect.size() >= 2);
            x = _clipped(vect[0]);
            y = _clipped(vect[1]);
        }

        /** @brief Constructor (std::array). */
        template<typename T>
            requires std::is_arithmetic_v<T>
        inline Base2D(const std::array<T, 2> arr) noexcept(false)
            : x{ ValueType{0} },
              y{ ValueType{0} }
        {
            x = _clipped(arr[0]);
            y = _clipped(arr[1]);
        }


        /** @brief Copy and Move constructors. */
        Base2D(const Base2D&) noexcept = default;
        Base2D(Base2D&&) noexcept = default;

        /** @brief Destructor. */
        virtual ~Base2D() noexcept = default;


        //---   Assignments   -----------------------------------------------
        /** @brief Copy and Move assignments. */
        virtual Base2D& operator= (const Base2D&) noexcept = default;
        virtual Base2D& operator= (Base2D&&) noexcept = default;

        /** @brief Assignment operator (one 2-bytes buffer). */
        template<typename T>
            requires std::is_arithmetic_v<T>
        inline Base2D& operator= (const T buffer[2]) noexcept
        {
            x = _clipped(buffer[0]);
            y = _clipped(buffer[1]);
            return *this;
        }

        /** @brief Assignment operator (std::vector). */
        template<typename T>
            requires std::is_arithmetic_v<T>
        inline Base2D& operator= (const std::vector<T>& vect) noexcept(false)
        {
            assert(vect.size() >= 2);
            x = _clipped(vect[0]);
            y = _clipped(vect[1]);
            return *this;
        }

        /** @brief Assignment operator (std::array). */
        template<typename T>
            requires std::is_arithmetic_v<T>
        inline Base2D& operator= (const std::array<T, 2>& arr) noexcept
        {
            x = _clipped(arr[0]);
            y = _clipped(arr[1]);
            return *this;
        }


        //---   Comparisons   -----------------------------------------------
        /** @brief Returns true if both coordinates are the same. */
        inline const bool operator==(const Base2D& rhs) const noexcept
        {
            return x == rhs.x && y == rhs.y;
        }

        /** @brief Returns true if any of x or y coordinates are not the same. */
        inline const bool operator!=(const Base2D& rhs) const noexcept
        {
            return !(*this == rhs);
        }


        //---   Adding   ----------------------------------------------------
        /** @brief In-place adds a 2D-components. */
        inline Base2D& operator+= (const Base2D& rhs) noexcept
        {
            x = _clipped(ConvertedType(x) + ConvertedType(rhs.x));
            y = _clipped(ConvertedType(y) + ConvertedType(rhs.y));
            return *this;
        }

        /** @brief In-place adds one std::vector. */
        template<typename T>
            requires std::is_arithmetic_v<T>
        inline Base2D& operator+= (const std::vector<T>& rhs) noexcept(false)
        {
            assert(rhs.size() >= 2);
            return *this += Base2D(rhs[0], rhs[1]);
        }

        /** @brief In-place adds one std::array. */
        template<typename T>
            requires std::is_arithmetic_v<T>
        Base2D& operator+= (const std::array<T, 2>& rhs) noexcept
        {
            return *this += Base2D(rhs[0], rhs[1]);
        }

        /** @brief Adds Base2D + Base2D. */
        friend inline Base2D operator+ (Base2D lhs, const Base2D& rhs) noexcept
        {
            return lhs += rhs;
        }

        /** @brief Adds Base2D + one std::vector. */
        template<typename T>
            requires std::is_arithmetic_v<T>
        friend inline Base2D operator+ (Base2D lhs, const std::vector<T>& rhs) noexcept(false)
        {
            return lhs += rhs;
        }

        /** @brief Adds one std::vector + Base2D. */
        template<typename T>
            requires std::is_arithmetic_v<T>
        friend inline Base2D operator+ (const std::vector<T>& lhs, Base2D rhs) noexcept(false)
        {
            return rhs += lhs;
        }

        /** @brief Adds Base2D + one std::array. */
        template<typename T>
            requires std::is_arithmetic_v<T>
        friend inline Base2D operator+ (Base2D lhs, const std::array<T, 2>& rhs) noexcept
        {
            return lhs += rhs;
        }

        /** @brief Adds one std::array + Base2D. */
        template<typename T>
            requires std::is_arithmetic_v<T>
        friend inline Base2D operator+ (const std::array<T, 2>& lhs, Base2D rhs) noexcept
        {
            return rhs += lhs;
        }


        //---   Subtracting   -----------------------------------------------
        /** @brief In-place subtracts a 2D-components. */
        inline Base2D& operator-= (const Base2D& rhs) noexcept
        {
            x = _clipped(ConvertedType(x) - ConvertedType(rhs.x));
            y = _clipped(ConvertedType(y) - ConvertedType(rhs.y));
            return *this;
        }

        /** @brief In-place subtracts one std::vector. */
        template<typename T>
            requires std::is_arithmetic_v<T>
        inline Base2D& operator-= (const std::vector<T>& rhs) noexcept(false)
        {
            return *this -= Base2D(rhs[0], rhs[1]);
        }

        /** @brief In-place subtracts one std::array. */
        template<typename T>
            requires std::is_arithmetic_v<T>
        Base2D& operator-= (const std::array<T, 2>& rhs) noexcept
        {
            return *this -= Base2D(rhs[0], rhs[1]);
        }

        /** @brief subtracts Base2D - Base2D. */
        friend inline Base2D operator- (Base2D lhs, const Base2D& rhs) noexcept
        {
            return lhs -= rhs;
        }

        /** @brief subtracts Base2D - one std::vector. */
        template<typename T>
            requires std::is_arithmetic_v<T>
        friend inline Base2D operator- (Base2D lhs, const std::vector<T>& rhs) noexcept(false)
        {
            return lhs -= rhs;
        }

        /** @brief subtracts one std::vector - Base2D. */
        template<typename T>
            requires std::is_arithmetic_v<T>
        friend inline Base2D operator- (const std::vector<T>& lhs, Base2D rhs) noexcept(false)
        {
            return Base2D(lhs) -= rhs;
        }

        /** @brief subtracts Base2D - one std::array. */
        template<typename T>
            requires std::is_arithmetic_v<T>
        friend inline Base2D operator- (Base2D lhs, const std::array<T, 2>& rhs) noexcept
        {
            return lhs -= rhs;
        }

        /** @brief subtracts one std::array - Base2D. */
        template<typename T>
            requires std::is_arithmetic_v<T>
        friend inline Base2D operator- (const std::array<T, 2>& lhs, Base2D rhs) noexcept
        {
            return Base2D(lhs) -= rhs;
        }


        //---   Magnifying   ------------------------------------------------
        /** @brief In-place multiplies (one single factor). */
        template<typename T>
            requires std::is_arithmetic_v<T>
        inline Base2D& operator*= (const T factor) noexcept
        {
            x = _clipped(std::lround(double(x) * factor));
            y = _clipped(std::lround(double(y) * factor));
            return *this;
        }

        /** @brief Mulitplies by a factor (post-). */
        template<typename T>
            requires std::is_arithmetic_v<T>
        inline friend Base2D operator* (const Base2D& lhs, const T rhs) noexcept
        {
            Base2D tmp{ lhs };
            return tmp *= rhs;
        }

        /** @brief Mulitplies by a factor (pre-). */
        template<typename T>
            requires std::is_arithmetic_v<T>
        inline friend Base2D operator* (const T lhs, const Base2D& rhs) noexcept
        {
            return rhs * lhs;
        }


        //---   Dividing   --------------------------------------------------
        /** @brief In-place divides (one single factor). */
        template<typename T>
            requires std::is_arithmetic_v<T>
        inline Base2D operator/= (const T factor) noexcept(false)
        {
            assert(factor > 0);
            return *this *= 1.0 / factor;;
        }

        /** @brief Divides by a factor (post-). */
        template<typename T>
            requires std::is_arithmetic_v<T>
        inline friend Base2D operator/ (const Base2D lhs, const T rhs) noexcept(false)
        {
            Base2D tmp{ lhs };
            return tmp /= rhs;
        }


        //---   Data   ------------------------------------------------------
        ValueType x;  //!< the X-axis coordinate 
        ValueType y;  //!< the Y-axis coordinate


    protected:
        using ConvertedType = long;  //!< the intetrnal type used to convert values before clipping

        /** @brief Clips value in 'ValueType' integer range. */
        template<typename T>
        requires std::is_arithmetic_v<T>
        const ValueType _clipped(const T value) const noexcept
        {
            if (std::is_integral_v<T>)
                return ValueType(std::clamp<ConvertedType>(ConvertedType(value),
                                 ConvertedType(std::numeric_limits<ValueType>::min()),
                                 ConvertedType(std::numeric_limits<ValueType>::max())));
            else
                return ValueType(std::clamp<ConvertedType>(std::lround(value),
                                 ConvertedType(std::numeric_limits<ValueType>::min()),
                                 ConvertedType(std::numeric_limits<ValueType>::max())));

        }
    };

}
