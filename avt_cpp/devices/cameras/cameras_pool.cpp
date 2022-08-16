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
#include <thread>

#include <opencv2/highgui.hpp>

#include "devices/cameras/camera.h"
#include "utils/types.h"


module devices.cameras.cameras_pool;

import avt.config;


//===========================================================================
namespace avt::devices::cameras
{
    void CamerasPool::evaluate_connected_cameras(const avt::ImageType& console_window) noexcept
    {
        this->clear();

        int x = 20;
        int y = 40;
        int y_offset = avt::config::AVTConsoleFont.size + 9;

        for (int camera_index = 0; camera_index < avt::config::CAMERAS_MAX_COUNT; ++camera_index) {

            avt::devices::cameras::Camera camera{ camera_index };
            if (camera.is_ok()) {
                if (!console_window.empty()) {
                    avt::config::AVTConsoleFont.draw_text(std::format("testing connection of camera #{}: ok ", camera.get_id()),
                                                          (avt::ImageType&)console_window, x, y, false);
                    cv::imshow("AVT console", console_window);
                    cv::waitKey(1);
                }
                push_back(camera);
            }
            else {
                if (!console_window.empty()) {
                    avt::config::AVTConsoleFont.draw_text(std::format("camera #{} not connected or not found", camera.get_id()),
                                                          (avt::ImageType&)console_window, x, y, false);
                    cv::imshow("AVT console", console_window);
                    cv::waitKey(1);
                    std::this_thread::sleep_for(std::chrono::milliseconds(1250));
                }
                break;
            }

            y += y_offset;
        }
    }

}
