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


export module unit_tests.shapes.test_rect;

import shapes.line;
import types.pair;
import shapes.rect;
import utils.rgba_color;


//===========================================================================
export namespace avt::unit_tests
{
    //=======================================================================
    void test_rect()
    {
        std::cout << "-- TEST avt::utils::Rect\n";

        avt::shapes::Rect r0;
        assert(r0.x == 0);
        assert(r0.y == 0);
        assert(r0.end.x == 0);
        assert(r0.end.y == 0);
        assert(r0.thickness == 1);
        assert(r0.color == avt::utils::RGBAColor(0, 0, 0, 255));

        avt::shapes::Rect r1({ 1,2 }, std::array<float, 2>{111.39f, 21.50f}, avt::utils::RGBAColor(11, 22, 33), 3);
        assert(r1.x == 1);
        assert(r1.y == 2);
        assert(r1.end.x == 111);
        assert(r1.end.y == 22);
        assert(r1.thickness == 3);
        assert(r1.color == avt::utils::RGBAColor(11, 22, 33, 255));

        avt::shapes::Rect r2 = r1;
        assert(r2.x == 1);
        assert(r2.y == 2);
        assert(r2.end.x == 111);
        assert(r2.end.y == 22);
        assert(r2.thickness == 3);
        assert(r2.color == avt::utils::RGBAColor(11, 22, 33, 255));

        avt::shapes::Rect r3{ r2 };
        r3.thickness = 1;
        assert(r3.x == 1);
        assert(r3.y == 2);
        assert(r3.end.x == 111);
        assert(r3.end.y == 22);
        assert(r3.thickness == 1);
        assert(r3.color == avt::utils::RGBAColor(11, 22, 33, 255));

        assert(r1 == r2);
        assert(r3 != r1);

        avt::shapes::Rect r4 = r2;

        r4.move(-20, 10.5f);
        assert(r4.x == -19);
        assert(r4.y == 13);
        assert(r4.end.x == 91);
        assert(r4.end.y == 33);
        assert(r4.thickness == 3);
        assert(r4.color == avt::utils::RGBAColor(11, 22, 33, 255));

        r4.move_at(std::vector<float>{-5.f, -15.f});
        assert(r4.x == -5);
        assert(r4.y == -15);
        assert(r4.end.x == 105);
        assert(r4.end.y == 5);
        assert(r4.thickness == 3);
        assert(r4.color == avt::utils::RGBAColor(11, 22, 33, 255));

        avt::shapes::Rect r5 = r4;
        r5 += std::vector<double>{10., 30.};
        assert(r5.x == 5);
        assert(r5.y == 15);
        assert(r5.end.x == 115);
        assert(r5.end.y == 35);
        assert(r5.thickness == 3);
        assert(r5.color == avt::utils::RGBAColor(11, 22, 33, 255));

        r4.move(std::vector<double>{11.4, 12.8, 13.0});
        assert(r4.x == 6);
        assert(r4.y == -2);
        assert(r4.end.x == 116);
        assert(r4.end.y == 18);
        assert(r4.thickness == 3);
        assert(r4.color == avt::utils::RGBAColor(11, 22, 33, 255));

        r3.move_at(-5, -15);
        assert(r3.x == -5);
        assert(r3.y == -15);
        assert(r3.end.x == 105);
        assert(r3.end.y == 5);
        assert(r3.thickness == 1);
        assert(r3.color == avt::utils::RGBAColor(11, 22, 33, 255));

        r2.color *= 1.2;
        assert(r2.x == 1);
        assert(r2.y == 2);
        assert(r2.end.x == 111);
        assert(r2.end.y == 22);
        assert(r2.thickness == 3);
        assert(r2.color == avt::utils::RGBAColor(13, 26, 40, 255));

        r2 += {1.1, 1.9};
        assert(r2.x == 2);
        assert(r2.y == 4);
        assert(r2.end.x == 112);
        assert(r2.end.y == 24);
        assert(r2.thickness == 3);
        assert(r2.color == avt::utils::RGBAColor(13, 26, 40, 255));

        r2 -= std::array<long long, 2>{100, 50};
        assert(r2.x == -98);
        assert(r2.y == -46);
        assert(r2.end.x == 12);
        assert(r2.end.y == -26);
        assert(r2.thickness == 3);
        assert(r2.color == avt::utils::RGBAColor(13, 26, 40, 255));

        r2 /= 1.5;
        assert(r2.x == -65);
        assert(r2.y == -31);
        assert(r2.end.x == 8);
        assert(r2.end.y == -17);
        assert(r2.thickness == 3);
        assert(r2.color == avt::utils::RGBAColor(13, 26, 40, 255));

        r2 = avt::shapes::Rect({ 11, 22 }, { 333.3, 443.7 }, avt::utils::RGBAColor(127, 128, 129, 130), 15);
        assert(r2.x == 11);
        assert(r2.y == 22);
        assert(r2.end.x == 333);
        assert(r2.end.y == 444);
        assert(r2.thickness == 15);
        assert(r2.color == avt::utils::RGBAColor(127, 128, 129, 130));

        r2.top_left() = std::vector<long>{-1, -22, -331};
        assert(r2.x == -1);
        assert(r2.y == -22);
        assert(r2.end.x == 333);
        assert(r2.end.y == 444);
        assert(r2.thickness == 15);
        assert(r2.color == avt::utils::RGBAColor(127, 128, 129, 130));

        r2.bottom_right() = std::array<float, 2>{ -11., -221.51f };
        assert(r2.x == -1);
        assert(r2.y == -22);
        assert(r2.end.x == -11);
        assert(r2.end.y == -222);
        assert(r2.thickness == 15);
        assert(r2.color == avt::utils::RGBAColor(127, 128, 129, 130));

        std::cout << "   All tests OK\n\n";
    }
}