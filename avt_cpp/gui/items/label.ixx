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

#include <string>

#include "utils/types.h"


export module gui.items.label;


import avt.config;
import gui.items.control_base;
import utils.coords2d;
import gui.fonts.font;
import utils.rgb_color;
import utils.size;
import gui.views.view;


//===========================================================================
export namespace avt::gui::items
{
    //=======================================================================
    /** @brief The class of the views that cembed targets pictures. */
    class Label : public gui::items::ControlBase
    {
    public:
        //---   Wrappers   --------------------------------------------------
        using MyBaseClass = gui::items::ControlBase;


        //---   Constructors / Destructors   --------------------------------
        /** @brief Value Constructor (1/4).
        *
        * @param text: str
        *   The text of this label. Defaults to the  empty
        *   string,  in  which case attribute '.text' will
        *   have to be assigned before drawing this label.
        * @param x, y : int
        *   The position of this label in the  view.  This
        *   is  the  position of the baseline of the text.
        * @param font: Font
        *   A reference to the font to  be  used  for  the
        *   text associated with this label. The default
        *   size for this font is used.
        * @param font_shadow: bool
        *   The shadowing mode of the  text.  Defaults  to 
        * false.
        */
        Label(avt::gui::views::View& parent_view,
              const int              x,
              const int              y,
              const std::string&     text,
              avt::gui::fonts::Font& font,
              const bool             font_shadow = false) noexcept;

        /** @brief Value Constructor (2/4).
        *
        * @param parent_view: View
        *   A reference to the containing view.
        * @param text: str
        *   The text of this label. Defaults to the  empty
        *   string,  in  which case attribute '.text' will
        *   have to be assigned before drawing this label.
        * @param x, y : int
        *   The position of this label in the  view.  This
        *   is  the  position of the baseline of the text.
        * @param font: Font
        *   A reference to the font to  be  used  for  the
        *   text associated with this label.
        * @param font_size: int
        *   The font size of the text to display.
        * @param font_shadow: bool
        *   The shadowing mode of the  text.  Defaults  to
        * false.
        */
        Label(avt::gui::views::View& parent_view,
              const int              x,
              const int              y,
              const std::string&     text,
              avt::gui::fonts::Font& font,
              const int              font_size,
              const bool             font_shadow = false) noexcept;

        /** @brief Value Constructor (3/4).
        *
        * @param parent_view: View
        *   A reference to the containing view.
        * @param text: str
        *   The text of this label. Defaults to the  empty
        *   string,  in  which case attribute '.text' will
        *   have to be assigned before drawing this label.
        * @param pos : Coords2D
        *   The position of this label in the  view.  This
        *   is  the  position of the baseline of the text.
        * @param font: Font
        *   A reference to the font to  be  used  for  the
        *   text associated with this label.
        * @param font_shadow: bool
        *   The shadowing mode of the  text.  Defaults  to
        * false.
        */
        Label(avt::gui::views::View&      parent_view,
              const avt::utils::Coords2D& pos,
              const std::string&          text,
              avt::gui::fonts::Font&      font,
              const bool                  font_shadow = false) noexcept;

        /** @brief Value Constructor (4/4).
        *
        * @param parent_view: View
        *   A reference to the containing view.
        * @param text: str
        *   The text of this label. Defaults to the  empty
        *   string,  in  which case attribute '.text' will
        *   have to be assigned before drawing this label.
        * @param x, y : int
        *   The position of this label in the  view.  This
        *   is  the  position of the baseline of the text.
        * @param font: Font
        *   A reference to the font to  be  used  for  the
        *   text associated with this label.
        * @param size_font: int
        *   The font size of the text to display.
        * @param font_shadow: bool
        *   The shadowing mode of the  text.  Defaults  to
        * false.
        */
        Label(avt::gui::views::View&      parent_view,
              const avt::utils::Coords2D& pos,
              const std::string&          text,
              avt::gui::fonts::Font&      font,
              const int                   font_size,
              const bool                  font_shadow = false) noexcept;

        /** @brief Default Constructor. */
        Label() noexcept = default;

        /** @brief Default Copy constructor. */
        Label(const Label&) noexcept = default;

        /** @brief Default Move constructor. */
        Label(Label&&) noexcept = default;

        /** @brief Default Destructor. */
        virtual ~Label() noexcept = default;


        //---   Assignments   -----------------------------------------------
        /** @brief Default Copy assignment. */
        Label& operator= (const Label&) noexcept = default;

        /** @brief Default move assignment. */
        Label& operator= (Label&&) noexcept = default;

        /** @brief New color assignment. */
        inline Label& operator= (const avt::utils::RGBColor& new_color) noexcept
        {
            set_color(new_color);
            return *this;
        }

        /** @brief New font assignment. */
        inline Label& operator= (const avt::gui::fonts::Font& new_font) noexcept
        {
            set_font(new_font);
            return *this;
        }

        /** @brief New text assignment (char*). */
        inline Label& operator= (const char* new_text) noexcept
        {
            return operator=(std::string{ new_text });
        }

        /** @brief New text assignment (std::string). */
        inline Label& operator= (const std::string& new_text) noexcept
        {
            set_text(new_text);
            return *this;
        }


        //---   Operations   ------------------------------------------------
        /** @brief Modifies the background color of this label - no display change. */
        void set_bg_color(const avt::utils::RGBColor& bg_color) noexcept;

        /** @brief Modifies the color of this label text - no display change. */
        void set_color(const avt::utils::RGBColor& color) noexcept;

        /** @brief Modifies the font used for this label - no display change. */
        void set_font(const avt::gui::fonts::Font& new_font) noexcept;

        /** @brief Modifies the font and its size used for this label - no display change. */
        void set_font(const avt::gui::fonts::Font& new_font, const int new_size) noexcept;

        /** @brief Modifies the size of the font used for this label - no display change. */
        void set_font_size(const int new_size) noexcept;

        /** @brief Modifies the text of this label - no display change. */
        void set_text(const std::string& new_text) noexcept;


    protected:
        //---   Operations   ------------------------------------------------
        /** @brief Draws this control in the specified view - specific part for inheriting classes.
        *
        *
        * This is the protected part of the drawing method. It is automatically
        * called by method 'draw()'. It relates to the specific part of drawing
        * inheriting controls. In this baseclass, this method is abstract.
        *
        * This method MUST BE implemented in inheriting  classes  which  should
        * check  attribute  '.visible'  to  decide to draw or not this control.
        * Attributes 'b_enabled' and  'b_active'  should  be  checked  also  to
        * decide the type of drawing for the controls.
        *
        * @arg view: View
        *   This is a reference to the embedding view for this control.
        */
        virtual void _draw(avt::gui::views::View& view) noexcept override
        {
            _font.draw_text(_text, view, pos, _b_font_shadow);  // notice: pos is inherited from base class ControlBase
        }


        //---   Attributes   ------------------------------------------------
        std::string             _text;
        avt::gui::fonts::Font   _font;
        int                     _font_size;
        bool                    _b_font_shadow;

        static inline constexpr int _DEFAULT_FONT_SIZE = -1;


    private:
        //---   Operations   ------------------------------------------------
        /** @brief Evaluates width and height of text. */
        inline void m_evaluate_text_dims() noexcept
        {
            const avt::utils::Size size = _font.get_text_size(_text);
            width  = size.width;
            height = size.height;
        }
    };

}
