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
//#include <iostream>
#include <chrono>
#include <thread>


export module mtmp.watchdog;

import mtmp.thread;
import mtmp.timer;


//===========================================================================
export namespace mtmp
{
    export class Watchdog;

    //=======================================================================
    /** @brief The base class for Watch Dogs.
    *
    * Watchdogs  are timers which act only once,  when their timeout period
    * of time has expired.  Meanwhile, they can be reset at any time.  This
    * leads  to  the running again of their timing count-down before waking 
    * up.
    *
    * @sa mtmp::Timer.
    *
    * Correct usage is:
    *   - Define a new class inheriting from this one;
    *   - Implement the abstract method run() in your new class;
    *   - Instantiate your new class at will;
    *   - To start the processing of the related watchdog, apply '.start()'
    *     on this newly created instance.
    *   - Call '.reset()' from times to time, for this watchdog not to wake
    *     up before a new period of time.
    *   - If the watchdog wakes up (i.e. no  call  to  '.reset()'  happened
    *     before  the  time  count-down completes), method '.run()' will be 
    *     run.
    *   - Meanwhile,  method '.stop()'  may  be  called  from  outside  the
    *     running thread.  This  will  set data member 'm_is_running' which
    *     can be tested via method '.is_running()' within  your  implement-
    *     ation of protected method '.run()' to stop its processing.
    */
    class Watchdog
    {
    public:
        //---   Constructors / Destructor   ---------------------------------
        /** @brief Constructor. */
        inline Watchdog(const double time_countdown_ms, const char name) noexcept
            : mp_timer{ nullptr },
              m_time_countdown_ms{ time_countdown_ms },
              m_name{ name }
        {}

        /** @brief Default Copy constructor. */
        Watchdog(const Watchdog&) = delete;

        /** @brief Default Move constructor. */
        Watchdog(Watchdog&&) noexcept = default;

        /** @brief Destructor. */
        virtual inline ~Watchdog() noexcept
        {}


        //---   Assignments   -----------------------------------------------
        /** @brief Default Copy assignment. */
        Watchdog& operator=(const Watchdog&) = default;

        /** @brief Default Move assignment. */
        Watchdog& operator=(Watchdog&&) = default;


        //---   Watchdog operations   ---------------------------------------
        /** @brief Resets the time count-down for this watchdog. */
        inline void reset() noexcept(false)
        {
            //std::cout << "- " << mp_timer << " - in " << m_name << " mtmp::Watchdog::reset() (" << std::chrono::system_clock::now() << ")\n";
            if (mp_timer != nullptr) {
                mp_timer->b_reset.store(true);
            }
                
            start();
        }

        /** @brief Starts this watchdog. */
        virtual void start() noexcept(false)
        {
            //std::cout << "- " << mp_timer << " - in " << m_name << " mtmp::Watchdog::start() (" << std::chrono::system_clock::now() << ")\n";

            if (mp_timer != nullptr)
                stop();

            mp_timer = new _Timer(this, (const double)m_time_countdown_ms);

            if (mp_timer == nullptr)
                throw mtmp::Watchdog::StartException();
            else {
                //std::cout << "- " << mp_timer << " - " << m_name << " launches mp_timer          (" << std::chrono::system_clock::now() << ")\n";
                mp_timer->start();
            }
        }

        /** @brief Stops this watchdog. */
        void stop() noexcept
        {
            if (mp_timer != nullptr) {
                //std::cout << "- " << mp_timer << " - in " << m_name << " mtmp::Watchdog::stop()  (" << std::chrono::system_clock::now() << ")\n";
                mp_timer->stop();
                mp_timer->join();
                delete mp_timer;
                mp_timer = nullptr;
            }
        }

        /** @brief Modifies the time down-count for this watchdog. */
        void set_time_countdown_ms(const double time_countdown_ms) noexcept(false)
        {
            if (time_countdown_ms < 0.5)
                throw mtmp::Watchdog::TimeCountdownException();
            m_time_countdown_ms = time_countdown_ms;
            reset();
        }


        //---   Specific Exceptions   ---------------------------------------
        /** @brief Exception on erroneous instantiation of this class. */
        class StartException : public std::exception
        {
            const char* what() const noexcept { return "!!! Error: Unable to start a new instance of class avt::mtmp::Watchdog\n"; }
        };

        /** @brief Exception on erroneous time count down. */
        class TimeCountdownException : public std::exception
        {
            const char* what() const noexcept { return "!!! Error: Time down count must be greater than 0 ms in class avt::mtmp::Watchdog\n"; }
        };


    protected:
        //---   Core processing method   ------------------------------------
        /** @brief The core of this watchdog processing.
        *
        * This is the code that should be run when the time count-
        * down  completes.  Inheriting classes should override it.
        */
        virtual inline void run()
        {
            //std::cout << "- " << mp_timer << " - in " << m_name << " mtmp::Watchdog.run()\n";
        }


    private:
        //---   Internal implementation   -----------------------------------
        class _Timer : public mtmp::Timer {
        public:
            inline _Timer(Watchdog* parent_watchdog,
                          const double time_countdown_ms) noexcept
                : mp_parent{ parent_watchdog },
                  mtmp::Timer(time_countdown_ms, 1, true)  //false)  // 
            {
                b_reset.store(false);
            }

            std::atomic_bool b_reset;

        protected:
            virtual void _run() noexcept
            {
                std::chrono::system_clock::time_point start_time = std::chrono::system_clock::now();
                std::this_thread::sleep_until(start_time + m_period_ms);
                _prepare_run();
                run();
                _terminate_run();
            }

            virtual inline void run() override
            {
                //std::this_thread::sleep_for(std::chrono::milliseconds(5));
                //std::cout << "\n- " << this << " - in " << mp_parent->m_name << " mtmp::Watchdog::_Timer.run()  (" << std::chrono::system_clock::now() << ")\n";
                if (!b_reset.load()) {
                    mp_parent->run();
                }
                else {
                    //std::cout << "\n- " << this << " - in " << mp_parent->m_name << " mtmp::Watchdog::_Timer.run()  (resetted)\n";
                }
            }

            mtmp::Watchdog*  mp_parent;
        };


        //---   Attributes   ------------------------------------------------
        _Timer* mp_timer;
        double  m_time_countdown_ms;
        char    m_name;
    };

}
