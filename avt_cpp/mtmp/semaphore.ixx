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

#include <algorithm>
#include <chrono>
#include <cstddef>
#include <semaphore>

export module avt.mtmp.semaphore;


//===========================================================================
export namespace avt::mtmp
{
    //=======================================================================
    /** @brief The class for Semaphores. */
    template<const std::ptrdiff_t kValue>
        requires (kValue > 0)
    class Semaphore final : public std::counting_semaphore<kValue>
    {
    public:
        //---   Constructors / Destructor   ---------------------------------
        /** @brief Default Constructor. */
        inline Semaphore(const std::ptrdiff_t init_value = 0) noexcept
            : std::counting_semaphore<kValue>{ std::max(init_value, 0) }
        {}

        /** @brief Deleted constructors. */
        Semaphore(const Semaphore&) = delete;
        Semaphore(Semaphore&&) = delete;

        /** @brief Destructor. */
        ~Semaphore() = default;


        //---   Assignments   -----------------------------------------------
        /** @brief Deleted assignments. */
        Semaphore& operator=(const Semaphore&) = delete;
        Semaphore& operator=(Semaphore&&) = delete;


        //---   Semaphores operations   -------------------------------------
        /** @brief Releases one resource that is protected by this semaphore. */
        inline void release() noexcept
        {
            release();
        }

        /** @brief Waits for the availability of at least one resource that is protected by this semaphore.
        *
        * May throw std::system_error.
        */
        inline void acquire() noexcept(false)
        {
            acquire();
        }

        /** @brief Timed out waiting for this semaphore (seconds).
        *
        * Returns true if this semaphore was available before timeout,
        * or false otherwise.
        * May throw std::system_error or a timeout-related exception.
        */
        inline bool wait_s(const double timeout_s) noexcept(false)
        {
            return wait_ms(timeout_s * 1e3);
        }

        /** @brief Timed out waiting for this semaphore (milliseconds).
        *
        * Returns true if this semaphore was available before timeout,
        * or false otherwise.
        * May throw std::system_error or a timeout-related exception.
        */
        inline bool wait_ms(const double timeout_ms) noexcept(false)
        {
            return std::counting_semaphore<kValue>::try_acquire_for(std::chrono::microseconds(llround(timeout_ms * 1e3)));
        }

        /** @brief Non-blocking wait for this semaphore.
        *
        * Returns true if this semaphore is available, or false otherwise.
        */
        inline bool try_wait() noexcept
        {
            return std::counting_semaphore<kValue>::try_acquire();
        }
    };

}

