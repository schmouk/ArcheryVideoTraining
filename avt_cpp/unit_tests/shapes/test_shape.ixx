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


export module unit_tests.shapes.test_shape;

import types.pair;
import utils.rgba_color;
import shapes.shape;


//===========================================================================
export namespace avt::unit_tests
{
    //=======================================================================
    void test_shape()
    {
        std::cout << "-- TEST avt::utils::Shape\n";

        avt::shapes::Shape s0;
        assert(s0.x == 0);
        assert(s0.y == 0);
        assert(s0.color == avt::utils::RGBAColor(0,0,0,255));

        avt::shapes::Shape s1(1, 2, avt::utils::RGBAColor(11, 22, 33));
        assert(s1.x == 1);
        assert(s1.y == 2);
        assert(s1.color == avt::utils::RGBAColor(11, 22, 33));

        avt::shapes::Shape s2(avt::utils::Coords2D(200, 100), avt::utils::RGBAColor(44, 55, 66));
        assert(s2.x == 200);
        assert(s2.y == 100);
        assert(s2.color == avt::utils::RGBAColor(44, 55, 66));
        assert(s2.color.a == avt::Byte(255));
        assert(s2 == avt::utils::Coords2D(200, 100));

        avt::shapes::Shape s3(std::array<long, 2>{25, 15}, avt::utils::RGBAColor(1, 2, 3, 4));
        assert(s3.x == 25);
        assert(s3.y == 15);
        assert(s3.color == avt::utils::RGBAColor(1, 2, 3, 4));

        avt::shapes::Shape s4(s3);
        assert(s4.x == 25);
        assert(s4.y == 15);
        assert(s4.color == avt::utils::RGBAColor(1, 2, 3));

        s4 = s2;
        assert(s4.x == 200);
        assert(s4.y == 100);
        assert(s4.color == avt::utils::RGBAColor(44, 55, 66));
        assert(s4.color.a == avt::Byte(255));
        assert(s2.x == 200);
        assert(s2.y == 100);
        assert(s2.color == avt::utils::RGBAColor(44, 55, 66));

        s4.move(-20, 10.5f);
        assert(s4.x == 180);
        assert(s4.y == 111);
        assert(s4.color == avt::utils::RGBAColor(44, 55, 66));

        s4.move(std::vector<double>{11.4, 12.8, 13.0});
        assert(s4.x == 191);
        assert(s4.y == 124);
        assert(s4.color == avt::utils::RGBAColor(44, 55, 66));

        s3.move_at(-5, -15);
        assert(s3.x == -5);
        assert(s3.y == -15);
        assert(s3.color == avt::utils::RGBAColor(1, 2, 3));

        s2.move_at(avt::Pair(1899, 1199));
        assert(s2.x == 1899);
        assert(s2.y == 1199);
        assert(s2.color == avt::utils::RGBAColor(44, 55, 66));

        s2.color *= 1.1;
        assert(s2.x == 1899);
        assert(s2.y == 1199);
        assert(s2.color == avt::utils::RGBAColor(48, 61, 73));


        std::cout << "   All tests OK\n\n";
    }
}