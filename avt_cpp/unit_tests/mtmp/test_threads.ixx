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
#include <chrono>         // std::chrono::milliseconds
#include <iostream>
#include <thread>         // std::this_thread::sleep_for

export module unit_tests.mtmp.test_threads;

import mtmp.thread;



//===========================================================================
namespace avt::unit_tests::mtmp::threads
{
    //=======================================================================
    class ThreadA : public avt::mtmp::Thread
    {
    public:
        inline ThreadA() noexcept
            : avt::mtmp::Thread()
        {}

        inline virtual ~ThreadA() noexcept
        {}

    protected:
        //---   Core processing method   ------------------------------------
        virtual void run() override
        {
            std::cout << "- in ThreadA::run()\n";

            for (int i = 0; i < 10 && is_running(); ++i) {
                sleep_s(1);
                std::cout << "A-" << i << '\n';
            }
        }
    };

    //=======================================================================
    class ThreadB : public avt::mtmp::Thread
    {
    public:
        inline ThreadB() noexcept
            : avt::mtmp::Thread()
        {}

        inline virtual ~ThreadB() noexcept
        {}

    protected:
        //---   Core processing method   ------------------------------------
        virtual void run()  override
        {
            std::cout << "- in ThreadB::run()\n";

            for (int i = 20; i > 0; --i) {
                sleep_s(0.5);
                std::cout << "B-" << i-1 << '\n';
            }
        }

    };

    //=======================================================================
    export void test_threads()
    {
        std::cout << "-- TEST avt::mtmp::Thread\n";

        ThreadA a;
        ThreadB b;

        std::cout << a.get_id() << " / " << b.get_id() << std::endl;

        assert(avt::mtmp::Thread::get_running_threads_count() == 0);

        a.start();
        b.start();

        std::this_thread::sleep_for(std::chrono::milliseconds(16));
        assert(avt::mtmp::Thread::get_running_threads_count() == 2);

        std::cout << a.get_id() << " / " << b.get_id() << std::endl;

        std::cout << "main thread is waiting for joins\n";
        std::this_thread::sleep_for(std::chrono::seconds(5));
        a.stop();
        a.join();

        std::this_thread::sleep_for(std::chrono::milliseconds(16));
        assert(avt::mtmp::Thread::get_running_threads_count() == 1);

        b.join();
        std::cout << "main thread has sucessfully joined\n";

        std::this_thread::sleep_for(std::chrono::milliseconds(16));
        assert(avt::mtmp::Thread::get_running_threads_count() == 0);

        std::cout << "   All tests OK\n\n";
    }
}