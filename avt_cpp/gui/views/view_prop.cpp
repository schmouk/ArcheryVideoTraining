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

#include <opencv2/core/types.hpp>


module gui.views.view_prop;

import avt.config;
import utils.rgb_color;
import gui.views.view;


//===========================================================================
namespace avt::gui::views
{
    ViewProp::ViewProp(const avt::gui::views::View& parent_view,
                       const float                  x,
                       const float                  y,
                       const float                  width,
                       const float                  height,
                       const avt::utils::RGBColor&  bg_color) noexcept(false)
        : parent_view{ parent_view },
          prop_x{ x },
          prop_y{ y },
          prop_width{ width },
          prop_height{ height },
          bg_color{ bg_color }
    {
        if (x < 0.0 || x > 1.0)
            throw std::invalid_argument(std::format(m_ERR_FORMAT, "x-position", x));
        if (y < 0.0 || y > 1.0)
            throw std::invalid_argument(std::format(m_ERR_FORMAT, "y-position", y));
        if (width < 0.0 || width > 1.0)
            throw std::invalid_argument(std::format(m_ERR_FORMAT, "width", width));
        if (height < 0.0 || height > 1.0)
            throw std::invalid_argument(std::format(m_ERR_FORMAT, "height", height));
    }
}
