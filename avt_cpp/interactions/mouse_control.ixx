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

#include <cstring>

#include <opencv2/highgui.hpp>


export module interactions.mouse;

import gui.avt_window;


//===========================================================================
namespace avt::interactions
{
    //=======================================================================
    /** @brief The class of mouse interactions. */
    class MouseControl
    {
    public:
        //--- Constructors / Destructors ------------------------------------
        /** @brief Value Constructor. */
        MouseControl(avt::gui::AVTWindow* p_avt_window) noexcept
            : mp_avt_window( p_avt_window )
        {
            // sets this mouse controller callback
            cv::setMouseCallback(mp_avt_window->window_id, controller, mp_avt_window);
        }

        /** @brief Deleted Copy constructor. */
        MouseControl(const MouseControl&) = delete;

        /** @brief Delete Move constructor. */
        MouseControl(MouseControl&&) = delete;

        /** @brief Default Destructor. */
        ~MouseControl() = default;
        

    protected:
        //-------------------------------------------------------------------
        /** @brief The internal Mouse Controller implementation.
        *
        * This callback gets the exact signature as specified by  library
        * OpenCV. Unfortunately, it MUST be static.
        *
        * @arg event: int
        *   The related mouse event (move, button down, button up, double
        *   click).
        * @arg x: int
        *   The X-axis position of the cursor when the event happened.
        * @arg y: int
        *   The Y-axis position of the cursor when the event happened.
        * @arg flags: int
        *   Status of mouse buttons and keyboard  control-keys  when  the
        *   event happened.
        * @arg user_data: void*
        *   A pointer to the passed parameters at call time.
        *   @see MouseControl() constructor.
        */
        static void controller(int event, int x, int y, int flags, void* user_data)
        {
            // reinterprets the user_data type
            avt::gui::AVTWindow* p_avt_window = reinterpret_cast<avt::gui::AVTWindow*>(user_data);

            // sets modifiers flags status
            const bool b_alt_key_mod   = (flags & cv::EVENT_FLAG_ALTKEY)   != 0;
            const bool b_ctrl_key_mod  = (flags & cv::EVENT_FLAG_CTRLKEY)  != 0;
            const bool b_shift_key_mod = (flags & cv::EVENT_FLAG_SHIFTKEY) != 0;

            // controls mouse event
            switch (event) {
            case cv::EVENT_MOUSEMOVE: {
                // mouse position moving

            } break;

            case cv::EVENT_MOUSEHWHEEL: {
                // horizontal mouse wheel scrolling

            } break;

            case cv::EVENT_MOUSEWHEEL: {
                // vertical mouse wheel scrolling

            } break;

            case cv::EVENT_LBUTTONDBLCLK: {
                // mouse left button double click

            } break;

            case cv::EVENT_LBUTTONDOWN: {
                // mouse left button pressed

            } break;

            case cv::EVENT_LBUTTONUP: {
                // mouse left button released

            } break;

            case cv::EVENT_MBUTTONDBLCLK: {
                // mouse middle button double click

            } break;

            case cv::EVENT_MBUTTONDOWN: {
                // mouse middle button pressed

            } break;

            case cv::EVENT_MBUTTONUP: {
                // mouse middle button released

            } break;

            case cv::EVENT_RBUTTONDBLCLK: {
                // mouse right button double click

            } break;

            case cv::EVENT_RBUTTONDOWN: {
                // mouse right button pressed

            } break;

            case cv::EVENT_RBUTTONUP: {
                // mouse right button released

            } break;

            default: {
                // should never happen...
            }

            } // ...end of switch
        }


    private:
        //---   Attributes   ------------------------------------------------
        avt::gui::AVTWindow* mp_avt_window;
    };


    //=======================================================================
    void mouse_controller(int event, int x, int y, int flags, void* user_data)
    {
        // reinterprets the user_data type
        avt::gui::AVTWindow* p_avt_window = reinterpret_cast<avt::gui::AVTWindow*>(user_data);

        // sets modifiers flags status
        const bool b_alt_key_mod   = (flags & cv::EVENT_FLAG_ALTKEY)   != 0;
        const bool b_ctrl_key_mod  = (flags & cv::EVENT_FLAG_CTRLKEY)  != 0;
        const bool b_shift_key_mod = (flags & cv::EVENT_FLAG_SHIFTKEY) != 0;

        // controls mouse event
        switch (event) {
        case cv::EVENT_MOUSEHWHEEL: {
            // horizontal mouse wheel scrolling

        } break;

        case cv::EVENT_MOUSEMOVE: {
            // mouse position moving

        } break;

        case cv::EVENT_MBUTTONDBLCLK: {
            // mouse middle button double click

        } break;


        }
    }

}