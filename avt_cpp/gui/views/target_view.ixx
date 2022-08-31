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

#include <algorithm>

#include <opencv2/imgproc.hpp>
#include <opencv2/core/mat.hpp>
#include <opencv2/core/types.hpp>

#include "utils/types.h"


export module gui.views.target_view;


import avt.config;
import gui.views.view_prop;


import utils.coords2d;
import utils.rgb_color;
import utils.size;
import gui.items.target;
import utils;


//===========================================================================
export namespace avt::gui::views
{
    //=======================================================================
    /** @brief The class of the views that cembed targets pictures. */
    class TargetView : public gui::views::ViewProp
    {
    public:
        //---   Wrappers   --------------------------------------------------
        using MyBaseClass = gui::views::ViewProp;


        //---   Constructors / Destructors   --------------------------------
        /** @brief Value Constructor. */
        TargetView(const avt::gui::views::View& parent_view,
                   const float                  x,
                   const float                  y,
                   const float                  width,
                   const float                  height,
                   const avt::utils::RGBColor&  bg_color = avt::config::DEFAULT_BACKGROUND) noexcept(false);

        /** @brief Deleted Empty constructor. */
        TargetView() noexcept = delete;

        /** @brief Deleted Copy constructor. */
        TargetView(const TargetView&) noexcept = delete;

        /** @brief Deleted Move constructor. */
        TargetView(TargetView&&) noexcept = delete;

        /** @brief Default Destructor. */
        ~TargetView() noexcept = default;


        //---   Operations   -----------------------------------------------
        /** @brief Draws this view content in its parent view. */
        inline void draw() noexcept
        {
            if (b_shown) {
                draw_target();
                draw_borders();
                MyBaseClass::draw();
            }
        }

        /** @brief Draws the borders of this view. */
        void draw_borders() noexcept;

        /** @brief Draws the contained target in this view. */
        void draw_target() noexcept;


        /** @brief Hides this target view.
        *
        * @sa show().
        */
        inline void hide() noexcept
        {
            b_shown = false;
        }


        /** @brief Selects the simulated and the true distances.
        * 
        * The  simulated distance is the distance that the archer wants to 
        * simulate with the selected target.
        * The true distance is the distance between the display screen and 
        * the archer.
        */
        void select_distances() noexcept;


        /** @brief Selects a target from the list of all available targets. */
        void select_target() noexcept;


        /** @brief Shows this target view.
        *
        * @sa hide().
        */
        inline void show() noexcept
        {
            b_shown = true;
        }


        //---   Attributes   ------------------------------------------------
        avt::gui::items::Target target;
        float                   simulated_dist;
        float                   true_dist;
        float                   displayed_ratio;
        bool                    b_shown;


    protected:
        //---   Operations   ------------------------------------------------
        /** @brief Evaluates the resizing of the displayed target on display.
        *
        * This ratio is a function of:
        *   - the simulated distance
        *   - the true distance
        *   - and the dpi of the used display - which is also
        *     a function of the display resolution.
        */
        void _evaluate_display_ratio() noexcept;

    };

}
