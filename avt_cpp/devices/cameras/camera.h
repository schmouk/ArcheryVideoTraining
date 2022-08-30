#pragma once
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
#include <opencv2/videoio.hpp>

#include "utils/types.h"

import utils.size;
import utils;


//===========================================================================
namespace avt::devices::cameras
{
    //=======================================================================
    // This module defines:
    //  - class Camera;
    //  - class NullCamera;
    // 

    //=======================================================================
    /** @brief The class of cameras instances.
    *
    * This class is based on OpenCV capturing mechanism.
    */
    class Camera
    {
    public:
        //---   Constructors / Destructor   ---------------------------------
        /** @brief Constructor with set camera id.
        *
        * @arg cam_id: int
        *   The identifier of this camera. Identifiers are integers, as
        *   specified by OpenCV,  that start with index 0 (embedded web
        *   cam on laptops, for instance)  and grows up.  Providing  an
        *   index  for  which  no  hardware  video  capturing device is
        *   connected  leads to  a  faulty  camera  mode  whose  status
        *   'is_ok()'  returns  False.   Connected  cameras  get  their
        *   ok-status as being True.
        */
        inline Camera(const int cam_id) noexcept
        {
            m_create_camera(cam_id);
            m_set_hw_size();
            set_frames_size(hw_default_width, hw_default_height);
        }

        /** @brief Constructor with set id and frames size (2 dimension values).
        *
        * @arg cam_id: int
        *   The identifier of this camera. Identifiers are integers, as
        *   specified by OpenCV,  that start with index 0 (embedded web
        *   cam on laptops, for instance)  and grows up.  Providing  an
        *   index  for  which  no  hardware  video  capturing device is
        *   connected  leads to  a  faulty  camera  mode  whose  status
        *   'is_ok()'  returns  False.   Connected  cameras  get  their
        *   ok-status as being True.*
        * @arg width: int
        *   The wished width for the acquired  frames.  Captured frames
        *   will be resized to this width before being delivered.
        * @arg height: int
        *   The wished height for the acquired  frames. Captured frames
        *   will be resized to this width before being delivered.
        */
        template<typename W, typename H>
            requires std::is_arithmetic_v<W>&& std::is_arithmetic_v<H>
        inline Camera(const int cam_id, const W width, const H height) noexcept
        {
            m_create_camera(cam_id);
            set_frames_size(avt::utils::clamp_us(width), avt::utils::clamp_us(height));
        }

        /** @brief Constructor with set id and frames size (1 size value).
        *
        * @arg cam_id: int
        *   The identifier of this camera. Identifiers are integers, as
        *   specified by OpenCV,  that start with index 0 (embedded web
        *   cam on laptops, for instance)  and grows up.  Providing  an
        *   index  for  which  no  hardware  video  capturing device is
        *   connected  leads to  a  faulty  camera  mode  whose  status
        *   'is_ok()'  returns  False.   Connected  cameras  get  their
        *   ok-status as being True.*
        * @arg size: avt::utils::Size
        *   A reference to the wished width and height for the acquired
        *   frames.  Captured  frames  will  be  resized  to this width
        *   before  being delivered.
        */
        inline Camera(const int cam_id, const avt::utils::Size& size) noexcept
        {
            m_create_camera(cam_id);
            set_frames_size(size.width, size.height);
        }

        /** @brief Constructor with set id and frames size (1 2D-container value).
        *
        * @arg cam_id: int
        *   The identifier of this camera. Identifiers are integers, as
        *   specified by OpenCV,  that start with index 0 (embedded web
        *   cam on laptops, for instance)  and grows up.  Providing  an
        *   index  for  which  no  hardware  video  capturing device is
        *   connected  leads to  a  faulty  camera  mode  whose  status
        *   'is_ok()'  returns  False.   Connected  cameras  get  their
        *   ok-status as being True.*
        * @arg size: avt::utils::Size
        *   A reference to the wished width and height for the acquired
        *   frames. Captured frames will be resized to this size before
        *   being delivered.
        */
        template<typename P>
            requires avt::is_pair_type_v<P>
        inline Camera(const int cam_id, const P& size) noexcept
        {
            m_create_camera(cam_id);
            set_frames_size(size[0], size[1]);
        }

        /** @brief Default Copy constructor. */
        Camera(const Camera&) noexcept = default;

        /** @brief Default Move constructor. */
        Camera(Camera&&) noexcept = default;

        /** @brief Destructor. */
        inline virtual ~Camera() noexcept
        {
            release();
        }


        //---   Assignments   -----------------------------------------------
        /** @brief Default Copy assignment. */
        Camera& operator= (const Camera&) noexcept = default;

        /** @brief Default Move assignment. */
        Camera& operator= (Camera&&) noexcept = default;


        //---   Operations   -----------------------------------------------
        /** @brief Gets next available frame. */
        avt::ImageType& read() noexcept;

        /** @brief Releases all resources that have been allocated with this camera. */
        inline void release() noexcept
        {
            try {
                cv_cam_handle.release();
            }
            catch (...) {}
        }

        /** @brief Resets the size of captured frames to their H/W initial settings. */
        inline void reset_hw_dims() noexcept
        {
            cv_cam_handle.set(cv::CAP_PROP_FRAME_WIDTH, hw_default_width);
            cv_cam_handle.set(cv::CAP_PROP_FRAME_HEIGHT, hw_default_height);
        }


