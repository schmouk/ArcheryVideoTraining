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
#include <shellscalingapi.h>

#include <cmath>


module devices.displays.display_monitor;


//===========================================================================
namespace avt::devices::displays
{
    /** Value constructor. */
    DisplayMonitor::DisplayMonitor(const HMONITOR monitor_handle,
                                   const HDC      display_context_handle,
                                   const int      left_x,
                                   const int      top_y,
                                   const int      width,
                                   const int      height) noexcept
        : win_handle(monitor_handle),
          win_dc_handle(display_context_handle),
          x(left_x),
          y(top_y),
          width(width),
          height(height)
        {
            if (is_ok())
                set_system_data();
        }

    /** Sets Windows additional features associated with this display monitor. */
    void DisplayMonitor::set_system_data() noexcept
    {
        MONITORINFO my_info;
        my_info.cbSize = sizeof(my_info);
        GetMonitorInfoA(win_handle, &my_info);
        is_primary_screen = ((my_info.dwFlags & MONITORINFOF_PRIMARY) == MONITORINFOF_PRIMARY);

        GetDpiForMonitor(win_handle, MDT_RAW_DPI, &raw_dpiX, &raw_dpiY);
        GetDpiForMonitor(win_handle, MDT_EFFECTIVE_DPI, &scaled_dpiX, &scaled_dpiY);

        width_mm  = GetDeviceCaps(win_dc_handle, HORZSIZE);
        height_mm = GetDeviceCaps(win_dc_handle, VERTSIZE);

        if (width_mm == 0) {
            width_mm  = lround(width * 25.4 / raw_dpiX);
            height_mm = lround(height * 25.4 / raw_dpiY);
        }

        vertical_refresh_rate = GetDeviceCaps(win_dc_handle, VREFRESH);
    }

}

