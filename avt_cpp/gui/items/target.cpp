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

#include <cmath>

#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/core/types.hpp>

#include "utils/types.h"


module gui.items.target;

import gui.views.view;


//===========================================================================
namespace avt::gui::items
{
    //=======================================================================
    /** Draws this target resized in the specified view. */
    void Target::draw(avt::gui::views::View& view, const float ratio)
    {
        int view_x, view_y;
        int targ_x, targ_y;
        int width, height;

        const int target_size{ int(std::lround(size_cm * ratio)) };
        const int view_width{ view.width() };
        const int view_height{ view.height() };

        // evaluates X-offsets
        if (target_size < view_width) {
            view_x = (view_width - target_size) / 2;
            targ_x = 0;
            width = target_size;
        }
        else {
            view_x = 0;
            targ_x = (target_size - view_width) / 2;
            width = view_width;
        }

        // evaluates Y-offsets
        if (target_size < view_height) {
            view_y = (view_height - target_size) / 2;
            targ_y = 0;
            height = target_size;
        }
        else {
            view_y = 0;
            targ_y = (target_size - view_height) / 2;
            height = view_width;
        }

        // inserts the target picture into the embedding view, at proper position
        if (target_size < view_width || target_size < view_height)
            view.clear();

        const avt::ImageType resized_pict{ get_resized(ratio) };
        resized_pict(cv::Rect(targ_x, targ_y, width, height)).copyTo(view(cv::Rect(view_x, view_y, width, height)));
    }


    /** Loads the whole picture of this target. */
    inline avt::ImageType& Target::get_picture() noexcept(false)
    {
        if (picture.empty())
            picture = cv::imread(filepath.string());
        if (picture.empty())
            throw TargetPictureLoadException();
        return picture;
    }

    /** Resizes the picture of this target according to the specified ratio. */
    avt::ImageType Target::get_resized(const float ratio) noexcept(false)
    {
        avt::ImageType resized_picture;
        cv::resize(get_picture(), resized_picture, cv::Size(), ratio, ratio, cv::INTER_CUBIC);
        return resized_picture;
    }

}