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
#include <utility>
#include <vector>

#include <opencv2/core/mat.hpp>
#include <opencv2/core/types.hpp>


//===========================================================================
namespace avt
{
    //---   main Types   ----------------------------------------------------
    using Byte         = unsigned char;                 //!< the unsigned 8-bits integer values type
    using CoordsType   = short;                         //!< the type of avt 2D-coordinates
    using DimsType     = unsigned short;                //!< the type of avt dimensions values
    using ImageType    = cv::Mat3b;                     //!< the common base type for frames and views.
    using PixelType    = cv::Vec3b;                     //!< the type of frame pixels

    using CVPoint      = cv::Point_<avt::CoordsType>;   //!< OpenCV-like Point_ wrapper
    using CVRect       = cv::Rect_<avt::CoordsType>;    //!< OpenCV-like Rect_ wrapper
    using CVScalarByte = cv::Scalar_<avt::Byte>;        //!< OpenCV-like Scalar wrapper
    using CVSize       = cv::Size_<avt::DimsType>;      //!< OpenCV-like Size_ wrapper


    //---   is_pair_type   --------------------------------------------------
    /** @brief States the Pair status of a type - always false unless specified as true in specializations. */
    template<typename T>
    struct is_pair_type
    {
        static constexpr bool value = false;
    };

    /** @brief Wrapper to the value of avt::is_pair_type. */
    template<typename T>
    inline constexpr bool is_pair_type_v = is_pair_type<T>::value;

    /** @brief States that buffers containing two arithmetic values are a Pair. */
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


    //---   is_container3_type   --------------------------------------------
    /** @brief States the at least 3-components container status of a type - always false unless specified as true in specializations. */
    template<typename T>
    struct is_container3_type
    {
        static constexpr bool value = false;
    };

    /** @brief Wrapper to the value of avt::is_container3_type. */
    template<typename T>
    inline constexpr bool is_container3_type_v = is_container3_type<T>::value;

    /** @brief States that buffers containing three arithmetic values are of a 3-components container type. */
    template<typename _T>
    struct is_container3_type<_T[3]>
    {
        static constexpr bool value = std::is_arithmetic_v<_T>;
    };

    /** @brief States that std::vector with t least 3 components is of a 3-components container type. */
    template<typename _T>
    struct is_container3_type<std::vector<_T>>
    {
        static constexpr bool value = std::is_arithmetic_v<_T>;  // CAUTION: vectors of size < 3 may generate exceptions when used
    };

    /** @brief States that std::array of size 3 is of a 3-components container type. */
    template<typename _T>
    struct is_container3_type<std::array<_T, 3>>
    {
        static constexpr bool value = std::is_arithmetic_v<_T>;
    };

}
