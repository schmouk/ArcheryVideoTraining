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

#include <opencv2/highgui.hpp>
#include <opencv2/core/types.hpp>

#include "devices/cameras/camera.h"
#include "utils/types.h"


module gui.views.camera_view;

import utils.coords2d;
//import gui.items.label;
import utils.rgb_color;
import utils.size;
import gui.views.view;


//===========================================================================
namespace avt::gui::views
{
    /** Draws this camera view content within the parent window. */
    void CameraView::draw() noexcept
    {
        m_draw_fps();
        //label.draw();
        m_draw_borders();
        MyBaseType::draw();
    }


    /** Draws a new camera frame within this camera view. */
    void CameraView::draw_frame(avt::ImageType& camera_frame) noexcept
    {
        /*** /
        frame_height, frame_width = frame.shape[:2]

        if frame_width != self.width or frame_height != self.height:

            ratio_x = self.width / frame_width
            ratio_y = self.height / frame_height
            ratio = min( ratio_x, ratio_y )
            frame = cv2.resize( frame, None, fx=ratio, fy=ratio, interpolation=cv2.INTER_LINEAR )

            new_height, new_width = frame.shape[:2]
            if new_width > self.width:
                new_width = self.width
            if new_height > self.height:
                new_height = self.height

            x = (self.width - new_width) // 2
            y = (self.height - new_height) // 2

            self.content = np.zeros( (self.height, self.width, 3), np.uint8 ) + 16
            self.content[ y:y+new_height,
                          x:x+new_width, : ] = frame[ :new_height, :new_width, : ]

        else:
            self.content = frame

        if self.b_flip_image:
            self.content = cv2.flip( self.content, 1 )

        self.draw()
        /***/
    }


    /** internally sets this instance attributes. */
    void CameraView::m_create_view() noexcept
    {
        view_name = std::format("Cam-{}", camera.get_id());
        //label = Label(*this, view_name, 20, 40);
        //fps_label = Label(*this, "", 20, 70, NULL, Font(14, avt::utils::RGBColor::YELLOW));
        //fps_frame_rate = FPSFrameRate(15);
        _CAM_VIEWS_COUNT++;
        draw();

        /*** /
        self.view_name = f"Cam-{camera.get_id()}"
        self.label = Label( self, self.view_name, 20, 40 )
        self.fps_label = Label( self, "", 20, 70, None, Font(14, YELLOW) )
        self.fps_rate = FPSRateFrames( 15 )
        self.joined = False
        
        self.camera = camera
        CameraView._CAM_VIEWS_COUNT += 1

        camera_frames_buffer = CameraFramesBuffer( 4 )
                
        super().__init__( parent, x, y, width, height, parent_rect )

        self.acq_thread  = CameraAcquisition(   self.camera, camera_frames_buffer )
        self.disp_thread = CameraDirectDisplay( self.camera, camera_frames_buffer, self )
        
        self.draw()
        /***/
    }


    /** Draws the borders of this camera view. */
    void CameraView::m_draw_borders() noexcept
    {
        avt::utils::RGBColor bg_color = avt::config::DEFAULT_BACKGROUND;

        (*this)(cv::Range(0, 2)                  , cv::Range::all()) = bg_color;
        (*this)(cv::Range(height() - 2, height()), cv::Range::all()) = bg_color;
        (*this)(cv::Range::all(), cv::Range(0, 2))                   = bg_color;
        (*this)(cv::Range::all(), cv::Range(width() - 2, width()))   = bg_color;

        (*this)(cv::Range(2, 3), cv::Range(2, width()))                           = bg_color / 1.5f;
        (*this)(cv::Range(2, height()-1), cv::Range(2,3))                         = bg_color / 1.5f;
        (*this)(cv::Range(height() - 3, height() - 2), cv::Range(3, width()))     = bg_color * 3;
        (*this)(cv::Range(3, height() - 2), cv::Range(width() - 3, width() - 2))  = bg_color * 3;
        (*this)(cv::Range(3, 4), cv::Range(3, width() - 1))                       = bg_color / 2;
        (*this)(cv::Range(4, height() - 2), cv::Range(3, 4))                      = bg_color / 2;
        (*this)(cv::Range(height() - 4, height() - 3), cv::Range(4, width() - 1)) = bg_color * 1.5f;
        (*this)(cv::Range(4, height() - 2), cv::Range(width() - 3, width() - 2))  = bg_color * 1.5f;

        /*** /
        bg_color = RGBColor( *AVTConfig.DEFAULT_BACKGROUND.color )

        self.content[  0,  : ] = bg_color.color
        self.content[  1,  : ] = bg_color.color
        self.content[ -1,  : ] = bg_color.color
        self.content[ -2,  : ] = bg_color.color
        self.content[  :,  0 ] = bg_color.color
        self.content[  :,  1 ] = bg_color.color
        self.content[  :, -1 ] = bg_color.color
        self.content[  :, -2 ] = bg_color.color

        self.content[ 2, 2:-1 ]  = (bg_color / 1.5).color
        self.content[ 2:-2, 2 ]  = (bg_color / 1.5).color
        self.content[ -3, 3:-1 ] = (bg_color * 3).color
        self.content[ 3:-3, -2 ] = (bg_color * 3).color
        self.content[ 3, 3:-2 ]  = (bg_color / 2).color
        self.content[ 4:-3, 3 ]  = (bg_color / 2).color
        self.content[ -4, 4:-2 ] = (bg_color * 1.5).color
        self.content[ 4:-3, -3 ] = (bg_color * 1.5).color        /***/
    }


    /** Draws the FPS rate of the associated camera. */
    void CameraView::m_draw_fps() noexcept
    {
        /*** /
        self.fps_rate.new_frame()
        fps_text = self.fps_rate.get_text()
        if fps_text != '':
            self.fps_label.text = f"{fps_text} fps"
            self.fps_label.draw()
        /***/
    }

}