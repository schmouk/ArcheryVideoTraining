#pragma once
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

#if defined(_UNIT_TESTS)

//===========================================================================
#include <iostream>

/** /import unit_tests.utils.test_base2d;
import unit_tests.utils.test_coords;
import unit_tests.utils.test_gray_color;
import unit_tests.utils.test_offsets;
import unit_tests.utils.test_size;
import unit_tests.utils.test_rgb_color;
/**/
import unit_tests.mtmp.test_threads;


//===========================================================================
int main()
{
    std::cout << "\n===================================================";
    std::cout << "\n=====   UNIT TESTS - Archery Video Training   =====";
    std::cout << "\n===================================================\n\n";

    /** /
    avt::unit_tests::test_base2d();
    avt::unit_tests::test_coords();
    avt::unit_tests::test_offsets();
    avt::unit_tests::test_size();
    avt::unit_tests::test_rgb_color();
    avt::unit_tests::test_gray_color();
    /**/
    mtmp::unit_tests::test_threads();

    std::cout << "\n >>>>>>>>>>   EVERYTHING WENT FINE   <<<<<<<<<<\n\n";
}

#endif
