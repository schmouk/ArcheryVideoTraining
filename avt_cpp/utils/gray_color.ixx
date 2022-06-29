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
export module utils.gray_color;

import utils.rgb_color;

//===========================================================================
export namespace avt::utils
{
    //=======================================================================
    /** @brief The class of GRAY RGB components colors.
    */
    class GrayColor : public avt::utils::RGBColor
    {
    public:
        //---   Constructors / Destructor   ---------------------------------
        /** @brief Default constructor. */
        inline GrayColor() noexcept
            : avt::utils::RGBColor()
        {}

        /** @brief "Copy" constructor (RGBColor). */
        explicit inline GrayColor(const avt::utils::RGBColor& clr) noexcept
            : avt::utils::RGBColor(clr.get_lum())
        {}

        /** @brief Valued Constructor (one gray value). */
        template<typename T>
            requires std::is_arithmetic_v<T>
        inline GrayColor(const T value) noexcept
            : avt::utils::RGBColor(value)
        {}

        /** @brief Copy and Move constructors. */
        GrayColor(const GrayColor&) noexcept = default;
        GrayColor(GrayColor&&) noexcept = default;

        /** @brief Destructor. */
        virtual ~GrayColor() noexcept = default;


        //---   Assignments   -----------------------------------------------
        /** @brief Copy and Move assignments. */
        virtual GrayColor& operator= (const GrayColor&) noexcept = default;
        virtual GrayColor& operator= (GrayColor&&) noexcept = default;

        /** @brief Gray component value assignment. */
        template<typename T>
            requires std::is_arithmetic_v<T>
        inline GrayColor& operator= (const T value)
        {
            set(value);
            return *this;
        }

        /** @brief Assignment (RGB Color). */
        inline GrayColor& operator= (const avt::utils::RGBColor& rhs) noexcept
        {
            set(rhs);
            return *this;
        }


        //---   Set color   -------------------------------------------------
        /** @brief Sets value (copy). */
        virtual inline void set(const GrayColor& copy) noexcept
        {
            rgb.R = rgb.G = rgb.B = copy.r();
        }

        /** @brief Sets value (RGBColor). */
        virtual inline void set(const avt::utils::RGBColor& copy) noexcept
        {
            rgb.R = rgb.G = rgb.B = copy.get_lum();
        }

        /** @brief Sets color (oner lum bytes). */
        template<typename T>
            requires std::is_arithmetic_v<T>
        inline void set(const T lum) noexcept
        {
            rgb.R = rgb.G = rgb.B = _clipped(lum);
        }

    };
}