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

#include <array>
#include <cassert>
#include <iostream>
#include <vector>

#include "utils/types.h"


export module unit_tests.gui.shapes.test_line;

import gui.shapes.line;
import types.pair;
import utils.rgba_color;


//===========================================================================
export namespace avt::unit_tests::gui::shapes
{
    //=======================================================================
    void test_line()
    {
        std::cout << "-- TEST avt::gui::shapes::Line\n";

        avt::gui::shapes::Line l0;
        assert(l0.x == 0);
        assert(l0.y == 0);
        assert(l0.end.x == 0);
        assert(l0.end.y == 0);
        assert(l0.thickness == 1);
        assert(l0.color == avt::utils::RGBAColor(0, 0, 0, 255));

        avt::gui::shapes::Line l1({ 1,2 }, std::array<float, 2>{111.39f, 21.50f}, avt::utils::RGBAColor(11, 22, 33), 3);
        assert(l1.x == 1);
        assert(l1.y == 2);
        assert(l1.end.x == 111);
        assert(l1.end.y == 22);
        assert(l1.thickness == 3);
        assert(l1.color == avt::utils::RGBAColor(11, 22, 33, 255));

        avt::gui::shapes::Line l2 = l1;
        assert(l2.x == 1);
        assert(l2.y == 2);
        assert(l2.end.x == 111);
        assert(l2.end.y == 22);
        assert(l2.thickness == 3);
        assert(l2.color == avt::utils::RGBAColor(11, 22, 33, 255));

        avt::gui::shapes::Line l3{ l2 };
        l3.thickness = 1;
        assert(l3.x == 1);
        assert(l3.y == 2);
        assert(l3.end.x == 111);
        assert(l3.end.y == 22);
        assert(l3.thickness == 1);
        assert(l3.color == avt::utils::RGBAColor(11, 22, 33, 255));

        assert(l1 == l2);
        assert(l3 != l1);

        avt::gui::shapes::Line l4 = l2;

        l4.move(-20, 10.5f);
        assert(l4.x == -19);
        assert(l4.y == 13);
        assert(l4.end.x == 91);
        assert(l4.end.y == 33);
        assert(l4.thickness == 3);
        assert(l4.color == avt::utils::RGBAColor(11, 22, 33, 255));

        l4.move_at(std::vector<float>{-5.f, -15.f});
        assert(l4.x == -5);
        assert(l4.y == -15);
        assert(l4.end.x == 105);
        assert(l4.end.y == 5);
        assert(l4.thickness == 3);
        assert(l4.color == avt::utils::RGBAColor(11, 22, 33, 255));

        avt::gui::shapes::Line l5 = l4;
        l5 += std::vector<double>{10., 30.};
        assert(l5.x == 5);
        assert(l5.y == 15);
        assert(l5.end.x == 115);
        assert(l5.end.y == 35);
        assert(l5.thickness == 3);
        assert(l5.color == avt::utils::RGBAColor(11, 22, 33, 255));

        l4.move(std::vector<double>{11.4, 12.8, 13.0});
        assert(l4.x == 6);
        assert(l4.y == -2);
        assert(l4.end.x == 116);
        assert(l4.end.y == 18);
        assert(l4.thickness == 3);
        assert(l4.color == avt::utils::RGBAColor(11, 22, 33, 255));

        l3.move_at(-5, -15);
        assert(l3.x == -5);
        assert(l3.y == -15);
        assert(l3.end.x == 105);
        assert(l3.end.y == 5);
        assert(l3.thickness == 1);
        assert(l3.color == avt::utils::RGBAColor(11, 22, 33, 255));

        l2.color *= 1.2;
        assert(l2.x == 1);
        assert(l2.y == 2);
        assert(l2.end.x == 111);
        assert(l2.end.y == 22);
        assert(l2.thickness == 3);
        assert(l2.color == avt::utils::RGBAColor(13, 26, 40, 255));

        l2 += {1.1, 1.9};
        assert(l2.x == 2);
        assert(l2.y == 4);
        assert(l2.end.x == 112);
        assert(l2.end.y == 24);
        assert(l2.thickness == 3);
        assert(l2.color == avt::utils::RGBAColor(13, 26, 40, 255));

        l2 -= std::array<long long, 2>{100, 50};
        assert(l2.x == -98);
        assert(l2.y == -46);
        assert(l2.end.x == 12);
        assert(l2.end.y == -26);
        assert(l2.thickness == 3);
        assert(l2.color == avt::utils::RGBAColor(13, 26, 40, 255));

        l2 /= 1.5;
        assert(l2.x == -65);
        assert(l2.y == -31);
        assert(l2.end.x == 8);
        assert(l2.end.y == -17);
        assert(l2.thickness == 3);
        assert(l2.color == avt::utils::RGBAColor(13, 26, 40, 255));


        std::cout << "   All tests OK\n\n";
    }
}