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

#include <opencv2/core/types.hpp>

#include "utils/types.h"
#include "gui/items/view.h"


export module unit_tests.gui.items.test_view;

import avt.config;
import utils.coords2d;
import video.frame;
import utils.rgb_color;
import utils.size;


//===========================================================================
export namespace avt::unit_tests::gui::items
{
    //=======================================================================
    void test_view()
    {
        std::cout << "-- TEST avt::gui::items::View\n";

        avt::gui::items::View v0(nullptr, 10, 5, 1000, 640);
        assert(v0.pos.x == 10);
        assert(v0.pos.y == 5);
        assert(v0.size().width == 1000);
        assert(v0.size().height == 640);
        assert(v0(1, 1) == avt::config::DEFAULT_BACKGROUND);

        avt::gui::items::View v1(&v0, avt::utils::Coords2D(11, 6), avt::utils::Size(1000, 640), avt::config::DEFAULT_BACKGROUND);
        assert(v1.pos.x == 11);
        assert(v1.pos.y == 6);
        assert(v1.size().width == 1000);
        assert(v1.size().height == 640);
        assert(v1(639, 999) == avt::config::DEFAULT_BACKGROUND);

        avt::gui::items::View v2(&v1, cv::Rect(21, 11, 201, 102), avt::utils::RGBColor::YELLOW);
        assert(v2.pos.x == 21);
        assert(v2.pos.y == 11);
        assert(v2.size().width == 201);
        assert(v2.size().height == 102);
        assert(v2(0, 0) == cv::Vec3b(0, 255, 255));

        avt::gui::items::View v3{ v2 };
        assert(v3.pos.x == 21);
        assert(v3.pos.y == 11);
        assert(v3.size().width == 201);
        assert(v3.size().height == 102);
        assert(v3(101, 200) == cv::Vec3b(0, 255, 255));

        avt::gui::items::View v4 = v1;
        assert(v4.pos.x == 11);
        assert(v4.pos.y == 6);
        assert(v4.size().width == 1000);
        assert(v4.size().height == 640);
        assert(v4(639, 999)[0] == 31);
        assert(v4(639, 999)[1] == 31);
        assert(v4(639, 999)[2] == 31);

        avt::utils::Coords2D abs_pos = v0.get_absolute_pos();
        assert(abs_pos.x == 10);
        assert(abs_pos.y == 5);

        abs_pos = v1.get_absolute_pos();
        assert(abs_pos.x == 21);
        assert(abs_pos.y == 11);

        abs_pos = v3.get_absolute_pos();
        assert(abs_pos.x == 42);
        assert(abs_pos.y == 22);


        avt::video::Frame frame = cv::Mat3b::zeros(1200, 1920);

        v3.draw(frame);
        assert(frame(21, 41) == cv::Vec3b(0, 0, 0));
        assert(frame(22, 41) == cv::Vec3b(0, 0, 0));
        assert(frame(22, 42) == avt::utils::RGBColor::YELLOW);
        assert(frame(22, 242) == avt::utils::RGBColor::YELLOW);
        assert(frame(22, 243) == cv::Vec3b(0, 0, 0));
        assert(frame(123, 41) == cv::Vec3b(0, 0, 0));
        assert(frame(123, 42) == avt::utils::RGBColor::YELLOW);
        assert(frame(123, 242) == avt::utils::RGBColor::YELLOW);
        assert(frame(123, 243) == cv::Vec3b(0, 0, 0));
        assert(frame(124, 42) == cv::Vec3b(0, 0, 0));

        v3.move(-5, 10);
        assert(v3.pos.x == 16);
        assert(v3.pos.y == 21);
        assert(v3.size().width == 201);
        assert(v3.size().height == 102);
        assert(v3(101, 200) == cv::Vec3b(0, 255, 255));
        abs_pos = v3.get_absolute_pos();
        assert(abs_pos.x == 37);
        assert(abs_pos.y == 32);

        v3.move(std::array<float, 2>{4.3f, -1.9f});
        assert(v3.pos.x == 20);
        assert(v3.pos.y == 19);
        assert(v3.size().width == 201);
        assert(v3.size().height == 102);
        assert(v3(101, 200) == cv::Vec3b(0, 255, 255));
        abs_pos = v3.get_absolute_pos();
        assert(abs_pos.x == 41);
        assert(abs_pos.y == 30);

        v4.move_at(2, 1);
        assert(v4.pos.x == 2);
        assert(v4.pos.y == 1);
        assert(v4.size().width == 1000);
        assert(v4.size().height == 640);
        assert(v4(639, 999)[0] == 31);
        assert(v4(639, 999)[1] == 31);
        assert(v4(639, 999)[2] == 31);
        abs_pos = v4.get_absolute_pos();
        assert(abs_pos.x == 12);
        assert(abs_pos.y == 6);

        v0.move_at(1, 1);
        assert(v0.pos.x == 1);
        assert(v0.pos.y == 1);
        assert(v0.size().width == 1000);
        assert(v0.size().height == 640);
        assert(v0(1, 1) == avt::config::DEFAULT_BACKGROUND);
        assert(v4.pos.x == 2);
        assert(v4.pos.y == 1);
        assert(v4.size().width == 1000);
        assert(v4.size().height == 640);
        assert(v4(639, 999)[0] == 31);
        assert(v4(639, 999)[1] == 31);
        assert(v4(639, 999)[2] == 31);
        abs_pos = v4.get_absolute_pos();
        assert(abs_pos.x == 3);
        assert(abs_pos.y == 2);

        v0.move_at(avt::utils::Coords2D{ 100, 60 });
        assert(v0.pos.x == 100);
        assert(v0.pos.y == 60);
        assert(v0.size().width == 1000);
        assert(v0.size().height == 640);
        assert(v0(1, 1) == avt::config::DEFAULT_BACKGROUND);
        assert(v4.pos.x == 2);
        assert(v4.pos.y == 1);
        assert(v4.size().width == 1000);
        assert(v4.size().height == 640);
        assert(v4(639, 999)[0] == 31);
        assert(v4(639, 999)[1] == 31);
        assert(v4(639, 999)[2] == 31);
        abs_pos = v4.get_absolute_pos();
        assert(abs_pos.x == 102);
        assert(abs_pos.y == 61);

        v4.move_at(std::vector<double>{19.9, 21.4, 30.});
        assert(v0.pos.x == 100);
        assert(v0.pos.y == 60);
        assert(v0.size().width == 1000);
        assert(v0.size().height == 640);
        assert(v0(1, 1) == avt::config::DEFAULT_BACKGROUND);
        assert(v4.pos.x == 20);
        assert(v4.pos.y == 21);
        assert(v4.size().width == 1000);
        assert(v4.size().height == 640);
        assert(v4(639, 999)[0] == 31);
        assert(v4(639, 999)[1] == 31);
        assert(v4(639, 999)[2] == 31);
        abs_pos = v4.get_absolute_pos();
        assert(abs_pos.x == 120);
        assert(abs_pos.y == 81);

        v4.resize(11, 22);
        assert(v4.pos.x == 20);
        assert(v4.pos.y == 21);
        assert(v4.size().width == 11);
        assert(v4.size().height == 22);
        assert(v4(21, 10)[0] == 31);
        assert(v4[21][10][1] == 31);
        assert(v4(21, 10)[2] == 31);
        abs_pos = v4.get_absolute_pos();
        assert(abs_pos.x == 120);
        assert(abs_pos.y == 81);
        assert(v1.pos.x == 11);
        assert(v1.pos.y == 6);
        assert(v1.size().width == 1000);
        assert(v1.size().height == 640);
        assert(v1(639, 999) == avt::config::DEFAULT_BACKGROUND);

        v4.resize(3.3f);
        assert(v4.pos.x == 20);
        assert(v4.pos.y == 21);
        assert(v4.size().width == 36);
        assert(v4.size().height == 73);
        assert(v4(72, 35)[0] == 31);
        assert(v4[72][35][1] == 31);
        assert(v4(72, 35)[2] == 31);
        abs_pos = v4.get_absolute_pos();
        assert(abs_pos.x == 120);
        assert(abs_pos.y == 81);

        assert(v4.area() == 36 * 73);
        assert(v4.size() == cv::Size(36, 73));
        assert(cv::Size(37, 73) != v4.size());

        v4.scale(0.85);
        assert(v4.pos.x == 17);
        assert(v4.pos.y == 18);
        assert(v4.size().width == 31);
        assert(v4.size().height == 62);
        assert(v4(61, 30)[0] == 31);
        assert(v4[61][30][1] == 31);
        assert(v4(61, 30)[2] == 31);
        abs_pos = v4.get_absolute_pos();
        assert(abs_pos.x == 117);
        assert(abs_pos.y == 78);



        std::cout << "   All tests OK\n\n";
    }
}