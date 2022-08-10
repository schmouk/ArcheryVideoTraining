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


module mtmp.timer;

import mtmp.thread;


//===========================================================================
namespace avt::mtmp
{
    /** Constructor. */
    Timer::Timer(const double period_ms, const bool b_delay) noexcept
        : avt::mtmp::Thread{},
        m_period_ms{ std::chrono::milliseconds(llround(period_ms)) },
        m_n_repeats{ 0 },
        m_b_delay{ b_delay }
    {}

    /** Constructor. */
    Timer::Timer(const double period_ms,
                 const size_t n_repeats,
                 const bool   b_delay) noexcept
        : avt::mtmp::Thread{},
            m_period_ms{ std::chrono::milliseconds(llround(period_ms)) },
            m_n_repeats{ n_repeats },
            m_b_delay{ b_delay }
    {}

    /** The internal running method. */
    void Timer::_run() noexcept
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

}
