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

#include <opencv2/core/types.hpp>

#include "utils/types.h"


//===========================================================================
export module utils.range;

import utils;

//===========================================================================
export namespace avt::utils
{
    //=======================================================================
    /** @brief The class of 2D sizes.
    */
    class Range : public cv::Range
        /* Notice: Due to inheritance, gets access to
        *  - bool   empty() const
        *  - int    size() const
        *  - static Range all()
        *  - int end
        *  - int start
        */
    {
    public:
        //-------------------------------------------------------------------
        using MyBaseType = cv::Range;  //!< wrapper to the base class


        //---   Constructors / Destructor   ---------------------------------
        /** @brief Empty constructor. */
        inline Range() noexcept
            : MyBaseType{}
        {}

        /** @brief Constructor by values (x, y). */
        template<typename S, typename E>
            requires std::is_arithmetic_v<S> && std::is_arithmetic_v<E>
        inline Range(const S start, const E end) noexcept
            : MyBaseType{ start, end }
        {}

        /** @brief Constructor (2-components containers). */
        template<typename P>
            requires avt::is_pair_type_v<P>
        inline Range(const P& pair) noexcept
            : MyBaseType{ std::clamp<int>(pair[0], _MIN_INT, _MAX_INT),
                          std::clamp<int>(pair[1], _MIN_INT, _MAX_INT) }
        {}

        /** @brief Default Copy Constructor. */
        Range(const Range&) noexcept = default;

        /** @brief Default Move COnstructor. */
        Range(Range&&) noexcept(false) = default;

        /** @brief Default Destructor. */
        virtual ~Range() noexcept = default;


        //---   Assignments   -----------------------------------------------
        /** @brief Default Copy assignment. */
        inline Range& operator=(const Range&) noexcept = default;

        /** @brief Default Move assignment. */
        inline Range& operator=(Range&&) noexcept = default;

        /** @brief Assignment operator (2-components container). */
        template<typename P>
            requires avt::is_pair_type_v<P>
        inline Range& operator= (const P& rhs) noexcept
        {
            start = std::clamp<int>(rhs[0], _MIN_INT, _MAX_INT);
            end   = std::clamp<int>(rhs[1], _MIN_INT, _MAX_INT);
        }

        //---   Comparisons   -----------------------------------------------
        /** @brief Returns true if sizes are the same, or false otherwise. */
        inline const bool operator== (const Range& rhs) const noexcept
        {
            return start == rhs.start && end == rhs.end;
        }

        /** @brief Returns true if this size and 2-components container are the ame, or false otherwise. */
        template<typename P>
            requires avt::is_pair_type_v<P>
        inline const bool operator== (const P& rhs) const noexcept
        {
            start == rhs[0];
            end == rhs[1];
        }

        /** @brief Returns true if sizes are not the same, or false otherwise. */
        inline const bool operator!= (const Range& rhs) const noexcept
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
            return (unsigned long)start * (unsigned long)end;
        }


    private:
        static constexpr int _MIN_INT{ std::numeric_limits<int>::min() };
        static constexpr int _MAX_INT{ std::numeric_limits<int>::max() };
    };

}
