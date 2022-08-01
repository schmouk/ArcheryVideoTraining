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
#include <chrono>
#include <iostream>
#include <thread>

export module unit_tests.mtmp.test_timers;

import avt.mtmp.timer;



//===========================================================================
namespace avt::unit_tests::mtmp::timer
{
    //=======================================================================
    class TimerA : public avt::mtmp::Timer
    {
    public:
        inline TimerA(const char   name,
                      const double period_ms,
                      const bool   b_delay = false) noexcept
            : avt::mtmp::Timer{ period_ms, b_delay }, m_name{ name }
        {}

        inline TimerA(const char   name,
                      const double period_ms,
                      const size_t n_repeats,
                      const bool   b_delay = false) noexcept
            : avt::mtmp::Timer{ period_ms, n_repeats, b_delay }, m_name{ name }
        {}

    protected:
        //---   Core processing method   ------------------------------------
        virtual void run() override
        {
            std::cout << m_name << '-' << std::chrono::system_clock::now() << '\n';
        }

    private:
        char m_name;
    };

    //=======================================================================
    export void test_timers()
    {
        std::cout << "-- TEST avt::mtmp::Timer\n";

        TimerA a{ 'A', 311, true };
        TimerA b{ 'B', 463.49, 20, false };

        a.start();
        b.start();

        std::cout << std::chrono::system_clock::now() << '\n';

        std::cout << "main thread is waiting for joins\n";
        std::this_thread::sleep_for(std::chrono::seconds(5));
        a.stop();
        a.join();
        b.join();

        assert(avt::mtmp::Thread::get_running_threads_count() == 0);

        std::cout << "   All tests OK\n\n";
    }
}