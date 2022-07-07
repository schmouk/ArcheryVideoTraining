#pragma once
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
#include <array>
#include <tuple>
#include <utility>
#include <vector>


//===========================================================================
namespace avt
{
    //--- main Types --------------------------------------------------------
    using Byte = unsigned char;  //!< the unsigned 8-bits integer values type
    using CoordsType = short;    //!< avt 2D-coordinates are coded on short integer values


    //--- is_pair_type ------------------------------------------------------
    /** @brief States the Pair status of a type - always false unless specified as true. */
    template<typename T>
    struct is_pair_type
    {
        static constexpr bool value = false;
    };

    /** @brief Wrapper to the value of avt::is_pair_type. */
    template<typename T>
    inline constexpr bool is_pair_type_v = is_pair_type<T>::value;

    /** @brief States that buffers containing two artihmetic values are a Pair. */
    template<typename _T>
    struct is_pair_type<_T[2]>
    {
        static constexpr bool value = std::is_arithmetic_v<_T>;
    };

    /** @brief States that std::vector is a Pair type. */
    template<typename _T>
    struct is_pair_type<std::vector<_T>>
    {
        static constexpr bool value = std::is_arithmetic_v<_T>;  // CAUTION: vectors of size < 2 may generate exceptions when used
    };

    /** @brief States that std::array of size 2 is a Pair type. */
    template<typename _T>
    struct is_pair_type<std::array<_T, 2>>
    {
        static constexpr bool value = std::is_arithmetic_v<_T>;
    };

    /** @brief States that std::tuple containing 2 values is a Pair type. */
    template<typename _T>
    struct is_pair_type<std::tuple<_T, _T>>
    {
        static constexpr bool value = std::is_arithmetic_v<_T>;
    };

}


//=== MACRO DEFINITIONS =====================================================
/** @brief Ensures at compile time the arithmetic status of a type.
* This is an SFINAE implementation
* (\see https://en.wikipedia.org/wiki/Substitution_failure_is_not_an_error).
*/
#define ENSURE_IS_ARITHMETIC(T) typename std::enable_if_t<std::is_arithmetic_v<T>, T>* = nullptr

/** @brief Ensures at compile time the Pair status of a type.
* This is an SFINAE implementation.
*/
#define ENSURE_IS_PAIR(T) typename std::enable_if_t<avt::is_pair_type_v<T>, T>* = nullptr
