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

#include <windows.h>
#include <processthreadsapi.h>


module mtmp.thread;


//===========================================================================
namespace avt::mtmp
{
    /** Destructor. */
    Thread::~Thread() noexcept
    {
        stop();
        if (is_ok())
            delete mp_thread;
    }

    /** Sets the priority level for this running thread. */
    bool Thread::set_priority(const long priority)
    {
        if (is_ok()) [[likely]] {
            if (priority == m_priority)
                return true;
            else
                return SetThreadPriority(mp_thread->native_handle(), priority) != 0;  // win32 function
        }
        else [[unlikely]] {
            return false;
        }
    }

    /** Starts (at will) the processing of this thread. */
    void Thread::start() noexcept(false)
    {
        if (m_already_started.load()) {
            throw avt::mtmp::Thread::StartedException();
        }
        else {
            // launches the thread
            mp_thread = new std::thread([this]() { this->_run(); });  // got it?
            if (mp_thread == nullptr)
                throw avt::mtmp::Thread::CreationException();
            // then sets its priority level
            set_priority(m_priority);
        }
    }

    /** Asks for the stopping of the processing of this thread. */
    void Thread::stop() noexcept
    {
        if (is_running())
        {
            m_is_running.store(false);
            ms_active_threads_count--;
        }
    }

}
