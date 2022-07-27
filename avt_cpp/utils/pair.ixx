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

#include <format>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <utility>

#include "utils/types.h"


export module types.pair;

import utils;

//===========================================================================
export namespace avt {

    //=======================================================================
    template<typename T>
        requires std::is_arithmetic_v<T>
    class Pair_;  // just a forward declaration

    using Pair = Pair_<avt::CoordsType>;  //!< i.e. pairs of short integers

    /** @brief States the Pair status of a Pair type. */
    template<typename _T>
    struct is_pair_type<avt::Pair_<_T>> {
        static const bool value = std::is_arithmetic_v<_T>;
    };


    //=======================================================================
    /** @brief the templated class for pairs in applicatoin avt. */
    template<typename T>
        requires std::is_arithmetic_v<T>
    class Pair_ : public std::pair<T, T>
    {
    public:
        //-------------------------------------------------------------------
        using MyBaseClass = std::pair<T, T>;  //!< Wrapper to the base class


        //--- Constructors / Destructor -------------------------------------
        /** @brief Default constructor. */
        inline Pair_() noexcept
            : MyBaseClass{}
        {}

        /** @brief Constructor (2 values). */
        template<typename F, typename S>
            requires std::is_arithmetic_v<F> && std::is_arithmetic_v<S>
        inline Pair_(const F first, const S second) noexcept
            : MyBaseClass{ avt::utils::clamp<T, F>(first),
                           avt::utils::clamp<T, S>(second) }
        {}

        /** @brief Constructor (2-components containers). */
        template<typename P>
            requires avt::is_pair_type_v<P>
        inline Pair_(const P& other) noexcept(false)
            : MyBaseClass{ avt::utils::clamp<T, decltype(other[0])>(other[0]),
                           avt::utils::clamp<T, decltype(other[1])>(other[1]) }
        {}


        //--- Indexing ------------------------------------------------------
        /** @brief Indexed accessor. */
        const T operator[] (const size_t index) const noexcept(false)
        {
            switch (index) {
            case 0:
                return this->first;  // inherited from base class
            case 1:
                return this->second; // inherited from base class
            default:
                throw(index_exception(index));
            }
        }

        /** @brief Indexed mutator. */
        T& operator[] (const size_t index) noexcept(false)
        {
            switch (index) {
            case 0:
                return this->first;  // inherited from base class
            case 1:
                return this->second; // inherited from base class
            default:
                throw(index_exception(index));
            }
        }

        /** @brief Default Copy constructor. */
        Pair_(const Pair_&) noexcept = default;

        /** @brief Default Move constructor. */
        Pair_(Pair_&&) noexcept = default;

        /** @brief Default destructor. */
        virtual ~Pair_() noexcept = default;


        //--- Assignements --------------------------------------------------
        /** @brief Default Copy assignment. */
        Pair_& operator= (const Pair_&) noexcept = default;

        /** @brief Default Move assignment. */
        Pair_& operator= (Pair_&&) noexcept = default;

        /** @brief Default 2-components container assignment. */
        template<typename P>
            requires avt::is_pair_type_v<P>
        Pair_& operator= (const P& other)
        {
            this->first  = avt::utils::clamp<T, decltype(other.first )>(other.first );
            this->second = avt::utils::clamp<T, decltype(other.second)>(other.second);
            return *this;
        }


    private:
        /** @brief private internal exception to be thrown on erroneous indexing. */
        inline static std::out_of_range& index_exception(const size_t index) noexcept(false)
        {
            constexpr std::string error_format =
                "ERROR: trying to index a pair with index value = {} (should be either 0 or 1)";
            return std::out_of_range(std::format(error_format, index));
        }
    };

}
