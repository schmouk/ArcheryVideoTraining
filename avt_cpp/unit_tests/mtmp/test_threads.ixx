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

#include <chrono>         // std::chrono::milliseconds
#include <iostream>
#include <thread>         // std::this_thread::sleep_for

export module unit_tests.mtmp.test_threads;

import mtmp.thread;



//===========================================================================
namespace mtmp::unit_tests
{
    //=======================================================================
    class ThreadA : public mtmp::Thread
    {
    public:
        inline ThreadA() noexcept
            : mtmp::Thread()
        {}

        inline virtual ~ThreadA() noexcept
        {}

    protected:
        //---   Core processing method   ------------------------------------
        virtual void run() override
        {
            for (int i = 0; i < 10 && is_running(); ++i) {
                sleep_s(1);
                std::cout << "A-" << i << '\n';
            }
        }
    };

    //=======================================================================
    class ThreadB : public mtmp::Thread
    {
    public:
        inline ThreadB() noexcept
            : mtmp::Thread()
        {}

        inline virtual ~ThreadB() noexcept
        {}

    protected:
        //---   Core processing method   ------------------------------------
        virtual void run()  override
        {
            for (int i = 20; i > 0; --i) {
                sleep_s(0.5);
                std::cout << "B-" << i-1 << '\n';
            }
        }

    };

    //=======================================================================
    export void test_threads()
    {
        std::cout << "-- TEST mtmp::thread\n";

        mtmp::unit_tests::ThreadA a;
        mtmp::unit_tests::ThreadB b;

        std::cout << a.get_id() << " / " << b.get_id() << std::endl;

        a.start();
        b.start();

        std::cout << a.get_id() << " / " << b.get_id() << std::endl;

        std::cout << "main thread is waiting for joins\n";
        std::this_thread::sleep_for(std::chrono::seconds(5));
        a.stop();
        a.join();
        b.join();
        std::cout << "main thread has sucessfully joined\n";

        std::cout << "   All tests OK\n\n";
    }
}