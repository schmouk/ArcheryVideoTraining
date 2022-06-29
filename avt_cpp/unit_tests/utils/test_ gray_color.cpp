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
#include <array>
#include <cassert>
#include <vector>

import utils.gray_color;


//===========================================================================
namespace avt::tests
{
    //=======================================================================
    void test_gray_color()
    {
        avt::utils::GrayColor r0;
        assert(r0.r() == 0);
        assert(r0.g() == 0);
        assert(r0.b() == 0);
        assert(r0.data[0] == 0);
        assert(r0.data[1] == 0);
        assert(r0.data[2] == 0);

        avt::utils::GrayColor r1(127);
        assert(r1.r() == 127);
        assert(r1.g() == 127);
        assert(r1.b() == 127);
        assert(r1.data[0] == 127);
        assert(r1.data[1] == 127);
        assert(r1.data[2] == 127);

        avt::utils::GrayColor r2 = 253;
        assert(r2.r() == 253);
        assert(r2.g() == 253);
        assert(r2.b() == 253);
        assert(r2.data[0] == 253);
        assert(r2.data[1] == 253);
        assert(r2.data[2] == 253);

        avt::utils::GrayColor r3(avt::utils::RGBColor(0, 127, 255));
        assert(r3.r() == 104);
        assert(r3.g() == 104);
        assert(r3.b() == 104);
        assert(r3.data[0] == 104);
        assert(r3.data[1] == 104);
        assert(r3.data[2] == 104);

        r0.set(15);
        assert(r0.r() == 15);
        assert(r0.g() == 15);
        assert(r0.b() == 15);
        assert(r0.data[0] == 15);
        assert(r0.data[1] == 15);
        assert(r0.data[2] == 15);

        r0.set(avt::utils::RGBColor(0, 127, 255));
        assert(r0.r() == 104);
        assert(r0.g() == 104);
        assert(r0.b() == 104);
        assert(r0.data[0] == 104);
        assert(r0.data[1] == 104);
        assert(r0.data[2] == 104);
    };
}