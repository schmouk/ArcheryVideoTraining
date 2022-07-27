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
            : mtmp::Watchdog{ duration_ms, name }, m_name{ name }
        {}

    protected:
        //---   Core processing method   ------------------------------------
        virtual void run() override
        {
            std::cout << "-->  Watchdog " << m_name << " has expired - " << std::chrono::system_clock::now() << '\n';
        }

    private:
        char m_name;
    };

    //=======================================================================
    export void test_watchdog()
    {
        std::cout << "-- TEST mtmp::Watchdog\n";

        mtmp::unit_tests::MyWatchdog a{ 'X', 1311 };
        mtmp::unit_tests::MyWatchdog b{ 'Y', 1763.49 };

        std::cout << "  -- " << std::chrono::system_clock::now() << "\n\n";

        a.start();
        b.start();
        std::cout << "A -- " << std::chrono::system_clock::now() << "\n\n";
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));

        a.reset();
        b.reset();
        std::cout << "B -- " << std::chrono::system_clock::now() << "\n\n";
        std::this_thread::sleep_for(std::chrono::milliseconds(1500));

        a.reset();
        b.reset();
        std::cout << "C -- " << std::chrono::system_clock::now() << "\n\n";
        std::this_thread::sleep_for(std::chrono::milliseconds(2000));

        a.stop();
        b.stop();
        std::cout << "D -- " << std::chrono::system_clock::now() << "\n\n";

        std::cout << "   All tests OK\n\n";
    }
}