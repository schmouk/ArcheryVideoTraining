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

#include "utils/types.h"


export module gui.views.control_base;


import utils.coords2d;
import utils.size;
import gui.views.view;


//===========================================================================
export namespace avt::gui::items
{
    //=======================================================================
    /** @brief The class of the views that cembed targets pictures. */
    class ControlBase
    {
    public:
        //---   Constructors / Destructors   --------------------------------
        /** @brief Value Constructor (x, y, width, height). */
        ControlBase(const avt::CoordsType x,
                    const avt::CoordsType y,
                    const avt::DimsType   width,
                    const avt::DimsType   height,
                    const bool            b_visible = true,
                    const bool            b_enables = true,
                    const bool            b_active  = true) noexcept;

        /** @brief Value Constructor (pos, size). */
        ControlBase(const avt::utils::Coords2D pos,
                    const avt::utils::Size     size,
                    const bool                 b_visible = true,
                    const bool                 b_enables = true,
                    const bool                 b_active = true) noexcept;

        /** @brief Default Contructor. */
        ControlBase() noexcept = default;

        /** @brief Default Copy constructor. */
        ControlBase(const ControlBase&) noexcept = default;

        /** @brief Default Move constructor. */
        ControlBase(ControlBase&&) noexcept = default;

        /** @brief Default Destructor. */
        virtual ~ControlBase() noexcept = default;


        //---   Assignments   -----------------------------------------------
        /** @brief Default Copy assignment. */
        ControlBase& operator= (const ControlBase&) noexcept = default;

        /** @brief Default Move assignment. */
        ControlBase& operator= (ControlBase&&) noexcept = default;


        //---   Operations   ------------------------------------------------
        /** @brief Makes this control active.
        *
        * @sa deactivate()
        */
        inline void activate() noexcept
        {
            b_active = true;
            refresh();
        }

        /** @brief Makes this control inactive.
        *
        * @sa activate()
        */
        inline void deactivate() noexcept
        {
            b_active = false;
            refresh();
        }

        /** @brief Makes this control disabled.
        *
        * \sa enable()
        */
        inline void disable() noexcept
        {
            b_enabled = false;
            refresh();
        }

        /** @brief Draws this control in the specified view.
        * 
        * This method internally calls protected method '_draw()' which must 
        * be implemented in inheriting classes.
        * 
        * See related method '_draw()'.
        * 
        * @param view: View
        *   A reference to the embedding view.
        * @param b_forced: bool
        *   Set this to True to get this control drawn whatever its  refresh 
        *   status is. Set it to false to get this control drawn only if its 
        *   refresh status is true. Defaults to false.
        */
        inline void draw(avt::gui::views::View& view, const bool b_forced = false) noexcept
        {
            if (b_visible && (b_forced || b_refresh)) {
                _draw(view);
                b_refresh = false;
            }
        }

        /** @brief Makes this control enabled.
        *
        * @sa disable()
        */
        inline void enable() noexcept
        {
            b_enabled = true;
            refresh();
        }

        /** @brief Makes this control hidden at drawing time.
        *
        * @sa show()
        */
        inline void hide() noexcept
        {
            b_visible = false;
            refresh();
        }

        /** @brief Sets the refresh status of this control.
        *
        * When status refresh is set, this control will be drawn at drawing time.
        */
        inline void refresh() noexcept
        {
            b_refresh = true;
        }

        /** @brief Makes this control shown at drawing time.
        *
        * @sa hide()
        */
        inline void show() noexcept
        {
            b_visible = true;
            refresh();
        }

        //---   Attributes   ------------------------------------------------
        avt::utils::Coords2D pos;
        avt::DimsType        height;
        avt::DimsType        width;
        bool                 b_active;
        bool                 b_enabled;
        bool                 b_refresh;
        bool                 b_visible;


    protected:
        //---   Operations   ------------------------------------------------
        /** @brief Draws this control in the specified view - specific part for inheriting classes.
        * 
        * 
        * This is the protected part of the drawing method. It is automatically 
        * called by method 'draw()'. It relates to the specific part of drawing 
        * inheriting controls. In this baseclass, this method is abstract.
        * 
        * This method MUST BE implemented in inheriting  classes  which  should 
        * check  attribute  '.visible'  to  decide to draw or not this control.
        * Attributes 'b_enabled' and  'b_active'  should  be  checked  also  to 
        * decide the type of drawing for the controls.
        * 
        * @arg view: View
        *   This is a reference to the embedding view for this control.
        */
        virtual void _draw(avt::gui::views::View& view) noexcept = 0;
    };

}
