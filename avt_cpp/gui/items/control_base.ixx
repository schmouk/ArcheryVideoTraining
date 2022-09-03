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


export module gui.items.control_base;


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
        ControlBase(const avt::gui::views::View& parent_view,
                    const avt::CoordsType        x,
                    const avt::CoordsType        y,
                    const avt::DimsType          width     = 0,
                    const avt::DimsType          height    = 0,
                    const bool                   b_visible = true,
                    const bool                   b_enables = true,
                    const bool                   b_active  = true) noexcept;

        /** @brief Value Constructor (pos, size). */
        ControlBase(const avt::gui::views::View& parent_view,
                    const avt::utils::Coords2D&  pos,
                    const avt::utils::Size&      size = avt::utils::Size{},
                    const bool                   b_visible = true,
                    const bool                   b_enables = true,
                    const bool                   b_active  = true) noexcept;

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


        //---   Drawings   --------------------------------------------------
        /** @brief Draws this control in the parent view.
        *
        * This method internally calls protected method '_draw()' which must
        * be implemented in inheriting classes.
        *
        * @sa related method '_draw()'.
        *
        * @param b_forced: bool
        *   Set this to True to get this control drawn whatever its  refresh
        *   status is. Set it to false to get this control drawn only if its
        *   refresh status is true. Defaults to true.
        */
        inline void draw(const bool b_forced = true) noexcept
        {
            draw(parent_view, b_forced);
        }

        /** @brief Draws this control in the specified view.
        *
        * This method internally calls protected method '_draw()' which must
        * be implemented in inheriting classes.
        *
        * @sa related method '_draw()'.
        *
        * @param view: View
        *   A reference to the embedding view.
        * @param b_forced: bool
        *   Set this to True to get this control drawn whatever its  refresh
        *   status is. Set it to false to get this control drawn only if its
        *   refresh status is true. Defaults to true.
        */
        inline void draw(avt::gui::views::View& view, const bool b_forced = true) noexcept
        {
            if (b_visible && (b_forced || b_refresh)) {
                _draw(view);
                b_refresh = false;
            }
        }

        /** @brief Draws this control in the parent view (forced position).
        *
        * The position set at contruction time is not modified.
        *
        * @sa related method '_draw()'.
        *
        * @param x: int
        *   The x-left position of the drawing in the view.
        * @param y: int
        *   The y-top position of the drawing in the view.
        * @param b_forced: bool
        *   Set this to True to get this control drawn whatever its  refresh
        *   status is. Set it to false to get this control drawn only if its
        *   refresh status is true. Defaults to true.
        */
        inline void draw(const int x, const int y, const bool b_forced = true) noexcept
        {
            draw(parent_view, x, y, b_forced);
        }

        /** @brief Draws this control in the parent view (forced position).
        *
        * The position set at contruction time is not modified.
        * This method internally calls protected method '_draw()' which must
        * be implemented in inheriting classes.
        *
        * @sa related method '_draw()'.
        *
        * @param pos: avt::utils::Coords2D
        *   The position of the drawing in the view.
        * @param b_forced: bool
        *   Set this to True to get this control drawn whatever its  refresh
        *   status is. Set it to false to get this control drawn only if its
        *   refresh status is true. Defaults to true.
        */
        inline void draw(const avt::utils::Coords2D& pos, const bool b_forced = true) noexcept
        {
            draw(parent_view, pos, b_forced);
        }

        /** @brief Draws this control in the specified view (forced position).
        *
        * This method internally calls protected method '_draw()' which must
        * be implemented in inheriting classes.
        *
        * @sa related method '_draw()'.
        *
        * @param view: View
        *   A reference to the embedding view.
        * @param x: int
        *   The x-left position of the drawing in the view.
        * @param y: int
        *   The y-top position of the drawing in the view.
        * @param b_forced: bool
        *   Set this to True to get this control drawn whatever its  refresh
        *   status is. Set it to false to get this control drawn only if its
        *   refresh status is true. Defaults to true.
        */
        void draw(avt::gui::views::View& view,
                  const int              x,
                  const int              y,
                  const bool             b_forced = true) noexcept;

        /** @brief Draws this control in the specified view (forced position).
        *
        * This method internally calls protected method '_draw()' which must
        * be implemented in inheriting classes.
        *
        * @sa related method '_draw()'.
        *
        * @param view: View
        *   A reference to the embedding view.
        * @param pos: avt::utils::Coords2D
        *   The position of the drawing in the view.
        * @param b_forced: bool
        *   Set this to True to get this control drawn whatever its  refresh
        *   status is. Set it to false to get this control drawn only if its
        *   refresh status is true. Defaults to true.
        */
        inline void draw(avt::gui::views::View&      view,
                         const avt::utils::Coords2D& pos,
                         const bool                  b_forced = true) noexcept
        {
            draw(view, pos.x, pos.y, b_forced);
        }


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


        //---   Moving   ----------------------------------------------------
        /** @brief Relative move of this control position (two scalar offsets). */
        template<typename X, typename Y>
            requires std::is_arithmetic_v<X>&& std::is_arithmetic_v<Y>
        inline void move(const X dx, const Y dy) noexcept
        {
            pos.move(dx, dy);
            refresh();
        }

        /** @brief Relative move of this control position (one Coords2D offset). */
        inline void move(const avt::utils::Coords2D& offset) noexcept
        {
            move(offset.x, offset.y);
        }

        /** @brief Relative move of this control position (one 2-D container offset). */
        template<typename P>
            requires avt::is_pair_type_v<P>
        inline void move(const P& offset) noexcept
        {
            move(offset[0], offset[1]);
        }

        /** @brief Absolute move of this control position (two scalar new coordinates). */
        template<typename X, typename Y>
            requires std::is_arithmetic_v<X>&& std::is_arithmetic_v<Y>
        inline void move_at(const X new_x, const Y new_y)
        {
            pos.move_at(new_x, new_y);
            refresh();
        }

        /** @brief Absolute move of this position (one Coords2D offset). */
        inline void move_at(const avt::utils::Coords2D& offset) noexcept
        {
            move_at(offset.x, offset.y);
        }

        /** @brief Absolute move of this position (one 2-D container offset). */
        template<typename P>
            requires avt::is_pair_type_v<P>
        inline void move_at(const P& offset) noexcept
        {
            move_at(offset[0], offset[1]);
        }


        //---   Attributes   ------------------------------------------------
        avt::gui::views::View parent_view;
        avt::utils::Coords2D  pos;
        avt::DimsType         height;
        avt::DimsType         width;
        bool                  b_active;
        bool                  b_enabled;
        bool                  b_refresh;
        bool                  b_visible;


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
