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

export module unit_tests.utils.test_offsets;

import utils.offsets;



//===========================================================================
export namespace avt::unit_tests
{
    //=======================================================================
    void test_offsets()
    {
        std::cout << "-- TEST avt::utils::Offsets\n";

        avt::utils::Offsets o0;
        assert(o0.dx == 0);
        assert(o0.dy == 0);

        avt::utils::Offsets o1(-65'000L, 3.9);
        assert(o1.dx == -32'768);
        assert(o1.dy == 4);

        avt::utils::Offsets o2(o1);
        assert(o2.dx == -32'768);
        assert(o2.dy == 4);

        o2 = o0;
        assert(o2.dx == 0);
        assert(o2.dy == 0);

        o2 = { (unsigned char)1, 3ULL };
        assert(o2.dx == 1);
        assert(o2.dy == 3);

        o2 = std::vector<double>{ 2.3, 23.5, 0.9 };
        assert(o2.dx == 2);
        assert(o2.dy == 24);

        o2 = std::array<float, 2>{ 1.1f, 1'023.6f };
        assert(o2.dx == 1);
        assert(o2.dy == 1'024);

        avt::utils::Offsets o3 = o1 + o2;
        assert(o3.dx == -32'767);
        assert(o3.dy == 1028);

        o2 += o1;
        assert(o2.dx == -32'767);
        assert(o2.dy == 1'028);

        o2 += std::vector<long>{ 32'767, 40'000, 16, 25 };
        assert(o2.dx == 0);
        assert(o2.dy == 32'767);

        o2 += std::array<float, 2>{ 1.f, -1.9f };
        assert(o2.dx == 1);
        assert(o2.dy == 32'765);

        o3 = o2 + std::vector<float>{ 1.f, -1.9f };
        assert(o3.dx == 2);
        assert(o3.dy == 32'763);
        assert(o2.dx == 1);
        assert(o2.dy == 32'765);

        avt::utils::Offsets o4 = std::vector<float>{ 1.f, -1.9f, 4.5f } + o2;
        assert(o4.dx == 2);
        assert(o4.dy == 32'763);
        assert(o2.dx == 1);
        assert(o2.dy == 32'765);

        avt::utils::Offsets o5 = o2 + std::array<char, 2>{ 1, -2 };
        assert(o5.dx == 2);
        assert(o5.dy == 32'763);
        assert(o2.dx == 1);
        assert(o2.dy == 32'765);

        avt::utils::Offsets o6 = std::array<char, 2>{ 1, -2 } + o2;
        assert(o6.dx == 2);
        assert(o6.dy == 32'763);
        assert(o2.dx == 1);
        assert(o2.dy == 32'765);

        assert(o3 == o4);
        assert(o4 == o3);
        assert(!(o3 != o4));
        assert(!(o4 != o3));
        assert(o2 != o3);
        assert(!(o2 == o3));

        o1 = { 4L, 37.6f };
        assert(o1.dx == 4);
        assert(o1.dy == 38);

        o2 = { 3, 1'023 };
        assert(o2.dx == 3);
        assert(o2.dy == 1'023);

        o2 -= o1;
        assert(o2.dx == -1);
        assert(o2.dy == 985);

        o2 -= std::vector<long>{ -32'767, -40'000, 16, 25 };
        assert(o2.dx == 32'766);
        assert(o2.dy == 32'767);

        o2 -= std::array<float, 2>{ 1.f, 2.9f };
        assert(o2.dx == 32'765);
        assert(o2.dy == 32'764);

        o3 = o2 - o1;
        assert(o3.dx == 32'761);
        assert(o3.dy == 32'726);
        assert(o2.dx == 32'765);
        assert(o2.dy == 32'764);

        o3 = o2 - std::vector<float>{ 1.f, 1.9f };
        assert(o3.dx == 32'764);
        assert(o3.dy == 32'762);
        assert(o2.dx == 32'765);
        assert(o2.dy == 32'764);

        o4 = std::vector<float>{ 1.f, -2.9f, 4.5f } - o2;
        assert(o4.dx == -32'764);
        assert(o4.dy == -32'767);
        assert(o2.dx == 32'765);
        assert(o2.dy == 32'764);

        o3 = o2 - std::array<char, 2>{ 1, 2 };
        assert(o3.dx == 32'764);
        assert(o3.dy == 32'762);
        assert(o2.dx == 32'765);
        assert(o2.dy == 32'764);

        o4 = std::array<float, 2>{ 1.f, -2.9f } - o2;
        assert(o4.dx == -32'764);
        assert(o4.dy == -32'767);
        assert(o2.dx == 32'765);
        assert(o2.dy == 32'764);

        o2 = avt::utils::Offsets{ 3, 1'023 };
        o2 *= 2.3f;
        assert(o2.dx == 7);
        assert(o2.dy == 2'353);

        avt::utils::Offsets o7 = o2 * 1.1;
        assert(o7.dx == 8);
        assert(o7.dy == 2'588);

        o7 = 2.1 * o2;
        assert(o7.dx == 15);
        assert(o7.dy == 4'941);

        avt::utils::Offsets o8 = o2 / 1.1f;
        assert(o8.dx == 6);
        assert(o8.dy == 2'139);

        o8 /= 0.9;
        assert(o8.dx == 7);
        assert(o8.dy == 2'377);


        std::cout << "   All tests OK\n\n";
    }
}