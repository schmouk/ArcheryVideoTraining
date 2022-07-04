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

export module unit_tests.utils.test_size;

import utils.offsets;
import utils.size;



//===========================================================================
export namespace avt::unit_tests
{
    //=======================================================================
    void test_size()
    {
        std::cout << "-- TEST avt::utils::Size\n";

        avt::utils::Size s0;
        assert(s0.width == 0);
        assert(s0.height == 0);

        avt::utils::Size s1(-65'000L, 3.9);
        assert(s1.width == -32'768);
        assert(s1.height == 4);

        avt::utils::Size s2(s1);
        assert(s2.width == -32'768);
        assert(s2.height == 4);

        s2 = s0;
        assert(s2.width == 0);
        assert(s2.height == 0);

        s2 = { (unsigned char)1, 3ULL };
        assert(s2.width == 1);
        assert(s2.height == 3);

        s2 = std::vector<double>{ 2.3, 23.5, 0.9 };
        assert(s2.width == 2);
        assert(s2.height == 24);

        s2 = std::array<float, 2>{ 1.1f, 1'023.6f };
        assert(s2.width == 1);
        assert(s2.height == 1'024);

        avt::utils::Size s3 = s1 + avt::utils::Offsets(s2);
        assert(s3.width == -32'767);
        assert(s3.height == 1028);

        s2 += avt::utils::Offsets(s1);
        assert(s2.width == -32'767);
        assert(s2.height == 1'028);

        s2 += std::vector<long>{ 32'767, 40'000, 16, 25 };
        assert(s2.width == 0);
        assert(s2.height == 32'767);

        s2 += std::array<float, 2>{ 1.f, -1.9f };
        assert(s2.width == 1);
        assert(s2.height == 32'765);

        s3 = s2 + std::vector<float>{ 1.f, -1.9f };
        assert(s3.width == 2);
        assert(s3.height == 32'763);
        assert(s2.width == 1);
        assert(s2.height == 32'765);

        avt::utils::Size s4 = std::vector<float>{ 1.f, -1.9f, 4.5f } + s2;
        assert(s4.width == 2);
        assert(s4.height == 32'763);
        assert(s2.width == 1);
        assert(s2.height == 32'765);

        avt::utils::Size s5 = s2 + std::array<char, 2>{ 1, -2 };
        assert(s5.width == 2);
        assert(s5.height == 32'763);
        assert(s2.width == 1);
        assert(s2.height == 32'765);

        avt::utils::Size s6 = std::array<char, 2>{ 1, -2 } + s2;
        assert(s6.width == 2);
        assert(s6.height == 32'763);
        assert(s2.width == 1);
        assert(s2.height == 32'765);

        assert(s3 == s4);
        assert(s4 == s3);
        assert(!(s3 != s4));
        assert(!(s4 != s3));
        assert(s2 != s3);
        assert(!(s2 == s3));

        s1 = { 4L, 37.6f };
        assert(s1.width == 4);
        assert(s1.height == 38);

        s2 = { 3, 1'023 };
        assert(s2.width == 3);
        assert(s2.height == 1'023);

        s2 -= avt::utils::Offsets(s1.width, s1.height);
        assert(s2.width == -1);
        assert(s2.height == 985);

        s2 -= std::vector<long>{ -32'767, -40'000, 16, 25 };
        assert(s2.width == 32'766);
        assert(s2.height == 32'767);

        s2 -= std::array<float, 2>{ 1.f, 2.9f };
        assert(s2.width == 32'765);
        assert(s2.height == 32'764);

        s3 = s2 - avt::utils::Offsets(s1);
        assert(s3.width == 32'761);
        assert(s3.height == 32'726);
        assert(s2.width == 32'765);
        assert(s2.height == 32'764);

        s3 = s2 - std::vector<float>{ 1.f, 1.9f };
        assert(s3.width == 32'764);
        assert(s3.height == 32'762);
        assert(s2.width == 32'765);
        assert(s2.height == 32'764);

        s4 = std::vector<float>{ 1.f, -2.9f, 4.5f } - s2;
        assert(s4.width == -32'764);
        assert(s4.height == -32'767);
        assert(s2.width == 32'765);
        assert(s2.height == 32'764);

        s3 = s2 - std::array<char, 2>{ 1, 2 };
        assert(s3.width == 32'764);
        assert(s3.height == 32'762);
        assert(s2.width == 32'765);
        assert(s2.height == 32'764);

        s4 = std::array<float, 2>{ 1.f, -2.9f } - s2;
        assert(s4.width == -32'764);
        assert(s4.height == -32'767);
        assert(s2.width == 32'765);
        assert(s2.height == 32'764);

        s2 = avt::utils::Size{ 3, 1'023 };
        s2 *= 2.3f;
        assert(s2.width == 7);
        assert(s2.height == 2'353);

        avt::utils::Size s7 = s2 * 1.1;
        assert(s7.width == 8);
        assert(s7.height == 2'588);

        s7 = 2.1 * s2;
        assert(s7.width == 15);
        assert(s7.height == 4'941);

        avt::utils::Size s8 = s2 / 1.1f;
        assert(s8.width == 6);
        assert(s8.height == 2'139);

        s8 /= 0.9;
        assert(s8.width == 7);
        assert(s8.height == 2'377);


        std::cout << "   All tests OK\n\n";
    }
}