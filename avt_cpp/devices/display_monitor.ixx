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
        /** @brief Value constructor. */
        DisplayMonitor(const HMONITOR monitor_handle = 0,
                       const HDC      display_context_handle = 0,
                       const int      left_x = 0,
                       const int      top_y = 0,
                       const int      width = 0,
                       const int      height = 0) noexcept;


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
        void set_system_data();
    };

}

