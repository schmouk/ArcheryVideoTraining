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
#include <cstring>
#include <thread>


module mtmp.watchdog;

import mtmp.thread;
import mtmp.timer;


//===========================================================================
namespace avt::mtmp
{
    /** Constructor. */
    Watchdog::Watchdog(const double time_countdown_ms, const std::string& name) noexcept
        : mp_timer{ nullptr },
          m_time_countdown_ms{ time_countdown_ms },
          m_name{ name }
    {}


    /** Resets the time count-down for this watchdog. */
    void Watchdog::reset() noexcept(false)
    {
        if (mp_timer != nullptr)
            mp_timer->b_reset.store(true);
        
        start();
    }

    /** Starts this watchdog. */
    void Watchdog::start() noexcept(false)
    {
        if (mp_timer != nullptr)
            stop();

        mp_timer = new _Timer(this, (const double)m_time_countdown_ms);

        if (mp_timer == nullptr)
            throw avt::mtmp::Watchdog::StartException();
        else {
            mp_timer->start();
        }
    }

    /** Stops this watchdog. */
    void Watchdog::stop() noexcept
    {
        if (mp_timer != nullptr) {
            mp_timer->stop();
            mp_timer->join();
            delete mp_timer;
            mp_timer = nullptr;
        }
    }

    /** Modifies the time down-count for this watchdog. */
    void Watchdog::set_time_countdown_ms(const double time_countdown_ms) noexcept(false)
    {
        if (time_countdown_ms < 0.5)
            throw avt::mtmp::Watchdog::TimeCountdownException();
        m_time_countdown_ms = time_countdown_ms;
        reset();
    }


    inline Watchdog::_Timer::_Timer(Watchdog* parent_watchdog, const double time_countdown_ms) noexcept
        : mp_parent{ parent_watchdog },
          avt::mtmp::Timer(mp_parent->m_name, time_countdown_ms, 1, true)
    {
        b_reset.store(false);
    }

    void Watchdog::_Timer::_run() noexcept
    {
        std::chrono::system_clock::time_point start_time = std::chrono::system_clock::now();
        std::this_thread::sleep_until(start_time + m_period_ms);
        _prepare_run();
        run();
        _terminate_run();
    }
}
