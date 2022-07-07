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
    inline const T clamp_(const U value,
                          ENSURE_IS_ARITHMETIC(T),
                          ENSURE_IS_ARITHMETIC(U)) noexcept
    {
        return T{ std::clamp<U>(value,
                                U(std::numeric_limits<T>::lower()),
                                U(std::numeric_limits<T>::max())) };
    }

    /** @brief Clamping avt::CoordsType values. */
    template<typename U>
    inline const avt::CoordsType clamp(const U value, ENSURE_IS_ARITHMETIC(U)) noexcept
    {
        return avt::utils::clamp_<avt::CoordsType, U>(value);
    }

    /** @brief Clamping unsigned short values. */
    template<typename U>
    inline const unsigned short clamp_us(const U value, ENSURE_IS_ARITHMETIC(U)) noexcept
    {
        return avt::utils::clamp_<unsigned short, U>(value);
    }

    /** @brief Clamping values - generic form. */
    template<typename T, typename U>
    inline const T clamp0_(const U value,
                           ENSURE_IS_ARITHMETIC(T),
                           ENSURE_IS_ARITHMETIC(U)) noexcept
    {
        return T{ std::clamp<U>(value, U{ 0 }, U{ std::numeric_limits<T>::max() }) };
    }

    /** @brief Clamping avt::CoordsType values. */
    template<typename U>
    inline const avt::CoordsType clamp0(const U value, ENSURE_IS_ARITHMETIC(U)) noexcept
    {
        return avt::utils::clamp0_<avt::CoordsType, U>(value);
    }

    /** @brief Clamping unsigned short values. */
    template<typename U>
    inline const unsigned clamp0_us(const U value, ENSURE_IS_ARITHMETIC(U)) noexcept
    {
        return avt::utils::clamp0_<unsigned short, U>(value);
    }

}