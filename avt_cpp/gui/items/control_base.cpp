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

#include "utils/types.h"


module gui.views.control_base;


import utils.coords2d;
import utils.size;
import gui.views.view;


//===========================================================================
namespace avt::gui::items
{
    /** Value Constructor (x, y, width, height). */
    ControlBase::ControlBase(const avt::CoordsType x,
                             const avt::CoordsType y,
                             const avt::DimsType   width,
                             const avt::DimsType   height,
                             const bool            b_visible,
                             const bool            b_enabled,
                             const bool            b_active) noexcept
    
        : pos(x, y),
          width{ width },
          height{ height },
          b_visible{ b_visible },
          b_enabled{ b_enabled },
          b_active{ b_active },
          b_refresh{ false }
    {}


    /** Value Constructor (pos, size). */
    ControlBase::ControlBase(const avt::utils::Coords2D pos,
                             const avt::utils::Size     size,
                             const bool                 b_visible,
                             const bool                 b_enabled,
                             const bool                 b_active) noexcept
        : pos{ pos },
          width{ size.width },
          height{ size.height },
          b_visible{ b_visible },
          b_enabled{ b_enabled },
          b_active{ b_active },
          b_refresh{ false }
    {}

}
