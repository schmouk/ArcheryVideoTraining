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
#include <opencv2/imgproc.hpp>
#include <opencv2/videoio.hpp>

#include "devices/cameras/camera.h"
#include "utils/types.h"

import avt.config;
import utils.size;


//===========================================================================
namespace avt::devices::cameras
{
    /** Gets next available frame. */
    avt::ImageType& Camera::read() noexcept
    {
        avt::ImageType frame;
        bool ok;
        try {
            ok = cv_cam_handle.read(frame);
        }
        catch (...) {
        }

        if (ok) {
            last_frame = m_resize(frame);
        }
        else {
            if (last_frame.empty())
                last_frame = avt::ImageType(std::max(frame_height, 480),
                                            std::max(frame_width, 640),
                                            avt::config::DEFAULT_BACKGROUND);
            else
                last_frame = avt::config::DEFAULT_BACKGROUND;
        }

        return last_frame;
    }


    /** @brief Resizes the passe image according to previously set dimensions. */
    avt::ImageType Camera::m_resize(avt::ImageType& frame) noexcept
    {
        if (frame.rows != frame_height || frame.cols != frame_width)
        {
            avt::ImageType frame_res;
            cv::resize(frame, frame_res, cv::Size(frame_width, frame_height), 0.0, 0.0, cv::INTER_CUBIC);
            return frame_res;
        }
        else
            return frame;
    }
}

