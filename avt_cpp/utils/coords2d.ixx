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
#include <tuple>
#include <utility>
#include <vector>

#include <opencv2/core/types.hpp>

#include "utils/types.h"


export module utils.coords2d;

import utils;


//===========================================================================
export namespace avt::utils
{
    //=======================================================================
    /** @brief The class of 2D short integer coordinates. */
    class Coords2D : public cv::Point_<avt::CoordsType>
    {
    public:

        //-------------------------------------------------------------------
        using MyBaseType = cv::Point_<avt::CoordsType>; //!< wrapper to the base class


        //--- Constructors / Destructors ------------------------------------
        /** @brief Default constructor. */
        inline Coords2D() noexcept
            : MyBaseType()
        {}

        /** @brief Constructor (2 values). */
        template<typename X, typename Y>
        inline Coords2D(const X x, const Y y,
                        ENSURE_IS_ARITHMETIC(X),
                        ENSURE_IS_ARITHMETIC(Y)) noexcept
            : MyBaseType(avt::utils::clamp(x), avt::utils::clamp(y))
        {}

        /** @brief Constructor (2-components container). */
        template<typename P>
        inline Coords2D(const P pair, ENSURE_IS_PAIR(P)) noexcept(false)
            : MyBaseType(avt::utils::clamp(pair[0]), avt::utils::clamp(pair[1]))
        {}

        /** @brief Default Copy constructor. */
        Coords2D(const Coords2D&) noexcept = default;

        /** @brief Default Move coonstructor. */
        Coords2D(Coords2D&&) noexcept(false) = default;

        /** @brief Default destructor. */
        virtual ~Coords2D() noexcept = default;


        //--- Assignments ---------------------------------------------------
        /** @brief Default Copy assignment. */
        Coords2D& operator=(const Coords2D&) noexcept = default;

        /** @brief Default Move assignment. */
        Coords2D& operator=(Coords2D&&) noexcept = default;

        /** @brief Copy Assignment (2-components container). */
        template<typename P>
            requires avt::is_pair_type_v<P>
        inline Coords2D& operator= (const P pair) noexcept
        {
            x = avt::utils::clamp(pair[0]);
            y = avt::utils::clamp(pair[1]);
            return *this;
        }
    };

}