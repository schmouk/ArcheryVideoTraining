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
#include <cstring>

#include <opencv2/imgproc.hpp>
#include <opencv2/core/types.hpp>

#include "utils/types.h"


module gui.fonts.font;

import utils.coords2d;
import utils.rgb_color;
import utils.size;


//===========================================================================
namespace avt::gui::fonts
{
    /** Value Constructor with no background color. */
    Font::Font(const int       size_,
               const RGBColor& color_,
               const bool      b_bold_,
               const bool      b_italic_,
               const bool      b_sans_serif_) noexcept
        : color{ color_ },
          font_scale{},
          bg_color{},
          size{},
          thickness{ b_bold_ ? 2 : 1 },
          cv_font{ b_sans_serif_ ? cv::FONT_HERSHEY_SIMPLEX : cv::FONT_HERSHEY_COMPLEX },
          b_bold{ b_bold_ },
          b_italic{ b_italic_ },
          b_sans_serif{ b_sans_serif_ },
          b_force_bgcolor{ false }
    {
        if (b_italic_)
            cv_font |= cv::FONT_ITALIC;
        set_color(color_);
        set_size(size);
    }

    /** Value Constructor with specified color and background color. */
    Font::Font(const int       size_,
               const RGBColor& color_,
               const RGBColor& bg_color_,
               const bool      b_bold_,
               const bool      b_italic_,
               const bool      b_sans_serif_) noexcept
        : color{ color_ },
          bg_color{ bg_color_ },
          font_scale{},
          size{},
          thickness{ b_bold_ ? 2 : 1 },
          cv_font{ b_sans_serif_ ? cv::FONT_HERSHEY_SIMPLEX : cv::FONT_HERSHEY_COMPLEX },
          b_bold{ b_bold_ },
          b_italic{ b_italic_ },
          b_sans_serif{ b_sans_serif_ },
          b_force_bgcolor{ true }
    {
        if (b_italic_)
            cv_font |= cv::FONT_ITALIC;
        set_color(color_);
        set_size(size);
    }

    /** Draws specified text with this font. */
    void Font::draw_text(const std::string&          text,
                         avt::ImageType&             image,
                         const avt::utils::Coords2D& pos,
                         const bool                  b_shadow) noexcept
    {
        if (b_force_bgcolor) {
            // let's put text over a background solid color
            cv::Size text_size = get_text_size(text);
            cv::Point final_pos = pos;
            final_pos.y -= thickness;
            cv::rectangle(image, cv::Rect(final_pos, text_size), cv::Scalar(bg_color), cv::FILLED);
        }
        else {
            // let's draw shadow artifact, if asked for
            if (b_shadow) {
                RGBColor shadow_color{ color * 0.6f };
                cv::putText(image,
                    text,
                    pos + avt::utils::Coords2D{ 1, 1 },
                    cv_font,
                    font_scale,
                    (cv::Scalar)shadow_color,
                    1,
                    cv::LINE_AA);
            }
        }

        // then, let's draw the final text
        cv::putText(image,
            text,
            pos,
            cv_font,
            font_scale,
            (cv::Scalar)color,
            thickness,
            cv::LINE_AA);
    }

    /** Returns the width and height (in pixels) associated with the specified text when drawn with this font. */
    avt::utils::Size Font::get_text_size(const std::string& text, int* baseline) noexcept
    {
        int the_baseline;
        cv::Size the_size = cv::getTextSize(text, cv_font, font_scale, thickness, &the_baseline);
        if (baseline != nullptr)
            *baseline = the_baseline + thickness;
        return avt::utils::Size(the_size);
    }

}