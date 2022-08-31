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
#include "devices/cameras/camera.h"
#include "utils/types.h"


export module gui.views.camera_view;

import utils.coords2d;
import gui.fonts.font;
//import devices.fps_frame_rate;
//import gui.items.label;
import utils.size;
import gui.views.view;


//===========================================================================
export namespace avt::gui::views
{
    //=======================================================================
    /** @brief The class of Camera Views.
    *
    * There are as many Camerra Views as there are cameras that
    * are connected to the PC while launching application AVT.
    */
    class CameraView : public avt::gui::views::View
    {
    public:
        //---   Wrappers   --------------------------------------------------
        using MyBaseType    = avt::gui::views::View;         //!< wrapper to the base class
        using Camera        = avt::devices::cameras::Camera;
        using Font          = avt::gui::fonts::Font;
        //using FPSFrameRate  = avt::devices::FPSFrameRate;
        //using Label         = avt::gui::items::Label;
        using ViewType      = avt::gui::views::View;


        //---   Constructors / Destructors   --------------------------------
        /** @brief Value Constructor (4 scalars + 1 color). */
        template<typename X, typename Y, typename H, typename W>
            requires std::is_arithmetic_v<X> && std::is_arithmetic_v<Y> && std::is_arithmetic_v<H> && std::is_arithmetic_v<W>
        inline CameraView(Camera&   camera,
                          ViewType* p_parent_view,
                          const X   x,
                          const Y   y,
                          const W   width,
                          const H   height) noexcept
            : MyBaseType(p_parent_view, x, y, width, height),
              camera(camera)
        {
            m_create_view();
        }

        /** @brief Value Constructor (1 pos + 1 size + 1 color). */
        inline CameraView(Camera&                     camera,
                          ViewType*                   p_parent_view,
                          const avt::utils::Coords2D& top_left,
                          const avt::utils::Size&     size) noexcept
            : MyBaseType(p_parent_view, top_left.x, top_left.y, size.width, size.height),
              camera(camera)
        {
            m_create_view();
        }

        /** @brief Value Constructor (1 rect + 1 color). */
        inline CameraView(Camera&            camera,
                          ViewType*          p_parent_view,
                          const avt::CVRect& rect) noexcept
            : MyBaseType(p_parent_view, rect.x, rect.y, rect.width, rect.height),
              camera(camera)
        {
            m_create_view();
        }


        /** @brief Deleted Copy constructor. */
        CameraView(const CameraView&) = delete;

        /** @brief Deleted Move constructor. */
        CameraView(CameraView&&) = delete;

        /** @brief Default Destructor. */
        virtual ~CameraView() noexcept = default;


        //---   Assignments   ----------------------------------------------
        /** @brief Deleted Copy assignment. */
        CameraView& operator= (const CameraView&) = delete;

        /** @brief Deleted Move assignment. */
        CameraView& operator= (CameraView&&) = delete;


        //---   Operations   ------------------------------------------------
        /** @brief Draws this view content within the parent window. */
        void draw() noexcept;

        /** @brief Draws a new camera frame within this camera view. */
        void draw_frame(avt::ImageType& camera_frame) noexcept;

        /** @brief Toggles the flipping mode of displayed captured frames. */
        inline void flip_image() noexcept
        {
            b_flip_image = !b_flip_image;
        }

        /** @brief Returns the ok-status of the associated camera. */
        inline const bool is_ok() const noexcept
        {
            return camera.is_ok();
        }


        //---   Attributes   ------------------------------------------------
        std::string   view_name;
        Camera        camera;  //!< a reference to the camera that is associated with this CameraView
        //Label         label;
        //Label         fps_label;
        //FPSFrameRate  fps_frame_rate;
        bool          b_flip_image{ false };
        


    private:
        //---   Class Attributes   ------------------------------------------
        static inline int _CAM_VIEWS_COUNT = 0;

        //---   Internal operations   ---------------------------------------
        /** @brief Internally sets this instance attributes. */
        void m_create_view() noexcept;

        /** @brief Draws the borders of this camera view. */
        void m_draw_borders() noexcept;

        /** @brief Draws the FPS rate of the associated camera. */
        void m_draw_fps() noexcept;
    };

}