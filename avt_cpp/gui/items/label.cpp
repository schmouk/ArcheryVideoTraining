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


module gui.items.label;


import avt.config;
import gui.items.control_base;
import utils.coords2d;
import gui.fonts.font;
import utils.rgb_color;
import gui.views.view;


//===========================================================================
namespace avt::gui::items
{
    /** Value Constructor (1/4) */
    Label::Label(avt::gui::views::View& parent_view,
                 const int              x,
                 const int              y,
                 const std::string&     text,
                 avt::gui::fonts::Font& font,
                 const bool             font_shadow) noexcept
        : MyBaseClass(parent_view, x, y),
          _text{ text },
          _font{ font },
          _font_size{ Label::_DEFAULT_FONT_SIZE },
          _b_font_shadow{ font_shadow }
    {
        m_evaluate_text_dims();
    }


    /** Value Constructor (2/4). */
    Label::Label(avt::gui::views::View& parent_view,
                 const int              x,
                 const int              y,
                 const std::string&     text,
                 avt::gui::fonts::Font& font,
                 const int              font_size,
                 const bool             font_shadow) noexcept
        : MyBaseClass(parent_view, x, y),
          _text{ text },
          _font{ font },
          _font_size{ font_size },
          _b_font_shadow{ font_shadow }
    {
        m_evaluate_text_dims();
    }


    /** Value Constructor (3/4). */
    Label::Label(avt::gui::views::View&      parent_view,
                 const avt::utils::Coords2D& pos,
                 const std::string&          text,
                 avt::gui::fonts::Font&      font,
                 const bool                  font_shadow) noexcept
        : MyBaseClass(parent_view, pos),
          _text{ text },
          _font{ font },
          _font_size{ Label::_DEFAULT_FONT_SIZE },
          _b_font_shadow{ font_shadow }
    {
        m_evaluate_text_dims();
    }


    /** Value Constructor (4/4). */
    Label::Label(avt::gui::views::View&      parent_view,
                 const avt::utils::Coords2D& pos,
                 const std::string&          text,
                 avt::gui::fonts::Font&      font,
                 const int                   font_size,
                 const bool                  font_shadow) noexcept
        : MyBaseClass(parent_view, pos),
          _text{ text },
          _font{ font },
          _font_size{ font_size },
          _b_font_shadow{ font_shadow }
    {
        m_evaluate_text_dims();
    }


    /** Modifies the background color of this label. */
    void Label::set_bg_color(const avt::utils::RGBColor& bg_color) noexcept
    {
        if (bg_color != _font.bg_color) {
            _font.bg_color = bg_color;
            b_refresh = true;
        }
    }


    /** Modifies the color of this label text. */
    void Label::set_color(const avt::utils::RGBColor& color) noexcept
    {
        if (color != _font.color) {
            _font.color = color;
            b_refresh = true;
        }
    }


    /** Modifies the text of this label. */
    inline void Label::set_text(const std::string& new_text) noexcept
    {
        _text = new_text;
        m_evaluate_text_dims();
        b_refresh = true;
    }


    /** Modifies the font used for this label - no display change. */
    void Label::set_font(const avt::gui::fonts::Font& new_font) noexcept
    {
        if (new_font != _font) {
            _font = new_font;
            m_evaluate_text_dims();
            b_refresh = true;
        }
    }


    /** Modifies the font and its size used for this label - no display change. */
    void Label::set_font(const avt::gui::fonts::Font& new_font, const int new_size) noexcept
    {
        set_font(new_font);
        set_font_size(new_size);
    }


    /** Modifies the size of the font used for this label - no display change. */
    void Label::set_font_size(const int new_size) noexcept
    {
        if (new_size != _font.size) {
            _font.set_size(new_size);
            b_refresh = true;
        }
    }

}
