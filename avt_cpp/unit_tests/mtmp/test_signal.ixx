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

export module unit_tests.mtmp.test_signal;

import avt.mtmp.signal;
import avt.mtmp.thread;


//===========================================================================
namespace avt::unit_tests::mtmp::signal
{
    //=======================================================================
    class ThreadA : public avt::mtmp::Thread
    {
    public:
        inline ThreadA(avt::mtmp::Signal* p_signal) noexcept
            : avt::mtmp::Thread(), mp_signal(p_signal)
        {
            std::cout << "instantiates Thread A\n";
        }

        inline virtual ~ThreadA() noexcept
        {}

    protected:
        //---   Core processing method   ------------------------------------
        virtual void run() override
        {
            std::cout << "Runs Thread A\n";
            for (int i = 0; i < 10 && is_running(); ++i) {
                sleep_s(0.5);
                std::cout << "A - emits signal\n";
                mp_signal->emit();
            }
        }

    private:
        avt::mtmp::Signal* mp_signal;
    };

    //=======================================================================
    class ThreadB : public avt::mtmp::Thread
    {
    public:
        inline ThreadB(const char id, avt::mtmp::Signal* p_signal) noexcept
            : avt::mtmp::Thread(), m_id(id), mp_signal(p_signal)
        {
            std::cout << "instantiates Thread B(" << m_id << ")\n";
        }

        inline virtual ~ThreadB() noexcept
        {}

    protected:
        //---   Core processing method   ------------------------------------
        virtual void run()  override
        {
            std::cout << "Runs Thread B(" << m_id << ")\n";
            for (int i = 20; i > 0; --i) {
                const bool ok = (i % 2) ? mp_signal->wait_ms(400) : mp_signal->wait_s(0.2);
                std::cout << "B-" << m_id << ": " << ok << '/' << i - 1 << '\n';
            }
        }

    private:
        avt::mtmp::Signal* mp_signal;
        char m_id;

    };

    //=======================================================================
    export void test_signal()
    {
        std::cout << "-- TEST avt::mtmp::Signal\n";

        avt::mtmp::Signal the_signal;
        ThreadA a(&the_signal);
        ThreadB b1('1', &the_signal);
        ThreadB b2('2', &the_signal);

        b1.start();
        b2.start();
        a.start();

        a.join();
        b1.join();
        b2.join();


        std::cout << "   All tests OK\n\n";
    }
}