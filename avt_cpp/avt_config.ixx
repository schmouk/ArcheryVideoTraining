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

#include <filesystem>
#include <opencv2/core/mat.hpp>


export module avt.config;

import gui.fonts.bold_font;
import gui.fonts.font;
import utils.rgb_color;


//===========================================================================
export namespace avt::config
{
    //=======================================================================
    constexpr long        CAMERAS_MAX_COUNT = 4; //!< AVT will not manage more than this count of input cameras
    avt::utils::RGBColor  DEFAULT_BACKGROUND = avt::utils::RGBColor::ANTHRACITE; //!< default background is very dark

    avt::gui::fonts::Font AVTConsoleFont = avt::gui::fonts::Font(13, avt::utils::RGBColor::YELLOW - 16); //!< small console font for AVT
    avt::gui::fonts::Font AVTDefaultFont = avt::gui::fonts::BoldFont(20, avt::utils::RGBColor::YELLOW);  //!< default font for every AVT text duisplay

    std::filesystem::path PICTURES_DIR{ "../picts" };
}