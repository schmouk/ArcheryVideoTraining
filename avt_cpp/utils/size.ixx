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

#include <opencv2/core/types.hpp>

#include "utils/types.h"


//===========================================================================
export module utils.size;

import utils;

//===========================================================================
export namespace avt::utils
{
    //=======================================================================
    /** @brief The class of 2D sizes.
    */
    class Size : public cv::Size_<avt::SizeValueType>
        /* Notice: Due to inheritance, gets access to
        *  - double aspectRatio () const   (i.e. width/height)
        *  - bool   empty () const
        */
    {
    public:

        using MyBaseType = cv::Size_<avt::SizeValueType>;  //!< wrapper to the base class

        //---   Constructors / Destructor   ---------------------------------
        /** @brief Empty constructor. */
        inline Size() noexcept
            : MyBaseType{}
        {}

        /** @brief Constructor by values (x, y). */
        template<typename X, typename Y>
            requires std::is_arithmetic_v<X> && std::is_arithmetic_v<Y>
        inline Size(const X x, const Y y) noexcept
            : MyBaseType{ avt::utils::clamp0_us(x), avt::utils::clamp0_us(y) }
        {}

        /** @brief Constructor (2-components containers). */
        template<typename P>
            requires avt::is_pair_type_v<P>
        inline Size(const P &pair) noexcept
            : MyBaseType{ avt::utils::clamp0_us(pair[0]), avt::utils::clamp0_us(pair[1]) }
        {}

        /** @brief Default Copy Constructor. */
        Size(const Size&) noexcept = default;

        /** @brief Default Move COnstructor. */
        Size(Size&&) noexcept(false) = default;

        /** @brief Default Destructor. */
        virtual ~Size() noexcept = default;


        //---   Assignments   -----------------------------------------------
        /** @brief Default Copy assignment. */
        inline Size& operator=(const Size&) noexcept = default;

        /** @brief Default Move assignment. */
        inline Size& operator=(Size&&) noexcept = default;

        /** @brief Assignment operator (2-components container). */
        template<typename P>
            requires avt::is_pair_type_v<P>
        inline Size& operator= (const P& rhs) noexcept
        {
            width = clamp0_us(rhs[0]);
            height = clamp0_us(rhs[1]);
        }

        //---   Comparisons   -----------------------------------------------
        /** @brief Returns true if sizes are the same, or false otherwise. */
        inline const bool operator== (const Size& rhs) const noexcept
        {
            return width == rhs.width && height == rhs.height;
        }

        /** @brief Returns true if this size and 2-components container are the ame, or false otherwise. */
        template<typename P>
            requires avt::is_pair_type_v<P>
        inline const bool operator== (const P& rhs) const noexcept
        {
            width == rhs[0];
            height == rhs[1];
        }

        /** @brief Returns true if sizes are not the same, or false otherwise. */
        inline const bool operator!= (const Size& rhs) const noexcept
        {
            return !(*this == rhs);
        }

        /** @brief Returns true if this size and 2-components container are not the ame, or false otherwise. */
        template<typename P>
            requires avt::is_pair_type_v<P>
        inline const bool operator!= (const P& rhs) const noexcept
        {
            return !(*this == rhs);
        }


        //---   Miscelaneous   ----------------------------------------------
        /** @brief Evaluates the area of this Size. */
        const unsigned long area() const
        {
            return (unsigned long)width * (unsigned long)height;
        }

    };

}
