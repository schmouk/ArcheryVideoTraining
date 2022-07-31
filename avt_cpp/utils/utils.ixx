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
#include <cmath>
#include <limits>

#include "utils/types.h"


export module utils;

export namespace avt::utils
{
    //===   Types Clamping   ================================================
    /** @brief Clamping values - generic form. */
    template<typename T, typename U>
    const T clamp(const U value)
    {
        constexpr U _min = U(std::numeric_limits<T>::lowest());
        constexpr U _max = U(std::numeric_limits<T>::max());
        if (std::is_integral_v<U>) [[likely]] {
            return T(std::clamp(value, _min, _max));
        }
        else [[unlikely]]  {
            const U val = U(std::round(value));
            return T(std::clamp(val, _min, _max));
        }
    }

    /** @brief Clamping values - avt::Byte form. */
    template<typename U>
    inline const avt::Byte clamp_b(const U value)
    {
        return avt::utils::clamp<avt::Byte, U>(value);
    }

    /** @brief Clamping values - short form. */
    template<typename U>
    inline const short clamp_s(const U value)
    {
        return avt::utils::clamp<short, U>(value);
    }

    /** @brief Clamping values - unsigned short form. */
    template<typename U>
    inline const unsigned short clamp_us(const U value)
    {
        return avt::utils::clamp<unsigned short, U>(value);
    }


    //---   Clamping - specialization for type short   ----------------------
    /** @brief clamping (char -> short). */
    template<>
    inline const short clamp<short, char>(const char value)
    {
        return short(value);
    }

    /** @brief clamping (unsigned char -> short). */
    template<>
    inline const short clamp<short, unsigned char>(const unsigned char value)
    {
        return short(value);
    }


    //---   Clamping - specialization for type unsigned short   -------------
    /** @brief clamping (char -> unsigned short). */
    template<>
    inline const unsigned short clamp<unsigned short, char>(const char value)
    {
        return (unsigned short)std::max(char(0), value);
    }

    /** @brief clamping (unsigned char -> unsigned short). */
    template<>
    inline const unsigned short clamp<unsigned short, unsigned char>(const unsigned char value)
    {
        return (unsigned short)std::max(unsigned char(0), value);
    }

}