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

export module devices.display_monitor;


//===========================================================================
export namespace avt::devices
{
    //=======================================================================
    /** @brief The class of Display Monitors (Windows specific).
    *
    * Embeds all the needed stuff to manage display devices features.
    */
    class DisplayMonitor
    {
    public:

        //---   Features Data   ---------------------------------------------
        HMONITOR win_handle;        //!< Windows handle for this display monitor
        HDC      win_dc_handle;     //!< Windows handle of the device display context for this display monitor

        bool is_primary_screen;     //!< set to true when this monitor is the prmary (main) one.

        int x;                      //!< top_left corner X-position of this display screen
        int y;                      //!< top_left corner Y-position of this display screen
        int width;                  //!< width of this display screen (pixels count)
        int height;                 //!< height of this display screen (pixels count)

        int width_mm;               //!< width of this display screen (in millimeters)
        int height_mm;              //!< height of this display screen (in millimeters)

        int vertical_refresh_rate;  //!< the vertical refresh rate for this display screen (in Hz)

        unsigned int raw_dpiX;      //!< the linear dots per inch density of the screen, horizontal axis
        unsigned int raw_dpiY;      //!< the linear dots per inch density of the screen, vertical axis
        unsigned int scaled_dpiX;   //!< the effective dots per inch density of the screen, as set by the user, horizontal axis
        unsigned int scaled_dpiY;   //!< the effective dots per inch density of the screen, as set by the user, vertical axis


        //---   Constructors / Destructor   ---------------------------------
        /** @brief Default constructor. */
        inline DisplayMonitor(const HMONITOR monitor_handle = nullptr,
            const HDC      display_context_handle = nullptr,
            const int      left_x = 0,
            const int      top_y = 0,
            const int      width = 0,
            const int      height = 0) noexcept
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


        DisplayMonitor(const DisplayMonitor&) = default;
        DisplayMonitor(DisplayMonitor&&) = default;
        virtual ~DisplayMonitor() = default;

        //---   Assignments   -----------------------------------------------
        DisplayMonitor& operator= (const DisplayMonitor&) = default;
        DisplayMonitor& operator= (DisplayMonitor&&) = default;


        //---   Miscelaneous   ----------------------------------------------
        /** @brief Returns true if this monitor descriptor is valid. */
        inline const bool is_ok() const noexcept
        {
            return win_handle != nullptr;
        }


        /** @brief Sets Windows additional features associated with this display monitor. */
        void set_system_data()
        {
            MONITORINFO my_info;
            my_info.cbSize = sizeof(my_info);
            GetMonitorInfoA(win_handle, &my_info);
            is_primary_screen = ((my_info.dwFlags & MONITORINFOF_PRIMARY) == MONITORINFOF_PRIMARY);

            GetDpiForMonitor(win_handle, MDT_RAW_DPI, &raw_dpiX, &raw_dpiY);
            GetDpiForMonitor(win_handle, MDT_EFFECTIVE_DPI, &scaled_dpiX, &scaled_dpiY);

            width_mm = GetDeviceCaps(win_dc_handle, HORZSIZE);
            height_mm = GetDeviceCaps(win_dc_handle, VERTSIZE);

            vertical_refresh_rate = GetDeviceCaps(win_dc_handle, VREFRESH);
        }
    };

}

