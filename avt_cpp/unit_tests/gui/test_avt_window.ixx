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
#include <Windows.h>
#include <winuser.h>


#include <opencv2/highgui.hpp>
#include <opencv2/core/types.hpp>

#include "utils/types.h"
#include "gui/items/view.h"


export module unit_tests.gui.test_avt_window;


import gui.avt_window;
import avt.config;


//===========================================================================
export namespace avt::unit_tests::gui
{
    //-----------------------------------------------------------------------
    void my_mouse_event(int event, int x, int y, int flags, void* userdata)
    {
        SetCursor(LoadCursor(0, IDC_ARROW));
    }

    //=======================================================================
    void test_main_window()
    {
        std::cout << "-- TEST avt::gui::AVTWindow\n";

        avt::gui::AVTWindow main_window("main-window", "Archery Video Training - v0.0", avt::config::DEFAULT_BACKGROUND);
        cv::setMouseCallback("main-window", my_mouse_event);

        main_window.draw(0);


        std::cout << "   All tests OK\n\n";
    }
}