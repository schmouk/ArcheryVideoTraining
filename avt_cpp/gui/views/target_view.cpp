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

#include <filesystem>


module gui.views.target_view;

import avt.config;
import utils.rgb_color;
import gui.items.target;
import gui.views.view;


//===========================================================================
namespace avt::gui::views
{
    /** @brief Value Constructor. */
    TargetView::TargetView(const avt::gui::views::View& parent_view,
                           const float                  x,
                           const float                  y,
                           const float                  width,
                           const float                  height,
                           const avt::utils::RGBColor&  bg_color) noexcept(false)
        : MyBaseClass(parent_view, x, y, width, height, bg_color),
          target{},
          simulated_dist{ 0.0f },
          true_dist{ 0.0f },
          displayed_ratio{ 1.0f },
          b_shown{ true }
    {}


    /** Draws the borders of this view. */
    void TargetView::draw_borders() noexcept
    {

        (*this)(cv::Range(0, 2)                      , cv::Range::all())                    = bg_color;
        (*this)(cv::Range(height() - 2, height())    , cv::Range::all())                    = bg_color;
        (*this)(cv::Range::all()                     , cv::Range(0, 2))                     = bg_color;
        (*this)(cv::Range::all()                     , cv::Range(width() - 2, width()))     = bg_color;

        (*this)(cv::Range(2, 3)                      , cv::Range(2, width()))               = bg_color / 2;
        (*this)(cv::Range(2, height() - 1)           , cv::Range(2, 3))                     = bg_color / 2;
        (*this)(cv::Range(height() - 3, height() - 2), cv::Range(3, width()))               = bg_color * 3;
        (*this)(cv::Range(3, height() - 2)           , cv::Range(width() - 3, width() - 2)) = bg_color * 3;
        (*this)(cv::Range(3, 4)                      , cv::Range(3, width() - 1))           = avt::utils::RGBColor::BLACK;
        (*this)(cv::Range(4, height() - 2)           , cv::Range(3, 4))                     = avt::utils::RGBColor::BLACK;
        (*this)(cv::Range(height() - 4, height() - 3), cv::Range(4, width() - 1))           = bg_color * 1.5f;
        (*this)(cv::Range(4, height() - 2)           , cv::Range(width() - 3, width() - 2)) = bg_color * 1.5f;
    }


    /** Draws the contained target in this view. */
    void TargetView::draw_target() noexcept
    {
        if (target.is_ok() && displayed_ratio > 0.0)
            target.draw(parent_view, displayed_ratio);
        else
            fill_background();
    }


    /** Selects the simulated and the true distances. */
    void TargetView::select_distances() noexcept
    {
        simulated_dist = 0.0f;  // TODO: implement this
        true_dist = 0.0f;       // TODO: implement this
        _evaluate_display_ratio();
    }


    /** Selects a target from the list of all available targets. */
    void TargetView::select_target() noexcept
    {
        // TODO: implement this
        target = avt::gui::items::Target();
    }


    /** Evaluates the resizing of the displayed target on display. */
    void TargetView::_evaluate_display_ratio() noexcept
    {
        // TODO: implement this
        displayed_ratio = 0.0f;
    }

}