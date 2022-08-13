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

module mtmp.barrier;

import mtmp.guarded_block;
import mtmp.mutex;
import mtmp.signal;
import mtmp.thread;


//===========================================================================
namespace avt::mtmp
{
    /** Constructor. */
    Barrier::Barrier(const unsigned long synchronizing_threads_count) noexcept(false)
        : m_sync_threads_count{ synchronizing_threads_count },
          m_waiting_threads_count{ 0 },
          m_guard_mtx{},
          m_turnstile_1{ false },
          m_turnstile_2{ true }
    {
        if (synchronizing_threads_count < 1)
            throw CreationValueException();
    }

    /** Makes all threads to wait on this barrier then run altogether. */
    void Barrier::wait() noexcept(false)
    {
        // First synchronizing step - on turnstile 1
        {
            avt::mtmp::GuardedBlock guard{ &m_guard_mtx };
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
            avt::mtmp::GuardedBlock guard{ &m_guard_mtx };
            m_waiting_threads_count--;
            if (m_waiting_threads_count == 0) {
                m_turnstile_1.wait();
                m_turnstile_2.emit();
            }
        }
        m_turnstile_2.wait();
        m_turnstile_2.emit();
    }

    /** Timed out waiting for this signal (milliseconds). */
    bool Barrier::wait_ms(const double timeout_ms) noexcept(false)
    {
        bool no_timeout = true;

        // First synchronizing step - on turnstile 1
        {
            avt::mtmp::GuardedBlock guard{ &m_guard_mtx };
            m_waiting_threads_count++;
            if (m_waiting_threads_count == m_sync_threads_count) {
                no_timeout = no_timeout && m_turnstile_2.wait_ms(timeout_ms);
                m_turnstile_1.emit();
            }
        }
        no_timeout = no_timeout && m_turnstile_1.wait_ms(timeout_ms);
        m_turnstile_1.emit();

        // Second synchronizing step - on turnstile 2
        {
            avt::mtmp::GuardedBlock guard{ &m_guard_mtx };
            m_waiting_threads_count--;
            if (m_waiting_threads_count == 0) {
                no_timeout = no_timeout && m_turnstile_1.wait_ms(timeout_ms);
                m_turnstile_2.emit();
            }
        }
        no_timeout = no_timeout && m_turnstile_2.wait_ms(timeout_ms);
        m_turnstile_2.emit();

        return no_timeout;
    }

}
