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

#include <string>
#include <opencv2/core/types.hpp>


export module gui.views.view_prop;

import avt.config;
import utils.rgb_color;
import gui.views.view;


//===========================================================================
export namespace avt::gui::views
{
    //=======================================================================
    /** @brief The class of views defined by proportional values.

    * These proportional values are set in interval [0.0, 1.0] and represent
    * percentages of the embedding window size (i.e. width and height).
    */
    class ViewProp : public avt::gui::views::View
    {
    public:
        //---   Constructors / Destructor   ---------------------------------
        /** @brief Constructor. */
        ViewProp(const avt::gui::views::View& parent_view,
                 const float                  x,
                 const float                  y,
                 const float                  width,
                 const float                  height,
                 const avt::utils::RGBColor&  bg_color = avt::config::DEFAULT_BACKGROUND) noexcept(false);

        /** @brief Deleted Empty constructor. */
        ViewProp() noexcept = delete;

        /** @brief Deleted Copy constructor. */
        ViewProp(const ViewProp&) noexcept = delete;

        /** @brief Deleted Move constructor. */
        ViewProp(ViewProp&&) noexcept = delete;

        /** @brief Default Destructor. */
        ~ViewProp() noexcept = default;


        //---   Attributes   ------------------------------------------------
        avt::gui::views::View parent_view;
        avt::utils::RGBColor  bg_color;
        float                 prop_x;
        float                 prop_y;
        float                 prop_width;
        float                 prop_height;


    private:
        //---   Class attribute   -------------------------------------------
        static inline constexpr std::string m_ERR_FORMAT = "!!! ERROR: {:s} value must be in range [0.0; 1.0], can't be {:g}";
    };
}