        //---   Accessors / Mutators   -------------------------------------
        /** @brief Gets the frames-per-second rate of capturing for this camera.
        *
        * Notice: always returns 0.0 on webcams in OpenCV.
        */
        inline const double get_fps() const noexcept
        {
            try {
                return cv_cam_handle.get(cv::CAP_PROP_FPS);
            }
            catch (...) {
                return 0.0f;
            }
        }

        /** @brief Gets the height of captured frames as set in the H/W device. */
        inline const int get_hw_height() const noexcept
        {
            try {
                return int(cv_cam_handle.get(cv::CAP_PROP_FRAME_HEIGHT));
            }
            catch (...) {
                return 0;
            }
        }

        /** @brief Gets the width of captured frames as set in the H/W device. */
        inline const int get_hw_width() const noexcept
        {
            try {
                return int(cv_cam_handle.get(cv::CAP_PROP_FRAME_WIDTH));
            }
            catch (...) {
                return 0;
            }
        }

        /** @brief Gets the id of this camera, starting at 1 rather than 0. */
        inline const int get_id() const noexcept
        {
            return cam_id + 1;
        }

        /** @brief Gets the elapsed period of time between two successive captures of frames.
        *
        * Notice: get_fps() always returns 0.0 on webcams in OpenCV,
        * so this method actually always returns 0.0.
        */
        inline const double get_period() const noexcept
        {
            try {
                return 1.0 / get_fps();
            }
            catch (...) {
                return 0.0;
            }
        }

        /** @brief Returns the ok-status of this camera. */
        inline const bool is_ok() const noexcept
        {
            return get_hw_width() != 0;
        }

        /** @brief Sets the wished size for captured frames.
        *
        * Captured frames will be resized to this size before being delivered.
        */
        inline void set_frames_size(const avt::DimsType width, const avt::DimsType height) noexcept
        {
            frame_width = width;
            frame_height = height;
        }

        /** @brief Sets the physical dimensions of captured frames (2 scalar dimensions). */
        template<typename W, typename H>
            requires std::is_arithmetic_v<W>&& std::is_arithmetic_v<H>
        void set_hw_dims(const W width, const H height) noexcept
        {
            cv_cam_handle.set(cv::CAP_PROP_FRAME_WIDTH, double(width));
            cv_cam_handle.set(cv::CAP_PROP_FRAME_HEIGHT, double(height));
            m_set_hw_size();  // this call is mandatory because passed arguments may be wrong according to the device H/W
        }

        /** @brief Sets the physical dimensions of captured frames (1 size). */
        inline void set_hw_dims(const avt::utils::Size& size) noexcept
        {
            set_hw_dims(size.width, size.height);
        }

        /** @brief Sets the physical dimensions of captured frames (1 2D-container size). */
        template<typename P>
            requires avt::is_pair_type_v<P>
        inline void set_hw_dims(const P& size) noexcept
        {
            set_hw_dims(size[0], size[1]);
        }


        //---   Attributes   ------------------------------------------------
        avt::ImageType   last_frame;        //!< a reference to the last captured frame
        cv::VideoCapture cv_cam_handle;     //!< the OpenCV handle to the related video capture instance
        int              cam_id;            //!< the identifier of this camera
        int              hw_default_height; //!< the device H/W default height of captured frames
        int              hw_default_width;  //!< the device H/W default width of captured frames
        int              frame_height;      //!< the final height of captured frames when delivered
        int              frame_width;       //!< the final width of captured frames when delivered


    private:
        /** @brief Creates this camera. */
        void m_create_camera(const int cam_id_) noexcept
        {
            cam_id = cam_id_;
            cv_cam_handle = cv::VideoCapture{ cam_id_ };
        }

        /** @brief Resizes the passe image according to previously set dimensions. */
        avt::ImageType m_resize(avt::ImageType& frame) noexcept;

        /** @brief Evaluates the H/W default size of frames that are captured by this camera. */
        void m_set_hw_size() noexcept
        {
            hw_default_width = get_hw_width();
            hw_default_height = get_hw_height();
        }
    };


    //=======================================================================
    /** @brief The class of NULL Cameras. */
    class NullCamera : public Camera
    {
    public:
        //---   Constructors / Destructors ----------------------------------
        /** @brief Constructor with set camera id.
        *
        * @arg cam_id: int
        *   The identifier of this camera. Identifiers are integers, as
        *   specified by OpenCV,  that start with index 0 (embedded web
        *   cam on laptops, for instance)  and grows up.  Providing  an
        *   index  for  which  no  hardware  video  capturing device is
        *   connected  leads to  a  faulty  camera  mode  whose  status
        *   'is_ok()'  returns  False.   Connected  cameras  get  their
        *   ok-status as being True.
        */
        inline NullCamera(const int cam_id) noexcept
            : Camera(cam_id)
        {}

        /** @brief Default Destructor. */
        virtual ~NullCamera() noexcept = default;

        /** @brief Returns the ok-status of this camera as being false. */
        inline const bool is_ok() const noexcept
        {
            return false;
        }
    };
}
