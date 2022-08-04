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


export module gui.fonts;

import utils.coords2d;
import video.frame;
import utils.rgb_color;
import utils.size;


//===========================================================================
export namespace avt::gui::fonts
{
    //=======================================================================
    /** @brief The base class for all fonts.
    *
    *   Notice: This is a minimal implementation that should help using 
    *           OpenCV when putting text in video frames.
    */
    class Font
    {
    public:
        //--- Constructors / Destructors ------------------------------------
        /** @brief Value Constructor with no background color.
        *
        * Notice: mode 'italic' is not available with 'sans-serif'  fonts.
        *         This is an Open_CV restriction.
        *
        * @arg size: int
        *   The size of the police. Should be greater than 5. Must be set.
        * @arg color: RGBColor
        *   A reference to the color for the text displayed  according  to 
        *   the font. Defaults to white.
        * @arg b_bold: bool
        *   Set this to true to get an italic font.  Prefer to instantiate 
        *   class BoldFont for such a use.  Defaults to false (i.e. normal 
        *   font).
        * @arg b_italic: bool
        *   Set this to true to get an italic font.  Prefer to instantiate 
        *   class ItalicFont for such a use.
        *   Defaults to false (i.e. normal font).
        * @arg b_sans_serif: bool
        *   Set this to true to get a 'sans-serif' police used.  Set it to 
        *   false to get a 'serif' police.
        *   Defauts to true (i.e. 'sans-serif' police).
        */
        Font(const int                   size_,
             const avt::utils::RGBColor& color_        = avt::utils::RGBColor::WHITE,
             const bool                  b_bold_       = false,
             const bool                  b_italic_     = false,
             const bool                  b_sans_serif_ = true) noexcept
            : color{ color_},
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
        * @arg b_bold: bool
        *   Set this to true to get an italic font.  Prefer to instantiate
        *   class BoldFont for such a use.  Defaults to false (i.e. normal
        *   font).
        * @arg b_italic: bool
        *   Set this to true to get an italic font.  Prefer to instantiate
        *   class ItalicFont for such a use.
        *   Defaults to false (i.e. normal font).
        * @arg b_sans_serif: bool
        *   Set this to true to get a 'sans-serif' police used.  Set it to
        *   false to get a 'serif' police.
        *   Defauts to true (i.e. 'sans-serif' police).
        */
        Font(const int                  size_,
            const avt::utils::RGBColor& color_,
            const avt::utils::RGBColor& bg_color_,
            const bool                  b_bold_ = false,
            const bool                  b_italic_ = false,
            const bool                  b_sans_serif_ = true) noexcept
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

        /** @brief Default Copy constructor. */
        Font(const Font&) = default;

        /** @brief Default Move constructor. */
        Font(Font&&) = default;

        /** @brief Default destructor. */
        virtual ~Font() noexcept = default;


        //---   Assignments   -----------------------------------------------
        /** @brief Default Copy assignment. */
        Font& operator= (const Font&) = default;

        /** @brief Default Move assignment. */
        Font& operator= (Font&&) = default;


        //---   Drawing text   ----------------------------------------------
        /** @brief Draws specified text with this font.
        *
        * Notice: next  version could propose transparency for background and 
        *         blurring background also if no background rectangle is set.
        *
        * @arg text: std::string
        *   The text to be drawn.
        * @arg frame: avt::video::Frame
        *   A reference to the frame within which the text is drawn.
        * @arg pos: Coords2D
        *   The absolute (x, y) position of the text in the specified view.
        * @arg b_shadow: bool
        *   Set  this  to  true to get a shadowing of the text,  or set it to
        *   false otherwise. Defaults to true.
        */
        void draw_text(const std::string&          text,
                       avt::video::Frame&          frame,
                       const avt::utils::Coords2D& pos,
                       const bool                  b_shadow)
        {
            if (b_force_bgcolor) {
                // let's put text over a background solid color
                cv::Size text_size = get_text_size(text);
                cv::Point final_pos = pos;
                final_pos.y -= thickness;
                cv::rectangle(frame, cv::Rect(final_pos, text_size), cv::Scalar(bg_color), cv::FILLED);
            }
            else {
                // let's draw shadow artifact, if asked for
                if (b_shadow) {
                    avt::utils::RGBColor shadow_color{ color * 0.6f };
                    cv::putText(frame, 
                                text,
                                pos + avt::utils::Coords2D{1, 1},
                                cv_font,
                                font_scale,
                                (cv::Scalar)shadow_color,
                                1,
                                cv::LINE_AA);
                }
            }

            // then, let's draw the final text
            cv::putText(frame,
                        text,
                        pos,
                        cv_font,
                        font_scale,
                        (cv::Scalar)color,
                        thickness,
                        cv::LINE_AA);
        }


        //---   Operations on colors   --------------------------------------
        /** @brief Suppresses the background color (i.e. no background filling under displayed text. */
        inline void clear_bg_color() noexcept
        {
            bg_color = avt::utils::RGBColor::NULL_COLOR;
        }

        /** @brief Modifies the background color for this font. */
        inline void set_bg_color(const avt::utils::RGBColor& new_bg_color) noexcept
        {
            bg_color = new_bg_color;
        }

        /** @brief Modifies the text color for this font. */
        inline void set_color(const avt::utils::RGBColor& new_color) noexcept
        {
            color = new_color;
        }

        /** @brief Sets the size for this font (forced to be greater than 5). */
        inline void set_size(const int size_) noexcept
        {
            size = std::min(6, size_);
            font_scale = cv::getFontScaleFromHeight(cv_font, size, thickness);
        }


        //---   Operations on Text sizes   ----------------------------------
        /** @brief Returns the baseline (in pixels) associated with the specified text when drawn with this font. */
        inline int get_text_baseline(const std::string& text) noexcept
        {
            int baseline;
            get_text_size(text, &baseline);
            return baseline + thickness;
        }

        /** @brief Returns the height (in pixels) of specified text when drawn with this font. */
        inline int get_text_height(const std::string& text) noexcept
        {
            return get_text_size(text).height;
        }

        /** @brief Returns the width and height (in pixels) associated with the specified text when drawn with this font. */
        inline avt::utils::Size get_text_size(const std::string& text, int* baseline = nullptr) noexcept
        {
            int the_baseline;
            cv::Size the_size = cv::getTextSize(text, cv_font, font_scale, thickness, &the_baseline);
            if (baseline != nullptr)
                *baseline = the_baseline + thickness;
            return avt::utils::Size(the_size);
        }

        /** @brief Returns the width (in pixels) of specified text when drawn with this font. */
        inline int get_text_width(const std::string& text) noexcept
        {
            return get_text_size(text).width;
        }


    protected:
        //---   Attributes   ------------------------------------------------
        avt::utils::RGBColor color;
        avt::utils::RGBColor bg_color;
        double               font_scale;
        int                  size;
        int                  thickness;
        int                  cv_font;
        bool                 b_bold;
        bool                 b_italic;
        bool                 b_sans_serif;
        bool                 b_force_bgcolor;
    };

}