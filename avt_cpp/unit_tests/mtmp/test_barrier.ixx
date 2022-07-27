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
#include <mutex>
#include <thread>
#include <vector>

export module unit_tests.mtmp.test_barrier;

import mtmp.barrier;
import mtmp.guarded_block;
import mtmp.mutex;
import mtmp.thread;


//===========================================================================
namespace mtmp::unit_tests::test_barrier
{
    //=======================================================================
    class ThreadA : public mtmp::Thread
    {
    public:
        inline ThreadA(const char name, mtmp::Barrier* p_barrier, const double waiting_s, mtmp::Mutex* print_mutex) noexcept
            : mtmp::Thread{}, m_waiting_s{ waiting_s }, mp_barrier{ p_barrier }, m_name{ name }, mp_prt_mtx{ print_mutex }
        {}

        inline virtual ~ThreadA() noexcept
        {}

    protected:
        //---   Core processing method   ------------------------------------
        virtual void run() override
        {
            {
                mtmp::GuardedBlock guard{ mp_prt_mtx };
                std::cout << std::this_thread::get_id() << "- " << m_name 
                          << " in run(), sleeps " << m_waiting_s << "s             (" 
                          << std::chrono::system_clock::now() << ")\n";
            }
            
            mtmp::Thread::sleep_s(m_waiting_s);
            
            {
                mtmp::GuardedBlock guard{ mp_prt_mtx };
                std::cout << std::this_thread::get_id() << "- " << m_name 
                          << " waits for barrier synchronization (" << std::chrono::system_clock::now() << ")\n";
            }

            mp_barrier->wait();

            {
                mtmp::GuardedBlock guard{ mp_prt_mtx };
                std::cout << std::this_thread::get_id() << "- " << m_name 
                          << " synchronization OK                (" << std::chrono::system_clock::now() << ")\n";
            }
        }

    private:
        double          m_waiting_s;
        mtmp::Barrier*  mp_barrier;
        mtmp::Mutex*    mp_prt_mtx;
        char            m_name;
    };
    //=======================================================================
    class ThreadB : public mtmp::Thread
    {
    public:
        inline ThreadB(const char name, mtmp::Barrier* p_barrier, const double waiting_s, mtmp::Mutex* print_mutex) noexcept
            : mtmp::Thread{}, m_waiting_s{ waiting_s }, mp_barrier{ p_barrier }, m_name{ name }, mp_prt_mtx{ print_mutex }
        {}

        inline virtual ~ThreadB() noexcept
        {}

    protected:
        //---   Core processing method   ------------------------------------
        virtual void run() override
        {
            {
                mtmp::GuardedBlock guard{ mp_prt_mtx };
                std::cout << std::this_thread::get_id() << "- " << m_name
                    << " in run(), sleeps " << m_waiting_s << "s             ("
                    << std::chrono::system_clock::now() << ")\n";
            }

            mtmp::Thread::sleep_s(m_waiting_s);

            {
                mtmp::GuardedBlock guard{ mp_prt_mtx };
                std::cout << std::this_thread::get_id() << "- " << m_name
                    << " waits for barrier synchronization (" << std::chrono::system_clock::now() << ")\n";
            }

            if (!mp_barrier->wait_ms(1)) {
                mtmp::GuardedBlock guard{ mp_prt_mtx };
                std::cout << std::this_thread::get_id() << "- " << m_name
                    << " synchronization timed out!        (" << std::chrono::system_clock::now() << ")\n";
            }
            else {
                mtmp::GuardedBlock guard{ mp_prt_mtx };
                std::cout << std::this_thread::get_id() << "- " << m_name
                    << " synchronization OK                (" << std::chrono::system_clock::now() << ")\n";
            }
        }

    private:
        double          m_waiting_s;
        mtmp::Barrier* mp_barrier;
        mtmp::Mutex* mp_prt_mtx;
        char            m_name;
    };


    //=======================================================================
    export void test_barrier()
    {
        std::cout << "-- TEST mtmp::Barrier\n";

        mtmp::Barrier the_barrier(5);
        mtmp::Mutex print_mutex;

        mtmp::unit_tests::test_barrier::ThreadA v('V', &the_barrier, 1.1, &print_mutex);
        mtmp::unit_tests::test_barrier::ThreadA w('W', &the_barrier, 1.3, &print_mutex);
        mtmp::unit_tests::test_barrier::ThreadA x('X', &the_barrier, 1.5, &print_mutex);
        mtmp::unit_tests::test_barrier::ThreadA y('Y', &the_barrier, 1.7, &print_mutex);
        mtmp::unit_tests::test_barrier::ThreadA z('Z', &the_barrier, 1.9, &print_mutex);
                          
        std::vector<ThreadA*> threads{};
        threads.push_back(&v);
        threads.push_back(&w);
        threads.push_back(&x);
        threads.push_back(&y);
        threads.push_back(&z);


        for (auto* t : threads)
            t->start();

        for (auto* t : threads)
            t->join();


        std::cout << std::endl;


        mtmp::unit_tests::test_barrier::ThreadB a('A', &the_barrier, 1.1, &print_mutex);
        mtmp::unit_tests::test_barrier::ThreadB b('B', &the_barrier, 1.3, &print_mutex);
        mtmp::unit_tests::test_barrier::ThreadB c('C', &the_barrier, 1.5, &print_mutex);
        mtmp::unit_tests::test_barrier::ThreadB d('D', &the_barrier, 1.7, &print_mutex);
        mtmp::unit_tests::test_barrier::ThreadB e('E', &the_barrier, 1.9, &print_mutex);

        std::vector<ThreadB*> threadsB{};
        threadsB.push_back(&a);
        threadsB.push_back(&b);
        threadsB.push_back(&c);
        threadsB.push_back(&d);
        threadsB.push_back(&e);


        for (auto* t : threadsB)
            t->start();

        for (auto* t : threadsB)
            t->join();


        std::cout << "   All tests OK\n\n";
    }
}