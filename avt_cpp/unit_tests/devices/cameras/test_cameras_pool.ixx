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

#include <cassert>
#include <format>
#include <iostream>

#include <opencv2/highgui.hpp>
#include <opencv2/core/mat.hpp>

#include "devices/cameras/camera.h"
#include "utils/types.h"

export module unit_tests.devices.cameras.test_cameras_pool;

import devices.cameras.cameras_pool;
import utils.rgb_color;


//===========================================================================
namespace avt::unit_tests::devices::cameras
{
    //=======================================================================
    export void test_cameras_pool()
    {
        std::cout << "-- TEST avt::devices::cameras::Camera\n";

        
        avt::ImageType console( 400, 600, (cv::Vec3b)avt::utils::RGBColor::DEEP_GRAY );
        avt::devices::cameras::CamerasPool cameras_pool(console);


        while (true) {
            for (auto cam: cameras_pool)
                cv::imshow(std::format("camera #{}", cam.get_id()), cam.read());

            if (cv::waitKey(1) == 27)
                break;
        }

        std::cout << "   All tests OK\n\n";
    }
}