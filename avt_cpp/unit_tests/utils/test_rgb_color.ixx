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

export module unit_tests.utils.test_rgb_color;

import utils.rgb_color;


//===========================================================================
export namespace avt::unit_tests
{
    //=======================================================================
    void test_rgb_color()
    {
        std::cout << "-- TEST avt::utils::RGBColor\n";

        avt::utils::RGBColor r0;
        assert(r0.r == 0);
        assert(r0.g == 0);
        assert(r0.b == 0);
        assert(r0.bgr[0] == 0);
        assert(r0.bgr[1] == 0);
        assert(r0.bgr[2] == 0);

        r0 = { 1.1, 2.2, 3.3 };
        assert(r0.r == 1);
        assert(r0.g == 2);
        assert(r0.b == 3);
        assert(r0.bgr[0] == 3);
        assert(r0.bgr[1] == 2);
        assert(r0.bgr[2] == 1);

        avt::utils::RGBColor r123(1, (unsigned char)2, 3.0f);
        assert(r123.r == 1);
        assert(r123.g == 2);
        assert(r123.b == 3);
        assert(r123.bgr[0] == 3);
        assert(r123.bgr[1] == 2);
        assert(r123.bgr[2] == 1);

        avt::utils::RGBColor r1(127);
        assert(r1.r == 127);
        assert(r1.g == 127);
        assert(r1.b == 127);
        assert(r1.bgr[0] == 127);
        assert(r1.bgr[1] == 127);
        assert(r1.bgr[2] == 127);

        avt::utils::RGBColor r2({ -1.0, 127.9, 256.1 });
        assert(r2.r == 0);
        assert(r2.g == 127);
        assert(r2.b == 255);
        assert(r2.bgr[0] == 255);
        assert(r2.bgr[1] == 127);
        assert(r2.bgr[2] == 0);

        avt::utils::RGBColor r3(std::vector<short>{32767, 128, -1});
        assert(r3.r == 255);
        assert(r3.g == 128);
        assert(r3.b == 0);
        assert(r3.bgr[0] == 0);
        assert(r3.bgr[1] == 128);
        assert(r3.bgr[2] == 255);

        avt::utils::RGBColor r4(std::array<long long, 3>{-1LL, 127LL, 0x7fff'ffff'ffff'ffffLL});
        assert(r4.r == 0);
        assert(r4.g == 127);
        assert(r4.b == 255);
        assert(r4.bgr[0] == 255);
        assert(r4.bgr[1] == 127);
        assert(r4.bgr[2] == 0);

        avt::utils::RGBColor r5(r123);
        assert(r5.r == 1);
        assert(r5.g == 2);
        assert(r5.b == 3);
        assert(r5.bgr[0] == 3);
        assert(r5.bgr[1] == 2);
        assert(r5.bgr[2] == 1);

        avt::utils::RGBColor r6{ r5 };
        assert(r6.r == 1);
        assert(r6.g == 2);
        assert(r6.b == 3);
        assert(r6.bgr[0] == 3);
        assert(r6.bgr[1] == 2);
        assert(r6.bgr[2] == 1);

        r6 = r2;
        assert(r6.r == 0);
        assert(r6.g == 127);
        assert(r6.b == 255);
        assert(r6.bgr[0] == 255);
        assert(r6.bgr[1] == 127);
        assert(r6.bgr[2] == 0);

        r6 = { short(-3), short(33), short(333) };
        assert(r6.r == 0);
        assert(r6.g == 33);
        assert(r6.b == 255);
        assert(r6.bgr[0] == 255);
        assert(r6.bgr[1] == 33);
        assert(r6.bgr[2] == 0);

        r6 = std::vector<short>{ -1, 127, 32767 };
        assert(r6.r == 0);
        assert(r6.g == 127);
        assert(r6.b == 255);
        assert(r6.bgr[0] == 255);
        assert(r6.bgr[1] == 127);
        assert(r6.bgr[2] == 0);

        r6 = std::array<long long, 3>{1LL, 128LL, 254LL};
        assert(r6.r == 1);
        assert(r6.g == 128);
        assert(r6.b == 254);
        assert(r6.bgr[0] == 254);
        assert(r6.bgr[1] == 128);
        assert(r6.bgr[2] == 1);

        r6.clr();
        assert(r6.r == 0);
        assert(r6.g == 0);
        assert(r6.b == 0);
        assert(r6.bgr[0] == 0);
        assert(r6.bgr[1] == 0);
        assert(r6.bgr[2] == 0);

        r6.set(r123);
        assert(r6.r == 1);
        assert(r6.g == 2);
        assert(r6.b == 3);
        assert(r6.bgr[0] == 3);
        assert(r6.bgr[1] == 2);
        assert(r6.bgr[2] == 1);

        r6.set(3, 2, 1);
        assert(r6.r == 3);
        assert(r6.g == 2);
        assert(r6.b == 1);
        assert(r6.bgr[0] == 1);
        assert(r6.bgr[1] == 2);
        assert(r6.bgr[2] == 3);

        r6.set({ -1, 127, 0x7fff'ffff });
        assert(r6.r == 0);
        assert(r6.g == 127);
        assert(r6.b == 255);
        assert(r6.bgr[0] == 255);
        assert(r6.bgr[1] == 127);
        assert(r6.bgr[2] == 0);

        r6.set(std::array<long long, 3>{1LL, 128LL, 254LL});
        assert(r6.r == 1);
        assert(r6.g == 128);
        assert(r6.b == 254);
        assert(r6.bgr[0] == 254);
        assert(r6.bgr[1] == 128);
        assert(r6.bgr[2] == 1);

        r6.set(std::vector<short>{ -1, 127, 32767 });
        assert(r6.r == 0);
        assert(r6.g == 127);
        assert(r6.b == 255);
        assert(r6.bgr[0] == 255);
        assert(r6.bgr[1] == 127);
        assert(r6.bgr[2] == 0);

        assert(r1.get_lum() == 127);
        assert(r6.get_lum() == 104); // (r, g, b) == (0, 127, 255)

        avt::utils::RGBColor r7 = r6.to_gray_color();
        assert(r7.r == 104);
        assert(r7.g == 104);
        assert(r7.b == 104);
        assert(r7.bgr[0] == 104);
        assert(r7.bgr[1] == 104);
        assert(r7.bgr[2] == 104);

        assert(r6 == r4);
        assert(r4 != r5);
        assert(!(r6 != r4));
        assert(!(r4 == r5));

        r123 += 6;
        assert(r123.r == 7);
        assert(r123.g == 8);
        assert(r123.b == 9);
        assert(r123.bgr[0] == 9);
        assert(r123.bgr[1] == 8);
        assert(r123.bgr[2] == 7);

        r123 += r6;
        assert(r123.r == 7);
        assert(r123.g == 135);
        assert(r123.b == 255);
        assert(r123.bgr[0] == 255);
        assert(r123.bgr[1] == 135);
        assert(r123.bgr[2] == 7);

        std::vector<short> v1{ -8, -6, 4 };
        r123 += v1;
        assert(r123.r == 0);
        assert(r123.g == 129);
        assert(r123.b == 255);
        assert(r123.bgr[0] == 255);
        assert(r123.bgr[1] == 129);
        assert(r123.bgr[2] == 0);

        r123 += std::array<double, 3>{ 1.9, 2.1, -3.9};
        assert(r123.r == 1);
        assert(r123.g == 131);
        assert(r123.b == 251);
        assert(r123.bgr[0] == 251);
        assert(r123.bgr[1] == 131);
        assert(r123.bgr[2] == 1);

        r123 = avt::utils::RGBColor(1, 2, 3);
        r6 = r1 + r123;
        assert(r6.r == 128);
        assert(r6.g == 129);
        assert(r6.b == 130);
        assert(r6.bgr[0] == 130);
        assert(r6.bgr[1] == 129);
        assert(r6.bgr[2] == 128);

        r6 = r123 + 253.9f;
        assert(r6.r == 254);
        assert(r6.g == 255);
        assert(r6.b == 255);
        assert(r6.bgr[0] == 255);
        assert(r6.bgr[1] == 255);
        assert(r6.bgr[2] == 254);

        r6 = -1 + r123;
        assert(r6.r == 0);
        assert(r6.g == 1);
        assert(r6.b == 2);
        assert(r6.bgr[0] == 2);
        assert(r6.bgr[1] == 1);
        assert(r6.bgr[2] == 0);

        r6 = r123 + std::vector<unsigned long>{1UL, 2UL, 3UL};
        assert(r6.r == 2);
        assert(r6.g == 4);
        assert(r6.b == 6);
        assert(r6.bgr[0] == 6);
        assert(r6.bgr[1] == 4);
        assert(r6.bgr[2] == 2);

        r6 = std::vector<float>{ 5.0f, 3.0f, 1.0f } + r123;
        assert(r6.r == 6);
        assert(r6.g == 5);
        assert(r6.b == 4);
        assert(r6.bgr[0] == 4);
        assert(r6.bgr[1] == 5);
        assert(r6.bgr[2] == 6);

        r6 = r123 + std::array<unsigned long, 3>{1UL, 2UL, 3UL};
        assert(r6.r == 2);
        assert(r6.g == 4);
        assert(r6.b == 6);
        assert(r6.bgr[0] == 6);
        assert(r6.bgr[1] == 4);
        assert(r6.bgr[2] == 2);

        r6 = std::array<float, 3>{ 5.0f, 3.0f, 1.0f } + r123;
        assert(r6.r == 6);
        assert(r6.g == 5);
        assert(r6.b == 4);
        assert(r6.bgr[0] == 4);
        assert(r6.bgr[1] == 5);
        assert(r6.bgr[2] == 6);

        r6 = { 129, 128, 127 };
        r6 -= r123;
        assert(r6.r == 128);
        assert(r6.g == 126);
        assert(r6.b == 124);
        assert(r6.bgr[0] == 124);
        assert(r6.bgr[1] == 126);
        assert(r6.bgr[2] == 128);

        r6 -= 125;
        assert(r6.r == 3);
        assert(r6.g == 1);
        assert(r6.b == 0);
        assert(r6.bgr[0] == 0);
        assert(r6.bgr[1] == 1);
        assert(r6.bgr[2] == 3);

        r6 = r1;
#pragma warning(push)
#pragma warning(disable: 4244)
        r6.r -= 1.1;
        r6.g -= 2.1;
        r6.b -= 3.1;
#pragma warning(pop)
        assert(r6.r == 125);
        assert(r6.g == 124);
        assert(r6.b == 123);
        assert(r6.bgr[0] == 123);
        assert(r6.bgr[1] == 124);
        assert(r6.bgr[2] == 125);

        r6 -= std::vector<long>{ 1L, 2L, 3L };
        assert(r6.r == 124);
        assert(r6.g == 122);
        assert(r6.b == 120);
        assert(r6.bgr[0] == 120);
        assert(r6.bgr[1] == 122);
        assert(r6.bgr[2] == 124);

        r6 -= std::array<short, 3>{ 1, 2, 3 };
        assert(r6.r == 123);
        assert(r6.g == 120);
        assert(r6.b == 117);
        assert(r6.bgr[0] == 117);
        assert(r6.bgr[1] == 120);
        assert(r6.bgr[2] == 123);

        r6 = r1 - r123;
        assert(r6.r == 126);
        assert(r6.g == 125);
        assert(r6.b == 124);
        assert(r6.bgr[0] == 124);
        assert(r6.bgr[1] == 125);
        assert(r6.bgr[2] == 126);

        r6 = r4 - 1;
        assert(r6.r == 0);
        assert(r6.g == 126);
        assert(r6.b == 254);
        assert(r6.bgr[0] == 254);
        assert(r6.bgr[1] == 126);
        assert(r6.bgr[2] == 0);

        r6 = 256 - r4;
        assert(r6.r == 255);
        assert(r6.g == 129);
        assert(r6.b == 1);
        assert(r6.bgr[0] == 1);
        assert(r6.bgr[1] == 129);
        assert(r6.bgr[2] == 255);

        r6 = avt::utils::RGBColor(127, 128, 129) - std::vector<long>{3L, 2L, 1L};
        assert(r6.r == 124);
        assert(r6.g == 126);
        assert(r6.b == 128);
        assert(r6.bgr[0] == 128);
        assert(r6.bgr[1] == 126);
        assert(r6.bgr[2] == 124);

        r6 = std::vector<double>{ 129.0, 128.1, 127.9 } - r123;
        assert(r6.r == 128);
        assert(r6.g == 126);
        assert(r6.b == 124);
        assert(r6.bgr[0] == 124);
        assert(r6.bgr[1] == 126);
        assert(r6.bgr[2] == 128);

        r6 = avt::utils::RGBColor(127, 128, 129) - std::array<long, 3>{3L, 2L, 1L};
        assert(r6.r == 124);
        assert(r6.g == 126);
        assert(r6.b == 128);
        assert(r6.bgr[0] == 128);
        assert(r6.bgr[1] == 126);
        assert(r6.bgr[2] == 124);

        r6 = std::array<double, 3>{ 129.0, 128.1, 127.9 } - r123;
        assert(r6.r == 128);
        assert(r6.g == 126);
        assert(r6.b == 124);
        assert(r6.bgr[0] == 124);
        assert(r6.bgr[1] == 126);
        assert(r6.bgr[2] == 128);

        r6 *= 1.1;
        assert(r6.r == 140);
        assert(r6.g == 138);
        assert(r6.b == 136);
        assert(r6.bgr[0] == 136);
        assert(r6.bgr[1] == 138);
        assert(r6.bgr[2] == 140);

        r6 = std::array<double, 3>{ 126, 151, 163 };
        r6 *= std::vector<double>{0.9, 1.1, 1.2};
        assert(r6.r == 113);
        assert(r6.g == 166);
        assert(r6.b == 195);
        assert(r6.bgr[0] == 195);
        assert(r6.bgr[1] == 166);
        assert(r6.bgr[2] == 113);

        r6 *= std::array<float, 3>{0.9f, 1.1f, 1.2f};
        assert(r6.r == 101);
        assert(r6.g == 182);
        assert(r6.b == 234);
        assert(r6.bgr[0] == 234);
        assert(r6.bgr[1] == 182);
        assert(r6.bgr[2] == 101);

        r6 = avt::utils::RGBColor(128, 126, 124) * 1.1;
        assert(r6.r == 140);
        assert(r6.g == 138);
        assert(r6.b == 136);
        assert(r6.bgr[0] == 136);
        assert(r6.bgr[1] == 138);
        assert(r6.bgr[2] == 140);

        r6 = 0.9 * avt::utils::RGBColor(128, 126, 124);
        assert(r6.r == 115);
        assert(r6.g == 113);
        assert(r6.b == 111);
        assert(r6.bgr[0] == 111);
        assert(r6.bgr[1] == 113);
        assert(r6.bgr[2] == 115);

        r6 = avt::utils::RGBColor(136, 138, 140) * std::vector<double>{ 0.9, 1.1, 1.2 };
        assert(r6.r == 122);
        assert(r6.g == 151);
        assert(r6.b == 168);
        assert(r6.bgr[0] == 168);
        assert(r6.bgr[1] == 151);
        assert(r6.bgr[2] == 122);

        r6 = std::vector<float>{ 1.2f, 0.9f, 1.1f } * avt::utils::RGBColor(136, 138, 140);
        assert(r6.r == 163);
        assert(r6.g == 124);
        assert(r6.b == 154);
        assert(r6.bgr[0] == 154);
        assert(r6.bgr[1] == 124);
        assert(r6.bgr[2] == 163);

        r6 = avt::utils::RGBColor(136, 138, 140) * std::array<double, 3>{ 0.9, 1.1, 1.2 };
        assert(r6.r == 122);
        assert(r6.g == 151);
        assert(r6.b == 168);
        assert(r6.bgr[0] == 168);
        assert(r6.bgr[1] == 151);
        assert(r6.bgr[2] == 122);

        r6 = std::array<float, 3>{ 1.2f, 0.9f, 1.1f } * avt::utils::RGBColor(136, 138, 140);
        assert(r6.r == 163);
        assert(r6.g == 124);
        assert(r6.b == 154);
        assert(r6.bgr[0] == 154);
        assert(r6.bgr[1] == 124);
        assert(r6.bgr[2] == 163);

        r6 /= 1.1f;
        assert(r6.r == 148);
        assert(r6.g == 112);
        assert(r6.b == 140);
        assert(r6.bgr[0] == 140);
        assert(r6.bgr[1] == 112);
        assert(r6.bgr[2] == 148);

        r6 = std::array<double, 3>{ 126, 151, 163 };
        r6 /= std::vector<double>{1.0 / 0.9, 1.0 / 1.1, 1.0 / 1.2};
        assert(r6.r == 113);
        assert(r6.g == 166);
        assert(r6.b == 195);
        assert(r6.bgr[0] == 195);
        assert(r6.bgr[1] == 166);
        assert(r6.bgr[2] == 113);

        r6 /= std::array<float, 3>{1.0f / 0.9f, 1.0f / 1.1f, 1.0f / 1.2f};
        assert(r6.r == 101);
        assert(r6.g == 182);
        assert(r6.b == 234);
        assert(r6.bgr[0] == 234);
        assert(r6.bgr[1] == 182);
        assert(r6.bgr[2] == 101);

        r6 = avt::utils::RGBColor(128, 126, 124) / (1.0 / 1.1);
        assert(r6.r == 140);
        assert(r6.g == 138);
        assert(r6.b == 136);
        assert(r6.bgr[0] == 136);
        assert(r6.bgr[1] == 138);
        assert(r6.bgr[2] == 140);

        r6 = 16384 / avt::utils::RGBColor(128, 126, 124);
        assert(r6.r == 128);
        assert(r6.g == 130);
        assert(r6.b == 132);
        assert(r6.bgr[0] == 132);
        assert(r6.bgr[1] == 130);
        assert(r6.bgr[2] == 128);

        r6 = avt::utils::RGBColor(136, 138, 140) / std::vector<double>{ 1.0 / 0.9, 1.0 / 1.1, 1.0 / 1.2 };
        assert(r6.r == 122);
        assert(r6.g == 151);
        assert(r6.b == 168);
        assert(r6.bgr[0] == 168);
        assert(r6.bgr[1] == 151);
        assert(r6.bgr[2] == 122);

        r6 = std::vector<float>{ 22'200.1f, 17'115.9f, 21'600.0f } / avt::utils::RGBColor(136, 138, 140);
        assert(r6.r == 163);
        assert(r6.g == 124);
        assert(r6.b == 154);
        assert(r6.bgr[0] == 154);
        assert(r6.bgr[1] == 124);
        assert(r6.bgr[2] == 163);

        r6 = avt::utils::RGBColor(136, 138, 140) / std::array<double, 3>{ 1.0 / 0.9, 1.0 / 1.1, 1.0 / 1.2 };
        assert(r6.r == 122);
        assert(r6.g == 151);
        assert(r6.b == 168);
        assert(r6.bgr[0] == 168);
        assert(r6.bgr[1] == 151);
        assert(r6.bgr[2] == 122);

        r6 = std::array<float, 3>{ 22'200.1f, 17'115.9f, 21'600.0f } / avt::utils::RGBColor(136, 138, 140);
        assert(r6.r == 163);
        assert(r6.g == 124);
        assert(r6.b == 154);
        assert(r6.bgr[0] == 154);
        assert(r6.bgr[1] == 124);
        assert(r6.bgr[2] == 163);

        std::cout << "   All tests OK\n\n";
    }
}