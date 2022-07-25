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

export module unit_tests.mtmp.test_watchdog;

import mtmp.watchdog;



//===========================================================================
namespace mtmp::unit_tests
{
    //=======================================================================
    class MyWatchdog : public mtmp::Watchdog
    {
    public:
        inline MyWatchdog(const char name, const double duration_ms) noexcept
            : mtmp::Watchdog{ duration_ms }, m_name{ name }
        {}

    protected:
        //---   Core processing method   ------------------------------------
        virtual void run() override
        {
            std::cout << "Watchdog " << m_name << "has expired - " << std::chrono::system_clock::now() << '\n';
        }

    private:
        char m_name;
    };

    //=======================================================================
    export void test_watchdog()
    {
        std::cout << "-- TEST mtmp::Watchdog\n";

        mtmp::unit_tests::MyWatchdog a{ 'X', 311 };
        mtmp::unit_tests::MyWatchdog b{ 'Y', 463.49 };

        a.start();
        b.start();
        //assert(mtmp::Thread::get_running_threads_count() == 2);
        std::cout << "A -- " << std::chrono::system_clock::now() << '\n';
        std::this_thread::sleep_for(std::chrono::milliseconds(1400));

        a.reset();
        b.reset();
        std::cout << "B -- " << std::chrono::system_clock::now() << '\n';
        std::this_thread::sleep_for(std::chrono::milliseconds(350));

        a.reset();
        b.reset();
        std::cout << "C -- " << std::chrono::system_clock::now() << '\n';
        std::this_thread::sleep_for(std::chrono::milliseconds(1470));

        a.stop();
        b.stop();
        std::cout << "D -- " << std::chrono::system_clock::now() << '\n';

        assert(mtmp::Thread::get_running_threads_count() == 0);

        std::cout << "   All tests OK\n\n";
    }
}