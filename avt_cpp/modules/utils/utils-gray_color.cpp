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
module utils:gray_color;

import utils.rgb_color;

//===========================================================================
namespace avt::utils
{
    //=======================================================================
    /** \brief The class of GRAY RGB components colors.
    */
    class GrayColor : public avt::utils::RGBColor
    {
    public:
        //---   Constructors / Destructor   ---------------------------------
        /** \brief Default constructor. */
        inline GrayColor() noexcept
            : avt::utils::RGBColor()
        {}

        /** \brief Valued Constructor (one gray value). */
        template<typename T>
            requires std::is_arithmetic_v<T>
        inline GrayColor(const T value) noexcept
            : avt::utils::RGBColor(value)
        {}

        /** \brief Copy and Move constructors. */
        GrayColor(const GrayColor&) noexcept = default;
        GrayColor(GrayColor&&) noexcept = default;

        /** \brief Destructor. */
        ~GrayColor() noexcept = default;


        //---   Assignments   -----------------------------------------------
        /** \brief Copy and Move assignments. */
        GrayColor& operator= (const GrayColor&) noexcept = default;
        GrayColor& operator= (GrayColor&&) noexcept = default;

        /** \brief Gray component value assignment. */
        template<typename T>
            requires std::is_arithmetic_v<T>
        inline GrayColor& operator= (const T value)
        {
            set(value);
            return *this;
        }

        /** \brief Assignment (RGB Color). */
        inline GrayColor& operator= (const RGBColor& rhs)
        {
            set(rhs);
            return *this;
        }


        //---   Set color   -------------------------------------------------
        /** \brief Sets value (copy). */
        inline void set(const GrayColor& copy) noexcept
        {
            r = g = b = copy.r;
        }

        /** \brief Sets value (RGBColor). */
        inline void set(const RGBColor& copy) noexcept
        {
            r = g = b = copy.get_lum();
        }

        /** \brief Sets color (three R, G, B bytes). */
        template<typename T, typename U, typename V>
            requires std::is_arithmetic_v<T>&& std::is_arithmetic_v<U>&& std::is_arithmetic_v<V>
        inline void set(const T r, const U g, const V b) noexcept
        {
            set(RGBColor(r, g, b));
        }

        /** \brief Sets color (one 3-bytes buffer). */
        template<typename T>
            requires std::is_arithmetic_v<T>
        inline void set(const unsigned char buffer[3]) noexcept
        {
            set(RGBColor(buffer[0], buffer[1], buffer[2]));
        }

        /** \brief Sets color (std::vector). */
        template<typename T>
            requires std::is_arithmetic_v<T>
        inline void set(const std::vector<T>& vect) noexcept(false)
        {
            assert(vect.size() == 3);
            set(RGBColor(vect[0], vect[1], vect[2]));
        }

        /** \brief Sets color (std::array). */
        template<typename T>
            requires std::is_arithmetic_v<T>
        inline void set(const std::array<T, 3>& arr) noexcept
        {
            set(RGBColor(arr[0], arr[1], arr[2]));
        }

    };
}
