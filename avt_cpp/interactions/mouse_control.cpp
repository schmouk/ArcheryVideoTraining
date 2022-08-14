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

#include <opencv2/highgui.hpp>


module interactions.mouse;

import interactions.app_automaton;
import gui.avt_window;


//===========================================================================
namespace avt::interactions
{
    /** Value Constructor. */
    MouseControl::MouseControl(avt::gui::AVTWindow* p_avt_window) noexcept
        : mp_automaton{ p_avt_window }
    {
        cv::setMouseCallback(p_avt_window->window_id, controller, this);
    }

    /** The internal Mouse Controller implementation. */
    void MouseControl::controller(int event, int x, int y, int flags, void* user_data)
    {
        // reinterprets the user_data type
        avt::interactions::MouseControl* p_mouse_control = reinterpret_cast<avt::interactions::MouseControl*>(user_data);
        if (p_mouse_control == nullptr)
            return;

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
            // should never happen!
        }

        } // ...end of switch
    }

}
