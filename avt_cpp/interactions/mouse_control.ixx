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


export module interactions.mouse;

import interactions.app_automaton;
import gui.avt_window;


//===========================================================================
export namespace avt::interactions
{
    //=======================================================================
    /** @brief The class of mouse interactions. */
    class MouseControl
    {
    public:
        //--- Constructors / Destructors ------------------------------------
        /** @brief Value Constructor. */
        MouseControl(avt::gui::AVTWindow* p_avt_window) noexcept;

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
        static void controller(int event, int x, int y, int flags, void* user_data);


    private:
        //---   Attributes   ------------------------------------------------
        avt::interactions::AppAutomaton mp_automaton;
    };

}
