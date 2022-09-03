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

#include <opencv2/highgui.hpp>

#include "utils/types.h"


export module unit_tests.gui.items.test_label;

import gui.fonts.bold_font;
import gui.fonts.font;
import gui.items.label;
import gui.views.view;


//===========================================================================
export namespace avt::unit_tests::gui::items
{
    //=======================================================================
    void test_label()
    {
        std::cout << "-- TEST avt::gui::items::Label\n";

        avt::gui::views::View test_view(300, 200, avt::utils::RGBColor::ANTHRACITE);

        avt::gui::fonts::Font font_10(10);
        avt::gui::fonts::Font font_12(12);
        avt::gui::fonts::Font font_15(15);

        avt::gui::items::Label label_1(test_view, 10, 20, "Label-1", font_12, false);
        label_1.draw();

        avt::gui::items::Label label_2(test_view, 10, 35, "Label-2", font_12, 8, false);
        label_2.draw();

        avt::gui::items::Label label_3(test_view, avt::utils::Coords2D(10, 46), "Label-3", font_10, true);
        label_3.draw(false);

        label_3.move(30, 2);
        label_3.draw();

        label_3.set_color(avt::utils::RGBColor{ 239, 239, 16 });
        label_3.draw(false);

        label_3 = " Label-3 ";
        label_3.set_font(avt::gui::fonts::BoldFont{ 15 });
        label_3.draw(16, 128, false);

        label_3.set_bg_color(avt::utils::RGBColor{ 127, 16, 16 });
        label_3.draw(16, 128, false);

        label_3 = avt::utils::RGBColor{ 0,0,0 };
        label_3.draw(16, 128, false);

        label_3 = avt::gui::fonts::Font(11, avt::utils::RGBColor{ 251, 128, 0 }, false, false, false);
        label_3.draw(16, 128, false);
        cv::imshow("AVT-test-label", test_view);
        cv::waitKey(0);

        test_view.clear();
        label_3.hide();
        label_1.draw();
        label_2.draw();
        label_3.draw();
        cv::imshow("AVT-test-label", test_view);
        cv::waitKey(0);

        label_3.show();
        label_3.draw();
        cv::imshow("AVT-test-label", test_view);
        cv::waitKey(0);


        std::cout << "   All tests OK\n\n";
    }
}