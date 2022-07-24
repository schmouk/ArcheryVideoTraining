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
#include <mutex>


export module mtmp.mutex;


//===========================================================================
export namespace mtmp
{
    //=======================================================================
    /** @brief The class of mutexes, with timeout processing if needed. */
    class Mutex
    {
    public:
        //---   Constructors / Destructor   ---------------------------------
        /** @brief Default Constructor. */
        inline Mutex() noexcept
            : m_mtx{}
        {}

        /** @brief Deleted Copy constructor. */
        Mutex(const Mutex&) = delete;

        /** @brief Deleted Move constructor. */
        Mutex(Mutex&&) = delete;

        /** @brief Default Destructor. */
        virtual inline ~Mutex() noexcept
        {}


        //---   Assignments   -----------------------------------------------
        /** @brief Deleted Copy assignment. */
        Mutex& operator=(const Mutex&) = delete;

        /** @brief Deleted Move assignment. */
        Mutex& operator=(Mutex&&) = delete;


        //---   Mutex operations   ------------------------------------------
        /** @brief Locking the calling thread.
        *
        * The calling thread will be blocked until this mutex gets unlocked.
        */
        inline void lock() noexcept
        {
            m_mtx.lock();
        }

        /** @brief Locking the calling thread - timed out (seconds).
        *
        * The calling thread will be blocked until this mutex  gets  unlocked
        * or after the timeout period of time expiration.  The timeout period
        * of time is expressed in fractional seconds.
        *
        * Returns true if the function succeeds in locking this mutex for the
        * thread, or false otherwise.
        */
        inline bool lock_s(const double timeout_s) noexcept
        {
            return lock_ms(timeout_s * 1e3);
        }

        /** @brief Locking the calling thread - timed out (milliseconds).
        *
        * The calling thread will be blocked until this mutex  gets  unlocked
        * or after the timeout period of time expiration.  The timeout period
        * of time is expressed in fractional milliseconds.
        * 
        * Returns true if the function succeeds in locking this mutex for the 
        * thread, or false otherwise.
        */
        inline bool lock_ms(const double timeout_ms) noexcept
        {
            return m_mtx.try_lock_for(std::chrono::microseconds(llround(timeout_ms * 1e3)));
        }

        /** @brief Unlocking the formerly locked thread. */
        inline void unlock() noexcept
        {
            m_mtx.unlock();
        }


    private:
        std::timed_mutex m_mtx;  //!< So, inheritance is implemented here via composition.
    };

}