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
#include <chrono>
#include <iostream>

#include <opencv2/highgui.hpp>

#include "devices/cameras/camera.h"

export module unit_tests.devices.cameras.test_camera;


//===========================================================================
namespace avt::unit_tests::devices::cameras
{
    //=======================================================================
    export void test_camera()
    {
        std::cout << "-- TEST avt::devices::cameras::Camera\n";

        avt::devices::cameras::Camera cam0(0, 640*1.5f, 480*1.5f);
        std::cout << "camera 0, H/W size = " << cam0.get_hw_width() << " x " << cam0.get_hw_height() << std::endl;

        avt::devices::cameras::Camera cam1(1);
        std::cout << "camera 1, H/W size = " << cam1.get_hw_width() << " x " << cam1.get_hw_height() << std::endl;

        avt::devices::cameras::Camera cam2(2);
        std::cout << "camera 2, H/W size = " << cam2.get_hw_width() << " x " << cam2.get_hw_height() << std::endl;

        avt::devices::cameras::NullCamera nullcam(2);
        assert(!nullcam.is_ok());

        auto last_time_point = std::chrono::steady_clock::now();
        while (true) {
            cv::imshow("camera 0", cam0.read());
            cv::imshow("camera 1", cam1.read());
            cv::imshow("camera 2", cam2.read());

            const auto time_point = std::chrono::steady_clock::now();
            std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(time_point - last_time_point).count() << std::endl;
            last_time_point = time_point;

            if (cv::waitKey(1) == 27)
                break;
        }

        std::cout << "   All tests OK\n\n";
    }
}