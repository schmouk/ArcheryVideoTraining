#pragma once
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

#if defined(_UNIT_TESTS)

//===========================================================================
#include <iostream>
#include <opencv2/highgui.hpp>
#include "opencv2/core/utils/logger.hpp"

//import unit_tests.utils.test_gray_color;
//import unit_tests.utils.test_rgb_color;
//import unit_tests.utils.test_size;
//import unit_tests.mtmp.test_threads;
//import unit_tests.mtmp.test_signal;
//import unit_tests.mtmp.test_mutex;
//import unit_tests.mtmp.test_timers;
//import unit_tests.mtmp.test_watchdog;
//import unit_tests.mtmp.test_barrier;
//import unit_tests.gui.shapes.test_shape;
//import unit_tests.gui.shapes.test_point;
//import unit_tests.gui.shapes.test_line;
//import unit_tests.gui.shapes.test_rect;
//import gui.fonts.font;
//import gui.fonts.bold_font;
//import gui.fonts.italic_font;
//import gui.fonts.bold_italic_font;
//import unit_tests.gui.views.test_view;
//import unit_tests.gui.test_avt_window;
//import unit_tests.gui.items.test_picture;
import unit_tests.devices.cameras.test_camera;


//===========================================================================
int main()
{
    cv::utils::logging::setLogLevel(cv::utils::logging::LogLevel::LOG_LEVEL_SILENT);

    std::cout << "\n===================================================";
    std::cout << "\n=====   UNIT TESTS - Archery Video Training   =====";
    std::cout << "\n===================================================\n\n";

    //avt::unit_tests::utils::test_gray_color();
    //avt::unit_tests::utils::test_rgb_color();
    //avt::unit_tests::utils::test_size();
    //avt::unit_tests::mtmp::threads::test_threads();
    //avt::unit_tests::mtmp::signal::test_signal();
    //avt::unit_tests::mtmp::mutex::test_mutex();
    //avt::unit_tests::mtmp::timer::test_timers();
    //avt::unit_tests::mtmp::watchdog::test_watchdog();
    //avt::unit_tests::mtmp::barrier::test_barrier();
    //avt::unit_tests::gui::shapes::test_shape();
    //avt::unit_tests::gui::shapes::test_point();
    //avt::unit_tests::gui::shapes::test_line();
    //avt::unit_tests::gui::shapes::test_rect();
    //avt::unit_tests::gui::views::test_view();
    //avt::unit_tests::gui::test_main_window();
    //avt::unit_tests::gui::items::test_picture();
    avt::unit_tests::devices::cameras::test_camera();


    std::cout << "\n >>>>>>>>>>   EVERYTHING WENT FINE   <<<<<<<<<<\n\n";
}

#endif
