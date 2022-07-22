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

export module unit_tests.mtmp.test_mutex;

import mtmp.mutex;
import mtmp.thread;


//===========================================================================
namespace mtmp::unit_tests
{
    //=======================================================================
    class ThreadA : public mtmp::Thread
    {
    public:
        inline ThreadA(mtmp::Mutex* p_mutex) noexcept
            : mtmp::Thread(), mp_mutex(p_mutex)
        {}

        inline virtual ~ThreadA() noexcept
        {}

    protected:
        //---   Core processing method   ------------------------------------
        virtual void run() override
        {
            for (int i = 0; i < 20 && is_running(); ++i) {
                std::cout << "A - unlocks the mutex\n";
                mp_mutex->unlock();
                sleep_s(0.5);
            }
        }

    private:
        mtmp::Mutex* mp_mutex;
    };

    //=======================================================================
    class ThreadB : public mtmp::Thread
    {
    public:
        inline ThreadB(const char id, mtmp::Mutex* p_mutex) noexcept
            : mtmp::Thread(), m_id(id), mp_mutex(p_mutex)
        {}

        inline virtual ~ThreadB() noexcept
        {}

    protected:
        //---   Core processing method   ------------------------------------
        virtual void run()  override
        {
            for (int i = 20; i > 0; --i) {
                const bool ok = (i % 2) ? mp_mutex->lock_ms(400) : mp_mutex->lock_s(0.2);
                std::cout << "B-" << m_id << ": " << ok << '/' << i - 1 << '\n';
                sleep_s(0.1);
            }
        }

    private:
        mtmp::Mutex* mp_mutex;
        char m_id;

    };

    //=======================================================================
    export void test_mutex()
    {
        std::cout << "-- TEST mtmp::Mutex\n";

        mtmp::Mutex the_mutex;
        mtmp::unit_tests::ThreadA a(&the_mutex);
        mtmp::unit_tests::ThreadB b1('1', &the_mutex);
        mtmp::unit_tests::ThreadB b2('2', &the_mutex);

        a.start();
        b1.start();
        b2.start();

        a.join();
        b1.join();
        b2.join();


        std::cout << "   All tests OK\n\n";
    }
}