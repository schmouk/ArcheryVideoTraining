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
#include <exception>
#include <cstring>
#include <thread>

#include <windows.h>
#include <processthreadsapi.h>


export module mtmp.thread;


//===========================================================================
export namespace avt::mtmp
{
    //=======================================================================
    /** @brief The base class for Threads.
    *
    * Uses the STL std::thread class (per composition) and offers goodies
    * for the launching of threads at will rather than at  instantiation.
    *
    * Correct usage is:
    *   - Define a new class inheriting from this one;
    *   - Implement the abstract method run() in your new class;
    *   - Instantiate your new class at will;
    *   - To start the processing of the related thread, apply '.start()'
    *     on this newly created instance.  This will internally call  the
    *     '.run()' method as implemented in your new class.
    *   - call '.join()' on this same instance to be awakened as soon  as
    *     the running method of the thread will have completed.
    *   - Meanwhile, method '.stop()' may  be  called  from  outside  the
    *     running thread.  This will set data member 'm_is_running' which
    *     can be tested via method 'is_running()' within your  implement-
    *     ation of protected method '.run()' to stop its processing.
    *
    *  Notice : a stopped thread SHOULD NOT be started again.  Attempting
    *     to do so leads to the throwing of a dedicated exception.
    */
    class Thread
    {
    public:
        //---   Class Data   ------------------------------------------------
        static constexpr long PRI_IDLE         = THREAD_PRIORITY_IDLE;          // win32 const
        static constexpr long PRI_LOWEST       = THREAD_PRIORITY_LOWEST;        // win32 const
        static constexpr long PRI_BELOW_NORMAL = THREAD_PRIORITY_BELOW_NORMAL;  // win32 const
        static constexpr long PRI_NORMAL       = THREAD_PRIORITY_NORMAL;        // win32 const
        static constexpr long PRI_ABOVE_NORMAL = THREAD_PRIORITY_ABOVE_NORMAL;  // win32 const
        static constexpr long PRI_HIGHEST      = THREAD_PRIORITY_HIGHEST;       // win32 const
        static constexpr long PRI_CRITICAL     = THREAD_PRIORITY_TIME_CRITICAL;  // win32 const


        //---   Constructors / Destructor   ---------------------------------
        /** @brief Default constructor. */
        inline Thread() noexcept
            : m_name{}, m_priority{ PRI_NORMAL }
        {}

        /** @brief Named constructor + optional priority setting. */
        inline Thread(const std::string& name, const long priority = PRI_NORMAL)
            : m_name{ name }, m_priority{ priority }
        {}

        /** @brief Default Copy constructor. */
        Thread(const Thread&) = delete;

        /** @brief Default Move constructor. */
        Thread(Thread&&) noexcept = default;

        /** @brief Destructor. */
        virtual ~Thread() noexcept;


        //---   Assignments   -----------------------------------------------
        /** @brief Default Copy assignment. */
        Thread& operator=(const Thread&) = default;

        /** @brief Default Move assignment. */
        Thread& operator=(Thread&&) = default;


        //---   Status checking   -------------------------------------------
        /** @brief Returns the ID of this thread.
        *
        * Thread IDs are set by the STL class 'std::thread::id'.  Should this
        * thread not having been started, the returned ID will be the one set
        * for all the "unjoinable threads".
        */
        inline const std::thread::id get_id() const noexcept
        {
            if (is_ok())
                return mp_thread->get_id();
            else
                return std::thread::id();
        }

        /** @brief Returns true if this thread has been successfully started, or false otherwise. */
        inline const bool is_ok() const noexcept
        {
            return mp_thread != nullptr;
        }

        /** @brief Returns true if this thread is currently running, or false otherwise. */
        inline const bool is_running() const noexcept
        {
            return is_ok() && m_is_running.load();
        }


        //---   Threads operations   ----------------------------------------
        /** @brief Joins this thread, waiting for the completion of its '.run()' method. */
        inline void join() noexcept
        {
            if (is_ok())
            {
                mp_thread->join();
                stop();
            }
        }

        /** @brief Sets the background mode for this thread (to false or true).
        *
        * Returns true if setting was ok, or false otherwise.
        */
        inline bool set_background_mode(const bool b_to_background = true)
        {
            if (is_ok()) [[likely]] {
                return SetThreadPriority(mp_thread->native_handle(),
                                         b_to_background ? THREAD_MODE_BACKGROUND_BEGIN : THREAD_MODE_BACKGROUND_END);
            }
            else [[unlikely]] {
                return false;
            }
        }

        /** @brief Sets the priority level for this running thread.
        *
        * Returns true if priority level setting was ok, or false otherwise.
        */
        bool set_priority(const long priority);

        /** @brief Puts this thread to sleep for a fractional count of seconds. */
        inline void sleep_s(const double duration_seconds) noexcept
        {
            sleep_ms(duration_seconds * 1e3);  // std::this_thread::sleep_for(std::chrono::microseconds(llround(duration_seconds * 1e6)));
        }

        /** @brief Puts this thread to sleep for a fractional count of milliseconds. */
        inline void sleep_ms(const double duration_ms) noexcept
        {
            std::this_thread::sleep_for(std::chrono::microseconds(llround(duration_ms * 1e3)));
        }

        /** @brief Starts (at will) the processing of this thread. */
        virtual void start() noexcept(false);

        /** @brief Asks for the stopping of the processing of this thread.
        *
        * The actions related to the stopping of this thread processings
        * should  take  place in the protected method 'run()' which must
        * be implemented in inheriting classes.
        */
        void stop() noexcept;


        //---   Threads counting   ------------------------------------------
        /** @brief Returns the number of currently active threads. */
        static inline const long get_running_threads_count() noexcept
        {
            return ms_active_threads_count.load();
        }


        //---   Specific Exceptions   ---------------------------------------
        /** @brief Exception on erroneous instantiation of this class. */
        class CreationException : public std::exception
        {
            const char* what() const noexcept { return "!!! Error: Unable to create a new instance of class avt::mtmp::Thread\n"; }
        };

        /** @brief Exception thrown when attempting to start again a thread. */
        class StartedException : public std::exception
        {
            const char* what() const noexcept { return "!!! Error: Unable to create a new instance of class avt::mtmp::Thread\n"; }
        };


    protected:
        //---   Core processing method   ------------------------------------
        /** @brief The core of this thread processing.
        *
        * This methods is abstract in the base class.  It must be implemented
        * in inheriting classes. Typical implementation is a loop, guarded by
        * the  'is_running()' status of this thread on each or nearly each of
        * its loops.
        */
        virtual void run() = 0;


        //--- Internal processing stuff   -----------------------------------
        /** @brief The internal inits to be done before calling '.run()'. */
        void _prepare_run() noexcept
        {
            m_is_running.store(true);
            m_already_started.store(true);
            ms_active_threads_count++;
        }

        /** @brief The internal running method.
        *
        * Launches the protected method '.run()' which must be implemented
        * in inheriting classes.
        */
        virtual inline void _run() noexcept
        {
            _prepare_run();
            run();
            _terminate_run();
        }

        /** @brief The internal termination to be done when '.run()' has completed. */
        virtual inline void _terminate_run()
        {}


        //---   Base class attributes   -------------------------------------
        static std::atomic_long ms_active_threads_count;

        std::atomic_bool m_already_started{ false };
        std::atomic_bool m_is_running{ false };
        std::thread* mp_thread{ nullptr };
        std::string m_name;
        long m_priority;
    };

    std::atomic_long Thread::ms_active_threads_count{ 0 };

}
