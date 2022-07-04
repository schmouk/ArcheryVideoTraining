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

export module unit_tests.utils.test_base2d;

import utils.base2d;



//===========================================================================
export namespace avt::unit_tests
{
    //=======================================================================
    void test_base2d()
    {
        std::cout << "-- TEST avt::utils::Base2D\n";

        avt::utils::Base2D b0;
        assert(b0.x == 0);
        assert(b0.y == 0);

        avt::utils::Base2D b1(-65'000L, 3.9);
        assert(b1.x == -32'768);
        assert(b1.y == 4);

        avt::utils::Base2D b2(b1);
        assert(b2.x == -32'768);
        assert(b2.y == 4);

        b2 = b0;
        assert(b2.x == 0);
        assert(b2.y == 0);

        b2 = {(unsigned char)1, 3ULL};
        assert(b2.x == 1);
        assert(b2.y == 3);

        b2 = std::vector<double>{ 2.3, 23.5, 0.9 };
        assert(b2.x == 2);
        assert(b2.y == 24);

        b2 = std::array<float, 2>{ 1.1f, 1'023.6f };
        assert(b2.x == 1);
        assert(b2.y == 1'024);

        avt::utils::Base2D b3 = b1 + b2;
        assert(b3.x == -32'767);
        assert(b3.y == 1028);

        b2 += b1;
        assert(b2.x == -32'767);
        assert(b2.y == 1'028);

        b2 += std::vector<long>{ 32'767, 40'000, 16, 25 };
        assert(b2.x == 0);
        assert(b2.y == 32'767);

        b2 += std::array<float, 2>{ 1.f, -1.9f };
        assert(b2.x == 1);
        assert(b2.y == 32'765);

        b3 = b2 + std::vector<float>{ 1.f, -1.9f };
        assert(b3.x == 2);
        assert(b3.y == 32'763);
        assert(b2.x == 1);
        assert(b2.y == 32'765);

        avt::utils::Base2D b4 = std::vector<float>{ 1.f, -1.9f, 4.5f } + b2;
        assert(b4.x == 2);
        assert(b4.y == 32'763);
        assert(b2.x == 1);
        assert(b2.y == 32'765);

        avt::utils::Base2D b5 = b2 + std::array<char, 2>{ 1, -2 };
        assert(b5.x == 2);
        assert(b5.y == 32'763);
        assert(b2.x == 1);
        assert(b2.y == 32'765);

        avt::utils::Base2D b6 = std::array<char, 2>{ 1, -2 } + b2;
        assert(b6.x == 2);
        assert(b6.y == 32'763);
        assert(b2.x == 1);
        assert(b2.y == 32'765);

        assert(b3 == b4);
        assert(b4 == b3);
        assert(!(b3 != b4));
        assert(!(b4 != b3));
        assert(b2 != b3);
        assert(!(b2 == b3));

        b1 = { 4L, 37.6f };
        assert(b1.x == 4);
        assert(b1.y == 38);

        b2 = { 3, 1'023 };
        assert(b2.x == 3);
        assert(b2.y == 1'023);

        b2 -= b1;
        assert(b2.x == -1);
        assert(b2.y == 985);

        b2 -= std::vector<long>{ -32'767, -40'000, 16, 25 };
        assert(b2.x == 32'766);
        assert(b2.y == 32'767);

        b2 -= std::array<float, 2>{ 1.f, 2.9f };
        assert(b2.x == 32'765);
        assert(b2.y == 32'764);

        b3 = b2 - b1;
        assert(b3.x == 32'761);
        assert(b3.y == 32'726);
        assert(b2.x == 32'765);
        assert(b2.y == 32'764);

        b3 = b2 - std::vector<float>{ 1.f, 1.9f };
        assert(b3.x == 32'764);
        assert(b3.y == 32'762);
        assert(b2.x == 32'765);
        assert(b2.y == 32'764);

        b4 = std::vector<float>{ 1.f, -2.9f, 4.5f } - b2;
        assert(b4.x == -32'764);
        assert(b4.y == -32'767);
        assert(b2.x == 32'765);
        assert(b2.y == 32'764);

        b3 = b2 - std::array<char, 2>{ 1, 2 };
        assert(b3.x == 32'764);
        assert(b3.y == 32'762);
        assert(b2.x == 32'765);
        assert(b2.y == 32'764);

        b4 = std::array<float, 2>{ 1.f, -2.9f } - b2;
        assert(b4.x == -32'764);
        assert(b4.y == -32'767);
        assert(b2.x == 32'765);
        assert(b2.y == 32'764);

        b2 = avt::utils::Base2D{ 3, 1'023 };
        b2 *= 2.3f;
        assert(b2.x == 7);
        assert(b2.y == 2'353);

        avt::utils::Base2D b7 = b2 * 1.1;
        assert(b7.x == 8);
        assert(b7.y == 2'588);

        b7 = 2.1 * b2;
        assert(b7.x == 15);
        assert(b7.y == 4'941);

        avt::utils::Base2D b8 = b2 / 1.1f;
        assert(b8.x == 6);
        assert(b8.y == 2'139);

        b8 /= 0.9;
        assert(b8.x == 7);
        assert(b8.y == 2'377);


        std::cout << "   All tests OK\n\n";
    }
}