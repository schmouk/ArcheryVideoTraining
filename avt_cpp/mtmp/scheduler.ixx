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

#include <Windows.h>
#include <timeapi.h>


export module mtmp.scheduler;


//===========================================================================
namespace avt::mtmp
{
    //=======================================================================
    /** @brief The class of mutexes, with timeout processing if needed. */
    export class Scheduler
    {
    public:
        //===================================================================
        //---   Constructors / Destructors   --------------------------------
        /** @brief Constructor with new scheduling time slice specification. */
        inline Scheduler(const unsigned int time_slice_ms) noexcept
        {

            m_time_slice_ms = _clamp(time_slice_ms);
            set_slice_duration(m_time_slice_ms);
        }

        /** @brief Destructor. */
        inline ~Scheduler() noexcept
        {
            restore_slice_duration(m_time_slice_ms);
        }


        //---   Windows Operations   ----------------------------------------
        /** @brief Restores the former time slice duration before setting.
        *
        * Returns true if things went fine, or false otherwise.
        */
        static inline bool restore_slice_duration(const unsigned int time_slice_ms) noexcept
        {
            return timeEndPeriod(time_slice_ms) == TIMERR_NOERROR;  // win32 function and const
        }

        /** @brief Sets the new time slice duration (time expressed in ms).
        *
        * Returns true if things went fine, or false otherwise.
        */
        static inline bool set_slice_duration(const unsigned int time_slice_ms) noexcept
        {
            return timeBeginPeriod(time_slice_ms) == TIMERR_NOERROR;  // win32 function and const
        }


        //---   Attributes   ------------------------------------------------
        unsigned int m_time_slice_ms;


    private:
        static inline bool  _already_inited{ false };
        static unsigned int _MIN_TIME_SLICE_ms;
        static unsigned int _MAX_TIME_SLICE_ms;

        static const unsigned int _clamp(const unsigned int time_slice_ms) noexcept
        {
            if (!_already_inited) {
                TIMECAPS device_caps;  // win32 structure
                if (timeGetDevCaps(&device_caps, sizeof(device_caps)) == MMSYSERR_NOERROR) {  // win32 function and const
                    _MIN_TIME_SLICE_ms = device_caps.wPeriodMin;
                    _MAX_TIME_SLICE_ms = device_caps.wPeriodMax;
                }
                else {
                    _MIN_TIME_SLICE_ms = 3;
                    _MAX_TIME_SLICE_ms = 24;
                }
                _already_inited = true;
            }

            return std::clamp<unsigned int>(time_slice_ms, _MIN_TIME_SLICE_ms, _MAX_TIME_SLICE_ms);
        }
    };

     unsigned int mtmp::Scheduler::_MIN_TIME_SLICE_ms;
     unsigned int mtmp::Scheduler::_MAX_TIME_SLICE_ms;

}