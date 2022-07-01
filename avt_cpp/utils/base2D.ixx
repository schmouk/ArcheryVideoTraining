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
        //---   Constructors / Destructor   ---------------------------------
        /** @brief Default constructor. */
        inline Base2D() noexcept
            : x{ 0 }, y{ 0 }
        {}

        /** @brief Valued constructor. */
        inline Base2D(const short x, const short y)
            : x{ x }, y{ y }
        {}

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
            x = short(buffer[0]);
            y = short(buffer[1]);
            return *this;
        }

        /** @brief Assignment operator (std::vector). */
        template<typename T>
            requires std::is_arithmetic_v<T>
        inline Base2D& operator= (const std::vector<T>& vect) noexcept(false)
        {
            assert(vect.size() >= 2);
            x = short(vect[0]);
            y = short(vect[1]);
            return *this;
        }

        /** @brief Assignment operator (std::array). */
        template<typename T>
            requires std::is_arithmetic_v<T>
        inline Base2D& operator= (const std::array<T, 2>& arr) noexcept
        {
            x = short(arr[0]);
            y = short(arr[1]);
            return *this;
        }


        //---   Adding   ----------------------------------------------------
        /** @brief In-place adds a 2D-components. */
        inline Base2D& operator+= (const Base2D& rhs) noexcept
        {
            x = _clipped(int(x) + int(rhs.x));
            y = _clipped(int(y) + int(rhs.y));
            return *this;
        }

        /** @brief In-place adds one std::vector. */
        template<typename T>
            requires std::is_arithmetic_v<T>
        inline Base2D& operator+= (const std::vector<T>& rhs) noexcept(false)
        {
            return this += Base2D(rhs);
        }

        /** @brief In-place adds one std::array. */
        template<typename T>
            requires std::is_arithmetic_v<T>
        Base2D& operator+= (const std::array<T, 2>& rhs) noexcept
        {
            return this += Base2D(rhs);
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
            x = _clipped(int(x) - int(rhs.x));
            y = _clipped(int(y) - int(rhs.y));
            return *this;
        }

        /** @brief In-place subtracts one std::vector. */
        template<typename T>
            requires std::is_arithmetic_v<T>
        inline Base2D& operator-= (const std::vector<T>& rhs) noexcept(false)
        {
            return this -= Base2D(rhs);
        }

        /** @brief In-place subtracts one std::array. */
        template<typename T>
            requires std::is_arithmetic_v<T>
        Base2D& operator-= (const std::array<T, 2>& rhs) noexcept
        {
            return this -= Base2D(rhs);
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
            return Base2D(rhs) -= lhs;
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
            return Base2D(rhs) -= lhs;
        }


        //---   Magnifying   ------------------------------------------------
        /** @brief In-place multiplies (one single factor). */
        template<typename T>
            requires std::is_arithmetic_v<T>
        Base2D& operator*= (const T factor) noexcept
        {
            x = _clipped(std::round(double(x) * factor));
            y = _clipped(std::round(double(y) * factor));
            return *this;
        }


        //---   Dividing   --------------------------------------------------
        /** @brief In-place divides (one single factor). */
        template<typename T>
            requires std::is_arithmetic_v<T>
        Base2D& operator/= (const T factor) noexcept
        {
            assert(factor > 0);
            return *this *= 1.0 / factor;;
        }


        //---   Data   ------------------------------------------------------
        short x;
        short y;


    protected:
        /** @brief Clips value in 'short' integer range. */
        const short _clipped(const int value) const noexcept
        {
            return short(std::clamp<int>(value, int(std::numeric_limits<short>::min()), int(std::numeric_limits<short>::max())));
        }
    };

}
