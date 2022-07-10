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
#include <limits>

#include "utils/types.h"

export module utils;

export namespace avt::utils
{
    /** @brief Clamping values - generic form. */
    template<typename T, typename U>
        requires std::is_arithmetic_v<T> && std::is_arithmetic_v<U>
    const T clamp_(const U& value, const T _MIN = std::numeric_limits<T>::lowest()) noexcept
    {
        constexpr T _MAX = std::numeric_limits<T>::max();

        if (std::numeric_limits<U>::is_integer) {
            if (value <= _MIN)
                return T(_MIN);
            if (value >= _MAX)
                return T(_MAX);
            return T(value);
        }
        else {
            const U val = value + U(0.5);
            if (val <= _MIN)
                return T(_MIN);
            if (val >= _MAX)
                return T(_MAX);
            return T(val);
        }
    }

    /** @brief Clamping avt::CoordsType values. */
    template<typename U>
        requires std::is_arithmetic_v<U>
    inline const avt::CoordsType clamp(const U& value) noexcept
    {
        return avt::utils::clamp_<avt::CoordsType, U>(value);
    }

    /** @brief Clamping unsigned short values. */
    template<typename U>
        requires std::is_arithmetic_v<U>
    inline const unsigned short clamp_us(const U value) noexcept
    {
        return avt::utils::clamp_<unsigned short, U>(value);
    }

    /** @brief Clamping values - generic form. */
    template<typename T, typename U>
        requires std::is_arithmetic_v<T> && std::is_arithmetic_v<U>
    inline const T clamp0_(const U& value) noexcept
    {
        return T(avt::utils::clamp_<U>(value, U(0)));
    }

    /** @brief Clamping avt::CoordsType values. */
    template<typename U>
        requires std::is_arithmetic_v<U>
    inline const avt::CoordsType clamp0(const U& value) noexcept
    {
        return avt::utils::clamp0_<avt::CoordsType, U>(value);
    }

    /** @brief Clamping unsigned short values. */
    template<typename U>
        requires std::is_arithmetic_v<U>
    inline const unsigned clamp0_us(const U value) noexcept
    {
        return avt::utils::clamp0_<unsigned short, U>(value);
    }

}