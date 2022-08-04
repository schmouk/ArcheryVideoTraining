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
export module gui.fonts.italic_font;

import gui.fonts.font;


//===========================================================================
export namespace avt::gui::fonts
{
    //=======================================================================
    /** @brief The base class for bold fonts.
    *
    *   Notice: This is a minimal implementation that should help using
    *           OpenCV when putting text in video frames.
    */
    class ItalicFont : public avt::gui::fonts::Font
    {
    public:
        //---   Wrappers   --------------------------------------------------
        using MyBaseType = avt::gui::fonts::Font;


        //--- Constructors / Destructors ------------------------------------
        /** @brief Value Constructor with no background color.
        *
        * @arg size: int
        *   The size of the police. Should be greater than 5. Must be set.
        * @arg color: RGBColor
        *   A reference to the color for the text displayed  according  to
        *   the font. Defaults to white.
        * @arg b_sans_serif: bool
        *   Set this to true to get a 'sans-serif' police used.  Set it to
        *   false to get a 'serif' police.
        *   Defauts to true (i.e. 'sans-serif' police).
        */
        inline ItalicFont(const int                   size,
            const avt::utils::RGBColor& color = avt::utils::RGBColor::WHITE,
            const bool                  b_sans_serif = true) noexcept
            : MyBaseType(size, color, false, true, b_sans_serif)
        {}

        /** @brief Value Constructor with specified color and background color.
        *
        * Notice: mode 'italic' is not available with 'sans-serif'  fonts.
        *         This is an Open_CV restriction.
        *
        * @arg size: int
        *   The size of the police. Should be greater than 5. Must be set.
        * @arg color: RGBColor
        *   A reference to the color for the text displayed  according  to
        *   the font. Defaults to white.
        * @arg bg_color: RGBColor
        *   A reference to the background color over which the  text  will
        *   be displayed. If NULL, text is put with transparency, in which
        *   case an artifact is added to help the easy read of  the  text.
        *   If  set,  text  is put into a rectangle filled with this back-
        *   ground color. Defaults to NULL.
        * @arg b_sans_serif: bool
        *   Set this to true to get a 'sans-serif' police used.  Set it to
        *   false to get a 'serif' police.
        *   Defauts to true (i.e. 'sans-serif' police).
        */
        ItalicFont(const int                   size,
            const avt::utils::RGBColor& color,
            const avt::utils::RGBColor& bg_color,
            const bool                  b_sans_serif = true) noexcept
            : MyBaseType(size, color, bg_color, false, true, b_sans_serif)
        {}

        /** @brief Default Copy constructor. */
        ItalicFont(const ItalicFont&) = default;

        /** @brief Default Move constructor. */
        ItalicFont(ItalicFont&&) = default;

        /** @brief Default destructor. */
        virtual ~ItalicFont() noexcept = default;


        //---   Assignments   -----------------------------------------------
        /** @brief Default Copy assignment. */
        ItalicFont& operator= (const ItalicFont&) = default;

        /** @brief Default Move assignment. */
        ItalicFont& operator= (ItalicFont&&) = default;
    };

}
