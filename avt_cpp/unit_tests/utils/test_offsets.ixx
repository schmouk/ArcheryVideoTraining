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

        avt::utils::Offsets c0;
        assert(c0.dx == 0);
        assert(c0.dy == 0);

        avt::utils::Offsets c1(-65'000L, 3.9);
        assert(c1.dx == -32'768);
        assert(c1.dy == 4);

        avt::utils::Offsets c2(c1);
        assert(c2.dx == -32'768);
        assert(c2.dy == 4);

        c2 = c0;
        assert(c2.dx == 0);
        assert(c2.dy == 0);

        c2 = { (unsigned char)1, 3ULL };
        assert(c2.dx == 1);
        assert(c2.dy == 3);

        c2 = std::vector<double>{ 2.3, 23.5, 0.9 };
        assert(c2.dx == 2);
        assert(c2.dy == 24);

        c2 = std::array<float, 2>{ 1.1f, 1'023.6f };
        assert(c2.dx == 1);
        assert(c2.dy == 1'024);

        avt::utils::Offsets c3 = c1 + c2;
        assert(c3.dx == -32'767);
        assert(c3.dy == 1028);

        c2 += c1;
        assert(c2.dx == -32'767);
        assert(c2.dy == 1'028);

        c2 += std::vector<long>{ 32'767, 40'000, 16, 25 };
        assert(c2.dx == 0);
        assert(c2.dy == 32'767);

        c2 += std::array<float, 2>{ 1.f, -1.9f };
        assert(c2.dx == 1);
        assert(c2.dy == 32'765);

        c3 = c2 + std::vector<float>{ 1.f, -1.9f };
        assert(c3.dx == 2);
        assert(c3.dy == 32'763);
        assert(c2.dx == 1);
        assert(c2.dy == 32'765);

        avt::utils::Offsets c4 = std::vector<float>{ 1.f, -1.9f, 4.5f } + c2;
        assert(c4.dx == 2);
        assert(c4.dy == 32'763);
        assert(c2.dx == 1);
        assert(c2.dy == 32'765);

        avt::utils::Offsets c5 = c2 + std::array<char, 2>{ 1, -2 };
        assert(c5.dx == 2);
        assert(c5.dy == 32'763);
        assert(c2.dx == 1);
        assert(c2.dy == 32'765);

        avt::utils::Offsets c6 = std::array<char, 2>{ 1, -2 } + c2;
        assert(c6.dx == 2);
        assert(c6.dy == 32'763);
        assert(c2.dx == 1);
        assert(c2.dy == 32'765);

        assert(c3 == c4);
        assert(c4 == c3);
        assert(!(c3 != c4));
        assert(!(c4 != c3));
        assert(c2 != c3);
        assert(!(c2 == c3));

        c1 = { 4L, 37.6f };
        assert(c1.dx == 4);
        assert(c1.dy == 38);

        c2 = { 3, 1'023 };
        assert(c2.dx == 3);
        assert(c2.dy == 1'023);

        c2 -= c1;
        assert(c2.dx == -1);
        assert(c2.dy == 985);

        c2 -= std::vector<long>{ -32'767, -40'000, 16, 25 };
        assert(c2.dx == 32'766);
        assert(c2.dy == 32'767);

        c2 -= std::array<float, 2>{ 1.f, 2.9f };
        assert(c2.dx == 32'765);
        assert(c2.dy == 32'764);

        c3 = c2 - c1;
        assert(c3.dx == 32'761);
        assert(c3.dy == 32'726);
        assert(c2.dx == 32'765);
        assert(c2.dy == 32'764);

        c3 = c2 - std::vector<float>{ 1.f, 1.9f };
        assert(c3.dx == 32'764);
        assert(c3.dy == 32'762);
        assert(c2.dx == 32'765);
        assert(c2.dy == 32'764);

        c4 = std::vector<float>{ 1.f, -2.9f, 4.5f } - c2;
        assert(c4.dx == -32'764);
        assert(c4.dy == -32'767);
        assert(c2.dx == 32'765);
        assert(c2.dy == 32'764);

        c3 = c2 - std::array<char, 2>{ 1, 2 };
        assert(c3.dx == 32'764);
        assert(c3.dy == 32'762);
        assert(c2.dx == 32'765);
        assert(c2.dy == 32'764);

        c4 = std::array<float, 2>{ 1.f, -2.9f } - c2;
        assert(c4.dx == -32'764);
        assert(c4.dy == -32'767);
        assert(c2.dx == 32'765);
        assert(c2.dy == 32'764);

        c2 = avt::utils::Offsets{ 3, 1'023 };
        c2 *= 2.3f;
        assert(c2.dx == 7);
        assert(c2.dy == 2'353);

        avt::utils::Offsets c7 = c2 * 1.1;
        assert(c7.dx == 8);
        assert(c7.dy == 2'588);

        c7 = 2.1 * c2;
        assert(c7.dx == 15);
        assert(c7.dy == 4'941);

        avt::utils::Offsets c8 = c2 / 1.1f;
        assert(c8.dx == 6);
        assert(c8.dy == 2'139);

        c8 /= 0.9;
        assert(c8.dx == 7);
        assert(c8.dy == 2'377);


        std::cout << "   All tests OK\n\n";
    }
}