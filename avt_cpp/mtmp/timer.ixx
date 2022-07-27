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

#include <chrono>
//#include <iostream>
#include <thread>


export module mtmp.timer;

import mtmp.thread;


//===========================================================================
export namespace mtmp
{
    //=======================================================================
    /** @brief The base class for Timers.
    *
    * Timers are threads that repeat a same task on a periodical time and 
    * for a specified overall duration.
    * 
    * @sa mtmp::Thread.
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
    *  Notice:  a stopped timer SHOULD NOT be started  again.  Attempting
    *     to do so leads to the throwing of a dedicated exception.
    */
    class Timer : public mtmp::Thread
    {
    public:
        //---   Constructors / Destructor   ---------------------------------
        /** @brief Constructor.
        *
        * On call of '.start()', infinitely repeats the task described in
        * method '.run()' unless '.stop()' is called.
        * Set b_delay to true to get a waiting period before running  the
        * repetead  task,  or set it to false to get this running as soon
        * as '.start()' is called.
        */
        inline Timer(const double period_ms,
                     const bool   b_delay = false) noexcept
            : mtmp::Thread{},
              m_period_ms{ std::chrono::milliseconds(llround(period_ms)) },
              m_n_repeats{ 0 },
              m_b_delay{ b_delay }
        {}

        /** @brief Constructor.
        *
        * On call of '.start()',  repeats the task described  in  method 
        * '.run()' at most n times or unless '.stop()' is called.
        * Set b_delay to true to get a waiting period before running the
        * repetead  task, or set it to false to get this running as soon
        * as '.start()' is called.
        * N.B. 'n_repeats' set to 0 means infinite repeating.  The timer 
        * must then be explicitely stopped to complete.
        */
        inline Timer(const double period_ms,
                     const size_t n_repeats,
                     const bool   b_delay = false) noexcept
            : mtmp::Thread{},
              m_period_ms{ std::chrono::milliseconds(llround(period_ms)) },
              m_n_repeats{ n_repeats },
              m_b_delay{ b_delay }
        {}


        /** @brief Default Copy constructor. */
        Timer(const Timer&) = delete;

        /** @brief Default Move constructor. */
        Timer(Timer&&) noexcept = default;

        /** @brief Destructor. */
        virtual inline ~Timer() noexcept
        {
            //std::cout << "\n- " << this << " - in mtmp::Timer::delete()  (" << std::chrono::system_clock::now() << ")\n";
        }


        //---   Assignments   -----------------------------------------------
        /** @brief Default Copy assignment. */
        Timer& operator=(const Timer&) = default;

        /** @brief Default Move assignment. */
        Timer& operator=(Timer&&) = default;


    protected:
        //---   Core processing method   ------------------------------------
        /** @brief The core of this thread processing.
        *
        * This methods does nothing in this generic class. Inheriting
        * classes  can  override  it  to implement their own repeated
        * processing.
        * @sa as an example class mtmp::Watchdog.
        */
        virtual inline void run() override
        {
            //std::cout << "\n- " << this << "in mtmp::Timer::run()  (" << std::chrono::system_clock::now() << ")\n";
        }


        //---   Internal processing stuff   ---------------------------------
        /** @brief The internal running method.
        *
        * Launches the protected method '.run()' which must be implemented
        * in inheriting classes.
        */
        virtual void _run() noexcept
        {
            _prepare_run();

            size_t count{ 1 };
            std::chrono::system_clock::time_point start_time = std::chrono::system_clock::now();
            
            if (m_b_delay) {
                std::this_thread::sleep_until(start_time + m_period_ms);
                count++;
            }

            while (this->is_running() && (m_n_repeats == 0 || count <= m_n_repeats)) {
                run();
                std::this_thread::sleep_until(start_time + count * m_period_ms);
                count++;
            }

            _terminate_run();
        }

        //---   Attributes   ------------------------------------------------
        std::chrono::milliseconds m_period_ms;
        size_t m_n_repeats;
        bool   m_b_delay;
    };

}
