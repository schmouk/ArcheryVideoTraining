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

#include <cassert>
#include <iostream>

export module unit_tests.utils.test_circular_buffer;

import utils.buffers;


//===========================================================================
export namespace avt::unit_tests::utils
{
    //=======================================================================
    void test_circular_buffer()
    {
        std::cout << "-- TEST avt::utils::CircularBuffer\n";

        avt::utils::CircularBuffer<short, 5> cb0;
        assert(cb0.index == 0);
        assert(cb0.count == 0);
        assert(cb0.MAX_SIZE == 5);
        assert(cb0[0] == 0);
        assert(cb0[1] == 0);
        assert(cb0[2] == 0);
        assert(cb0[3] == 0);
        assert(cb0[4] == 0);
        assert(cb0.buffer[0] == 0);
        assert(cb0.buffer[1] == 0);
        assert(cb0.buffer[2] == 0);
        assert(cb0.buffer[3] == 0);
        assert(cb0.buffer[4] == 0);
        assert(cb0.is_empty());
        assert(!cb0.is_full());
        assert(cb0.is_nearly_full(5));
        for (int i = 1; i < 5; ++i)
            assert(!cb0.is_nearly_full(i));

        cb0.append(11);
        assert(cb0.get_latest() == 11);
        assert(cb0.get_oldest() == 0);
        assert(cb0.index == 1);
        assert(cb0.count == 1);
        assert(cb0[-1] == 11);
        assert(cb0[0] == 0);
        assert(cb0[1] == 0);
        assert(cb0[2] == 0);
        assert(cb0[3] == 0);
        assert(cb0[4] == 11);
        assert(cb0.buffer[0] == 11);
        assert(cb0.buffer[1] == 0);
        assert(cb0.buffer[2] == 0);
        assert(cb0.buffer[3] == 0);
        assert(cb0.buffer[4] == 0);
        assert(!cb0.is_empty());
        assert(!cb0.is_full());
        for (int i=4; i<=5; ++i)
            assert(cb0.is_nearly_full(i));
        for (int i = 1; i < 4; ++i)
            assert(!cb0.is_nearly_full(i));

        cb0 += 22;
        assert(cb0.get_latest() == 22);
        assert(cb0.get_oldest() == 0);
        assert(cb0.index == 2);
        assert(cb0.count == 2);
        assert(cb0[-2] == 11);
        assert(cb0[-1] == 22);
        assert(cb0[0] == 0);
        assert(cb0[1] == 0);
        assert(cb0[2] == 0);
        assert(cb0[3] == 11);
        assert(cb0[4] == 22);
        assert(cb0.buffer[0] == 11);
        assert(cb0.buffer[1] == 22);
        assert(cb0.buffer[2] == 0);
        assert(cb0.buffer[3] == 0);
        assert(cb0.buffer[4] == 0);
        assert(!cb0.is_empty());
        assert(!cb0.is_full());
        for (int i = 3; i <= 5; ++i)
            assert(cb0.is_nearly_full(i));
        for (int i = 1; i < 3; ++i)
            assert(!cb0.is_nearly_full(i));

        cb0 += 33;
        assert(cb0.get_latest() == 33);
        assert(cb0.get_oldest() == 0);
        assert(cb0.index == 3);
        assert(cb0.count == 3);
        assert(cb0[-3] == 11);
        assert(cb0[-2] == 22);
        assert(cb0[-1] == 33);
        assert(cb0[0] == 0);
        assert(cb0[1] == 0);
        assert(cb0[2] == 11);
        assert(cb0[3] == 22);
        assert(cb0[4] == 33);
        assert(cb0.buffer[0] == 11);
        assert(cb0.buffer[1] == 22);
        assert(cb0.buffer[2] == 33);
        assert(cb0.buffer[3] == 0);
        assert(cb0.buffer[4] == 0);
        assert(!cb0.is_empty());
        assert(!cb0.is_full());
        for (int i = 2; i <= 5; ++i)
            assert(cb0.is_nearly_full(i));
        for (int i = 1; i < 2; ++i)
            assert(!cb0.is_nearly_full(i));

        cb0.append(44);
        assert(cb0.get_latest() == 44);
        assert(cb0.get_oldest() == 0);
        assert(cb0.index == 4);
        assert(cb0.count == 4);
        assert(cb0[-4] == 11);
        assert(cb0[-3] == 22);
        assert(cb0[-2] == 33);
        assert(cb0[-1] == 44);
        assert(cb0[0] == 0);
        assert(cb0[1] == 11);
        assert(cb0[2] == 22);
        assert(cb0[3] == 33);
        assert(cb0[4] == 44);
        assert(cb0.buffer[0] == 11);
        assert(cb0.buffer[1] == 22);
        assert(cb0.buffer[2] == 33);
        assert(cb0.buffer[3] == 44);
        assert(cb0.buffer[4] == 0);
        assert(!cb0.is_empty());
        assert(!cb0.is_full());
        for (int i = 1; i <= 5; ++i)
            assert(cb0.is_nearly_full(i));
        for (int i = 1; i < 1; ++i)
            assert(!cb0.is_nearly_full(i));

        cb0.append(55);
        assert(cb0.get_latest() == 55);
        assert(cb0.get_oldest() == 11);
        assert(cb0.index == 0);
        assert(cb0.count == 5);
        assert(cb0[-5] == 11);
        assert(cb0[-4] == 22);
        assert(cb0[-3] == 33);
        assert(cb0[-2] == 44);
        assert(cb0[-1] == 55);
        assert(cb0[0] == 11);
        assert(cb0[1] == 22);
        assert(cb0[2] == 33);
        assert(cb0[3] == 44);
        assert(cb0[4] == 55);
        assert(cb0.buffer[0] == 11);
        assert(cb0.buffer[1] == 22);
        assert(cb0.buffer[2] == 33);
        assert(cb0.buffer[3] == 44);
        assert(cb0.buffer[4] == 55);
        assert(!cb0.is_empty());
        assert(cb0.is_full());
        for (int i = 0; i <= 5; ++i)
            assert(cb0.is_nearly_full(i));
        for (int i = 1; i < 0; ++i)
            assert(!cb0.is_nearly_full(i));

        cb0 += 66;
        assert(cb0.get_latest() == 66);
        assert(cb0.get_oldest() == 22);
        assert(cb0.index == 1);
        assert(cb0.count == 5);
        assert(cb0[-5] == 22);
        assert(cb0[-4] == 33);
        assert(cb0[-3] == 44);
        assert(cb0[-2] == 55);
        assert(cb0[-1] == 66);
        assert(cb0[0] == 22);
        assert(cb0[1] == 33);
        assert(cb0[2] == 44);
        assert(cb0[3] == 55);
        assert(cb0[4] == 66);
        assert(cb0.buffer[0] == 66);
        assert(cb0.buffer[1] == 22);
        assert(cb0.buffer[2] == 33);
        assert(cb0.buffer[3] == 44);
        assert(cb0.buffer[4] == 55);

        cb0.store(77).store(88);
        assert(cb0.get_latest() == 88);
        assert(cb0.get_oldest() == 44);
        assert(cb0.index == 3);
        assert(cb0.count == 5);
        assert(cb0[-5] == 44);
        assert(cb0[-4] == 55);
        assert(cb0[-3] == 66);
        assert(cb0[-2] == 77);
        assert(cb0[-1] == 88);
        assert(cb0[0] == 44);
        assert(cb0[1] == 55);
        assert(cb0[2] == 66);
        assert(cb0[3] == 77);
        assert(cb0[4] == 88);
        assert(cb0.buffer[0] == 66);
        assert(cb0.buffer[1] == 77);
        assert(cb0.buffer[2] == 88);
        assert(cb0.buffer[3] == 44);
        assert(cb0.buffer[4] == 55);

        std::cout << "   All tests OK\n\n";
    }
}