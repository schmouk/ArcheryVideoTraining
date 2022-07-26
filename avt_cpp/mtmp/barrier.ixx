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

#include <atomic>
#include <chrono>
#include <exception>
#include <iostream>
#include <thread>
#include <vector>

export module mtmp.barrier;

import mtmp.guarded_block;
import mtmp.mutex;
import mtmp.signal;
import mtmp.thread;


//===========================================================================
export namespace mtmp
{
    //=======================================================================
    /** @brief The class for Barriers.
    *
    * Next text is an extract from Python documentation. @see
    * https://docs.python.org/3/library/threading.html#barrier-objects.
    * 
    * Barriers are a simple synchronization primitive for use by a  fixed 
    * number  of  threads  that need to wait for each other.  Each of the 
    * threads tries to pass the barrier by calling the wait() method  and 
    * will  bloc k until all of the threads have made their wait() calls. 
    * At this point,  the threads are to be considered as being  released 
    * simultaneously.
    *
    * The barrier can be reused any number of times for the same number 
    * of threads.
    */
    class Barrier
    {
    public:

        /**
        class Barrier :
            def __init__ ( self , n ):
                self . n = n
                self . count = 0
                self . mutex = Semaphore (1)
                self . turnstile = Semaphore (0)
                self . turnstile2 = Semaphore (0)

            def phase1 ( self ):
                self . mutex . wait ()
                self . count += 1
                if self . count == self . n :
                self . turnstile . signal ( self . n )
                self . mutex . signal ()
                self . turnstile . wait ()

             def phase2 ( self ):
                self . mutex . wait ()
                self . count -= 1
                if self . count == 0:
                self . turnstile2 . signal ( self . n )
                self . mutex . signal ()
                self . turnstile2 . wait ()

             def wait ( self ):
                self . phase1 ()
                self . phase2 ()
        **/

        //---   Constructors / Destructor   ---------------------------------
        /** @brief Constructor. */
        inline Barrier(const unsigned long synchronizing_threads_count) noexcept(false)
            : m_sync_threads_count{ synchronizing_threads_count },
              m_waiting_threads_count{ 0 },
              m_guard_mtx{},
              m_turnstile_1{ false },
              m_turnstile_2{ true }
        {
            if (synchronizing_threads_count < 1)
                throw CreationValueException();
        }

        /** @brief Default Copy constructor. */
        Barrier(const Barrier&) noexcept = delete;

        /** @brief Default Move constructor. */
        Barrier(Barrier&&) noexcept = delete;

        /** @brief Destructor. */
        virtual inline ~Barrier() noexcept = default;


        //---   Assignments   -----------------------------------------------
        /** @brief Default Copy assignment. */
        Barrier& operator=(const Barrier&) noexcept = delete;

        /** @brief Default Move assignment. */
        Barrier& operator=(Barrier&&) noexcept = delete;


        //---   Status checking   -------------------------------------------
        /** @brief Returns the number of threads to be synchronized with this barrier. */
        inline const unsigned long get_synchronized_threads() noexcept
        {
            mtmp::GuardedBlock guard(&m_guard_mtx);
            return m_sync_threads_count;
        }

        /** @brief Returns the count of threads that are currently waiting for this barrier. */
        inline const unsigned long get_waiting_threads_count() noexcept
        {
            mtmp::GuardedBlock guard(&m_guard_mtx);
            return m_waiting_threads_count;
        }


        //---   Barriers operations   ---------------------------------------
        /** @brief Makes all threads to wait on this barrier then run altogether. */
        void wait() noexcept(false)
        {
            // First synchronizing step - on turnstile 1
            {
                mtmp::GuardedBlock guard{ &m_guard_mtx };
                m_waiting_threads_count++;
                if (m_waiting_threads_count == m_sync_threads_count) {
                    m_turnstile_2.wait();
                    m_turnstile_1.emit();
                }
            }
            m_turnstile_1.wait();
            m_turnstile_1.emit();

            // Second synchronizing step - on turnstile 2
            {
                mtmp::GuardedBlock guard{ &m_guard_mtx };
                m_waiting_threads_count--;
                if (m_waiting_threads_count == 0) {
                    m_turnstile_1.wait();
                    m_turnstile_2.emit();
                }
            }
            m_turnstile_2.wait();
            m_turnstile_2.emit();
        }


        //---   Specific Exceptions   ---------------------------------------
        /** @brief Exception on erroneous instantiation of this class. */
        class CreationValueException : public std::exception
        {
            const char* what() const noexcept { return "!!! Error: the synchronizing-threads count must be greater than 0\n"; }
        };

        /** @brief Exception on erroneous creation of internal synchronizing primitive. */
        class SynchronizationException : public std::exception
        {
            const char* what() const noexcept { return "!!! Error: cannot internally instantiate a new synchronization primitive.\n"; }
        };


    private:
        //---   Attributes   ------------------------------------------------
        unsigned long   m_sync_threads_count;
        unsigned long   m_waiting_threads_count;
        mtmp::Mutex     m_guard_mtx;
        mtmp::Signal    m_turnstile_1;
        mtmp::Signal    m_turnstile_2;
    };

}
