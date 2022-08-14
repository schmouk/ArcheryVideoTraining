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
#include <semaphore>

export module mtmp.signal;


//===========================================================================
export namespace avt::mtmp
{
    //=======================================================================
    /** @brief The class for Signals.
    *
    * Signals are events with no value that can be broadcasted to
    * receivers that are waiting for them. Upon emission, one and
    * only one receiver will be awakened. Meanwhile, the awakened
    * receiver may emit again the signal.
    */
    class Signal final : public std::binary_semaphore
    {
    public:
        //---   Constructors / Destructor   ---------------------------------
        /** @brief Default Constructor. */
        inline Signal(const bool b_raised = false) noexcept
            : std::binary_semaphore{ b_raised ? 1 : 0 }
        {}

        /** @brief Default Copy constructor. */
        Signal(const Signal&) = default;

        /** @brief Default Move constructor. */
        Signal(Signal&&) = default;

        /** @brief Destructor. */
        ~Signal() = default;


        //---   Assignments   -----------------------------------------------
        /** @brief Deleted assignments. */
        Signal& operator=(const Signal&) = delete;
        Signal& operator=(Signal&&) = delete;


        //---   Signals operations   ----------------------------------------
        /** @brief Emits this signal. */
        inline void emit() noexcept
        {
            release();
        }

        /** @brief Waits for this signal.
        *
        * May throw std::system_error.
        */
        inline void wait() noexcept(false)
        {
            acquire();
        }

        /** @brief Timed out waiting for this signal (seconds).
        *
        * Returns true if signal has been emitted before timeout,  or
        * false otherwise.
        * May throw std::system_error or a timeout-related exception.
        */
        inline bool wait_s(const double timeout_s) noexcept(false)
        {
            return wait_ms(timeout_s * 1e3);
        }

        /** @brief Timed out waiting for this signal (milliseconds).
        *
        * Returns true if signal has been emitted before timeout,  or
        * false otherwise.
        * May throw std::system_error or a timeout-related exception.
        */
        inline bool wait_ms(const double timeout_ms) noexcept(false)
        {
            return try_acquire_for(std::chrono::microseconds(llround(timeout_ms * 1e3)));
        }

        /** @brief Non-blocking wait for this signal.
        *
        * Returns true if signal is available, or false otherwise.
        */
        inline bool try_wait() noexcept
        {
            return try_acquire();
        }
    };

}
