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

#include <cassert>
#include <iostream>

#include "utils/types.h"


export module unit_tests.gui.items.test_view;

import avt.config;
import utils.coords2d;
import utils.rgb_color;
import utils.size;
import gui.items.view;


//===========================================================================
export namespace avt::unit_tests::gui::items
{
    //=======================================================================
    void test_view()
    {
        std::cout << "-- TEST avt::utils::View\n";

        avt::gui::items::View v0(nullptr, 10, 5, 1000, 640);
        /** /
        assert(v0.pos_.x == 10);
        assert(v0.pos_.y == 5);
        assert(v0.size_.width == 1000);
        assert(v0.size_.height == 640);
        /**/

        /** /
        avt::gui::items::View v1(&v0, avt::utils::Coords2D(10, 5), avt::utils::Size(1000, 640), avt::config::DEFAULT_BACKGROUND);
        assert(v1.pos_.x == 10);
        assert(v1.pos_.y == 5);
        assert(v1.size_.width == 1000);
        assert(v1.size_.height == 640);
        /**/


        std::cout << "   All tests OK\n\n";
    }
}