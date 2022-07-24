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
#include <thread>


export module mtmp.watchdog;

import mtmp.timer;


//===========================================================================
export namespace mtmp
{
    //=======================================================================
    /** @brief The base class for Watch Dogs.
    *
    * Watchdogs  are timers which act only once,  when their timeout period
    * of time has expired.  Meanwhile, they can be reset at any time.  This
    * leads  to  the running again of their timing down count before waking 
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
    *   - If the watchdog wakes up (i.e. no call to '.reset()'  before  the
    *     time count down completes), method '.run()' will be run.
    *   - Meanwhile,  method '.stop()'  may  be  called  from  outside  the
    *     running thread.  This  will  set data member 'm_is_running' which
    *     can be tested via method 'is_running()'  within  your  implement-
    *     ation of protected method '.run()' to stop its processing.
    */
    class Watchdog
    {
    public:
        //---   Constructors / Destructor   ---------------------------------
        /** @brief Constructor. */
        inline Watchdog(const double time_countdown_ms) noexcept
            : mp_timer{ nullptr },
              m_time_countdown_ms{ time_countdown_ms }
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
        /** @brief Resets the time down-count for this watchdog. */
        inline void reset() noexcept(false)
        {
            start();
        }

        /** @brief Starts this watchdog. */
        void start() noexcept(false)
        {
            if (mp_timer != nullptr)
                stop();

            mp_timer = new mtmp::Timer(m_time_countdown_ms, 1, false);
            if (mp_timer == nullptr)
                throw mtmp::Watchdog::StartException();
            else
                mp_timer->start();
        }

        /** @brief Stops this watchdog. */
        inline void stop() noexcept
        {
            if (mp_timer != nullptr) {
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
        * This is the code that shoul be run when the time count-
        * down completes. Inheriting classes should implement it.
        */
        virtual inline void run()
        {}


    private:
         //---   Attributes   ------------------------------------------------
        mtmp::Timer* mp_timer;
        double       m_time_countdown_ms;
    };

}
