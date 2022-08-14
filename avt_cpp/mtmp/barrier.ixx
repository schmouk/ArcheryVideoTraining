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
export namespace avt::mtmp
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
        //---   Constructors / Destructor   ---------------------------------
        /** @brief Constructor. */
        Barrier(const unsigned long synchronizing_threads_count) noexcept(false);

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
            avt::mtmp::GuardedBlock guard(&m_guard_mtx);
            return m_sync_threads_count;
        }

        /** @brief Returns the count of threads that are currently waiting for this barrier. */
        inline const unsigned long get_waiting_threads_count() noexcept
        {
            avt::mtmp::GuardedBlock guard(&m_guard_mtx);
            return m_waiting_threads_count;
        }


        //---   Barriers operations   ---------------------------------------
        /** @brief Makes all threads to wait on this barrier then run altogether. */
        void wait() noexcept(false);

        /** @brief Timed out waiting for this barrier (seconds).
        *
        * Returns true if synchronization happened before timeout, or
        * false otherwise.
        * May throw std::system_error or a timeout-related exception.
        */
        inline bool wait_s(const double timeout_s) noexcept(false)
        {
            return wait_ms(timeout_s * 1e3);
        }

        /** @brief Timed out waiting for this signal (milliseconds).
        *
        * Returns true if synchronization happened before timeout, or
        * false otherwise.
        * May throw std::system_error or a timeout-related exception.
        */
        bool wait_ms(const double timeout_ms) noexcept(false);


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
        unsigned long       m_sync_threads_count;
        unsigned long       m_waiting_threads_count;
        avt::mtmp::Mutex    m_guard_mtx;
        avt::mtmp::Signal   m_turnstile_1;
        avt::mtmp::Signal   m_turnstile_2;
    };

}
