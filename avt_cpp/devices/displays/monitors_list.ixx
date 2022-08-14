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

#include <vector>


export module devices.displays.monitors_list;

import devices.displays.display_monitor;


//===========================================================================
export namespace avt::devices::diplays
{
    //=======================================================================
    // This module defines :
    //  - class MonitorsList;
    //  - MonitorsList monitors_list;
    //
    
    //=======================================================================
    /** @brief The class of lists of display monitors. */
    class MonitorsList : public std::vector<avt::devices::displays::DisplayMonitor>
    {
    public:
        //---   Constructors / Destructor   ---------------------------------
        /** @brief Default Constructor. */
        inline MonitorsList() noexcept
            : std::vector<avt::devices::displays::DisplayMonitor>()
        {
            EnumDisplayMonitors(nullptr, nullptr, m_init_callback, (LPARAM)this);  // Windows function
        }

        /** @brief Deleted Copy constructor. */
        MonitorsList(const avt::devices::displays::DisplayMonitor&) = delete;

        /** @brief Deleted Move constructor. */
        MonitorsList(avt::devices::displays::DisplayMonitor&&) = delete;

        /** @brief Default Destructor. */
        ~MonitorsList() noexcept = default;


        //---   Assignments   -----------------------------------------------
        /** @brief Deleted Copy assignment. */
        MonitorsList& operator= (const MonitorsList&) = delete;

        /** @brief Deleted Move assignment. */
        MonitorsList& operator= (MonitorsList&&) = delete;


    private:
        /** @brief the internal initialization callback. */
        static BOOL CALLBACK m_init_callback(HMONITOR monitor_handle, HDC dc_handle, LPRECT monitor_display_rect, LPARAM params)
        {
            MonitorsList* monitors = reinterpret_cast<MonitorsList*>(params);

            monitors->push_back(
                avt::devices::displays::DisplayMonitor(
                    monitor_handle,
                    dc_handle,
                    monitor_display_rect->left,
                    monitor_display_rect->top,
                    std::abs(monitor_display_rect->right - monitor_display_rect->left),
                    std::abs(monitor_display_rect->bottom - monitor_display_rect->top)
                )
            );

            return true;
        }
    };


    //=======================================================================
    /** @brief The (single) global instance of the display monitors list. */
    MonitorsList monitors_list;  

}