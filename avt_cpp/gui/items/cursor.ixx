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

#include <format>
#include <windows.h>
#include <winuser.h>

#include <opencv2/imgproc.hpp>
#include "utils/types.h"


export module gui.items.cursor;

import utils;
import utils.coords2d;


//===========================================================================
export namespace avt::gui::items
{
    //=======================================================================
    /** @brief The class of mouse cursors. */
    class Cursor
    {
    public:
        //---   Constructors / Destructors   --------------------------------
        /** @brief Default constructor.
        
            @arg cursor_id: int
            The applicative identifier of this cursor.

            @arg b_restore_normal_cursor: bool
            Set this to true to restore the 'normal' cursor  image  after 
            deletion of this cursor. Set this to false to not restore the 
            'normal' cursor otherwise. Defaults to true.
        */
        Cursor(wchar_t* cursor_id, const bool b_restore_normal_cursor = true) noexcept
            : m_cursor_id { cursor_id }, m_b_restore_normal_cursor{ b_restore_normal_cursor }
        {}

        /** @brief Destructor. */
        ~Cursor() noexcept
        {
            if (m_b_restore_normal_cursor)
                SetCursor(LoadCursor(0, IDC_ARROW));  // win32 specifics
        }


        //---   Exceptions   ------------------------------------------------
        /** @brief Exception on erroneous loading of this cursor. */
        class CreationException : public std::exception
        {
            const char* what() const noexcept { return ("!!! Error: Unable to load cursor\n"); }
        };


        //---   Operations   ------------------------------------------------
        /** @brief Activates this cursor and shows it. */
        void activate() noexcept(false)
        {
            try {
                SetCursor(LoadCursor(0, m_cursor_id));  // win32 specifics
                show();
            }
            catch (...) {
                throw(CreationException());
            }
        }

        /** @brief Returns the current position of this cursor.
        *
        * \see set_pos().
        */
        inline const avt::utils::Coords2D get_pos()
        {
            POINT out_pos;
            GetCursorPos(&out_pos);
            return avt::utils::Coords2D(out_pos.x, out_pos.y);
        }

        /** @brief Hides this cursor.
        *
        * \see show().
        */
        inline void hide()
        {
            ShowCursor(false);
        }

        /** @brief Forces the cursor position.
        *
        * \see get_pos().
        */
        inline void set_pos(const avt::utils::Coords2D& new_pos) noexcept
        {
            SetCursorPos(int(new_pos.x), int(new_pos.y));
        }

        /** @brief Forces the cursor position.
        *
        * \see get_pos().
        */
        template<typename X, typename Y>
            requires std::is_arithmetic_v<X> && std::is_arithmetic_v<Y>
        void set_pos(const X new_x, const Y new_y) noexcept
        {
            SetCursorPos(int(new_x), int(new_y));
        }

        /** @brief Forces the cursor position.
        *
        * \see get_pos().
        */
        template<typename P>
            requires avt::is_pair_type_v<P>
        inline void set_pos(const P& new_pos) noexcept(false)
        {
            set_pos(new_pos[0], new_pos[1]);
        }

        /** @brief Shows this cursor.
        *
        * \see hide().
        */
        inline void show()
        {
            ShowCursor(true);
        }


    private:
        //---   Attributes   ------------------------------------------------
        wchar_t* m_cursor_id;
        bool     m_b_restore_normal_cursor;
    };


    //=======================================================================
    Cursor Cursor_CROSSHAIR      (IDC_CROSS      , true);
    Cursor Cursor_HAND           (IDC_HAND       , true);
    Cursor Cursor_HELP           (IDC_HELP       , true);
    Cursor Cursor_NORMAL         (IDC_ARROW      , true);
    Cursor Cursor_SIZEALL        (IDC_SIZEALL    , true);
    Cursor Cursor_SIZE_DOWN      (IDC_SIZENS     , true);
    Cursor Cursor_SIZE_DOWN_LEFT (IDC_SIZENESW   , true);
    Cursor Cursor_SIZE_DOWN_RIGHT(IDC_SIZENWSE   , true);
    Cursor Cursor_SIZE_LEFT      (IDC_SIZEWE     , true);
    Cursor Cursor_SIZE_LEFT_RIGHT(IDC_SIZEWE     , true);
    Cursor Cursor_SIZE_RIGHT     (IDC_SIZEWE     , true);
    Cursor Cursor_SIZE_UP        (IDC_UPARROW    , true);
    Cursor Cursor_SIZE_UP_DOWN   (IDC_SIZENS     , true);
    Cursor Cursor_SIZE_UP_LEFT   (IDC_SIZENWSE   , true);
    Cursor Cursor_SIZE_UP_RIGHT  (IDC_SIZENESW   , true);
    Cursor Cursor_SLASHED_CIRCLE (IDC_NO         , true);
    Cursor Cursor_TEXT           (IDC_IBEAM      , true);
    Cursor Cursor_WAIT           (IDC_WAIT       , true);
    Cursor Cursor_WAIT_ARROW     (IDC_APPSTARTING, true);

}