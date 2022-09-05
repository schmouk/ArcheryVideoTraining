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

#include <atomic>
#include <chrono>
#include <cstring>
#include <exception>
#include <format>
#include <vector>

#include <opencv2/core/cvstd.hpp>
#include <opencv2/highgui.hpp>

#include "devices/cameras/camera.h"
#include "gui/items/icon.h"
#include "utils/types.h"


export module gui.views.control_view;

import gui.fonts.bold_font;
import devices.cameras.cameras_pool;
import avt.config;
import utils.coords2d;
import gui.items.cursor;
import gui.fonts.font;
import gui.items.label;
import gui.items.picture;
import utils.rgb_color;
import mtmp.timer;
import utils;
import gui.views.view;


//===========================================================================
export namespace avt::gui::views
{
    //=======================================================================
    /** @brief The class of Control Views.
    *
    * Control views are the views within which all graphical control
    * items are displayed for the controlling of application AVT.
    */
    class ControlView : public avt::gui::views::View, public avt::mtmp::Timer
    {
    private:
        using BoldFont    = avt::gui::fonts::BoldFont;
        using Camera      = avt::devices::cameras::Camera;
        using CamerasPool = avt::devices::cameras::CamerasPool;
        using Font        = avt::gui::fonts::Font;
        using Icon        = avt::gui::items::Icon;
        using Label       = avt::gui::items::Label;
        using RGBColor    = avt::utils::RGBColor;
        using View        = avt::gui::views::View;


    public:
        //---   Wrappers   --------------------------------------------------
        using ThreadType = avt::mtmp::Timer;
        using ViewType   = avt::gui::views::View;


        //---   Configuration constants   -----------------------------------
        static constexpr int CENTER       = -1;
        static constexpr int ICON_HEIGHT  = 40;
        static constexpr int ICON_PADDING = ICON_HEIGHT / 2;
        static constexpr int WIDTH        = 96;


        //---   Constructors / Destructors   --------------------------------
        /** @brief Value Constructor. */
        ControlView(ViewType* p_parent_view, const CamerasPool& cameras_pool) noexcept(false);

        /** @brief Deleted Copy constructor. */
        ControlView(const ControlView&) = delete;

        /** @brief Deleted Move constructor. */
        ControlView(ControlView&&) = delete;

        /** @brief Default Destructor. */
        virtual ~ControlView() noexcept = default;


        //---   Assignments   ----------------------------------------------
        /** @brief Deleted Copy assignment. */
        ControlView& operator= (const ControlView&) = delete;

        /** @brief Deleted Move assignment. */
        ControlView& operator= (ControlView&&) = delete;


        //---   Exceptions   ------------------------------------------------
        class NullParentException : public std::exception
        {
            const char* what() noexcept
            {
                return "!!! ERROR: The Control View is not attached to any parent window.";
            }
        };


        //---   Operations   ------------------------------------------------
        /** @brief Draws this view content within the parent window. */
        void draw() noexcept;

        /** @brief The processing core of this view thread. */
        virtual inline void run() noexcept override
        {
            draw();
        }


    private:
        /** @brief Internally creates all the controls that are embedded in this Control View. */
        void m_create_controls(const CamerasPool& cameras_pool) noexcept;

        /** @brief Draws lines on this view borders. */
        void m_draw_borders() noexcept;

        /** @brief Draws all controls in this control view. */
        void m_draw_controls() noexcept;


        //===================================================================
        //---   Base class for all controls types   -------------------------
        /** @brief The base class for all internal controls.
        *
        * @sa _CtrlCamera,  _CtrlDelay,   _CtrlExit,   _CtrlLines, _CtrlMatch,
        *     _Ctrl_Record, _Ctrl_Replay, _CtrlTarget, _CtrlTime,  _CtrlTimer.
        */
        class _CtrlBase
        {
        public:
            //--- Constructors/Destructors ----------------------------------
            /** @brief Value Constructor (2 coordinates). */
            template<typename X, typename Y>
                requires std::is_arithmetic_v<X> && std::is_arithmetic_v<Y>
            inline _CtrlBase(const X x_, const Y y_, const bool enabled_ = true, const bool active_ = false) noexcept
                : x{ avt::utils::clamp_s(x_) },
                  y{ avt::utils::clamp_s(y_) },
                  enabled{ enabled_ },
                  active{ active_ }
            {
                text_pos = avt::utils::Coords2D(x_, y_ + ControlView::ICON_HEIGHT + FONT_SIZE);
            }

            /** @brief Value Constructor (1 position). */
            inline _CtrlBase(const avt::utils::Coords2D& pos, const bool enabled_ = true, const bool active_ = false) noexcept
                : x{ pos.x },
                  y{ pos.y },
                  enabled{ enabled_ },
                  active{ active_ }
            {
                text_pos = avt::utils::Coords2D(pos.x, pos.y + ControlView::ICON_HEIGHT + FONT_SIZE);
            }

            /** @brief Value Constructor (1 2D-container position). */
            template<typename P>
                requires avt::is_pair_type_v<P>
            inline _CtrlBase(const P& pos, const bool enabled_ = true, const bool active_ = false) noexcept
                : x{ pos[0] },
                  y{ pos[1]},
                  enabled{ enabled_ },
                  active{ active_ }
            {
                text_pos = avt::utils::Coords2D(pos[0], pos[1] + ControlView::ICON_HEIGHT + FONT_SIZE);
            }

            /** @brief Default Constructor. */
            _CtrlBase() noexcept = default;

            /** @brief Default Destructor. */
            virtual ~_CtrlBase() noexcept = default;

            //--- Drawing operation -----------------------------------------
            /** @brief Draws a control in its embedding content.
            *
            * This method SHOULD BE overwritten in inheriting classes.
            */
            virtual void draw(View& view_image) noexcept;

            //--- Attributes ------------------------------------------------
            avt::utils::Coords2D text_pos;  //!< Position of text associated with this control
            avt::CoordsType x;              //!< left-most position on X-axis of this control
            avt::CoordsType y;              //!< top-most position on Y-axis of this control
            bool enabled;                   //!< true if this control is currently enabled, or false otherwise
            bool active;                    //!< true if this control is currently active, or false otherwise


            //--- Class Attributes   ----------------------------------------
            static constexpr int  FONT_SIZE = 14;
            static inline Font    FONT_ACTIVE{ FONT_SIZE, RGBColor::YELLOW };
            static inline Font    FONT_DISABLED{ FONT_SIZE, RGBColor::DEEP_GRAY };
            static inline Font    FONT_ENABLED{ FONT_SIZE, RGBColor::LIGHT_GRAY };
        };

        //===================================================================
        //---   Class for Camera controls   ---------------------------------
        /** @brief Manages all the controls related to cameras interfacing. */
        class _CtrlCamera : public _CtrlBase
        {
        public:
            //--- Constructors/Destructors ----------------------------------
            /** @brief Value Constructor (2 coordinates). */
            template<typename X, typename Y>
                requires std::is_arithmetic_v<X>&& std::is_arithmetic_v<Y>
            inline _CtrlCamera( Camera& camera, const X x, const Y y) noexcept
                : camera{ camera },
                  _CtrlBase(x != ControlView::CENTER ? x : (ControlView::WIDTH - _CtrlCamera::WIDTH) / 2, y)
            {
                is_on = camera.is_ok();
            }

            /** @brief Value Constructor (1 position). */
            inline _CtrlCamera( Camera& camera, const avt::utils::Coords2D& pos) noexcept
                : camera{ camera },
                  _CtrlBase{ pos }
            {
                is_on = camera.is_ok();
            }

            /** @brief Value Constructor (1 2D-container position). */
            template<typename P>
                requires avt::is_pair_type_v<P>
            inline _CtrlCamera( Camera& camera, const P& pos) noexcept
                : camera{ camera },
                  _CtrlBase{ pos }
            {
                is_on = camera.is_ok();
            }

            /** @brief Default Constructor. */
            _CtrlCamera() noexcept = default;

            /** @brief Default Destructor. */
            virtual ~_CtrlCamera() noexcept = default;

            //--- Drawing operation -----------------------------------------
            /** @brief Draws a control in its embedding content. */
            virtual void draw(View& view_image) noexcept;

            //--- Other operations ------------------------------------------
            inline void toggle_switch() noexcept
            {
                is_on = !is_on;
            }

            //--- Attributes ------------------------------------------------
            Camera camera;         //!< reference to the related camera
            bool   is_on{ true };  //!< true if this camera control-switch os ON, or false if it is OFF


            //--- Class Attributes   ----------------------------------------
            static inline BoldFont FONT_NOT_OK{ 13, RGBColor::ANTHRACITE };
            static inline BoldFont FONT_OFF{ 13, RGBColor::GRAY };
            static inline BoldFont FONT_ON{ 13, RGBColor::YELLOW };
            static inline Icon     ICON_OFF{ "controls/switch-off.png" };
            static inline Icon     ICON_ON{ "controls/switch-on.png" };
            static inline Icon     ICON_DISABLED{ "controls/switch-disabled.png" };
            static inline int WIDTH  = ICON_ON.width();
            static inline int HEIGHT = ICON_ON.height();
        };

        //===================================================================
        //---   Class for the Delay Control   -------------------------------
        /** @brief Manages the delay control. */
        class _CtrlDelay : public _CtrlBase
        {
        public:
            //--- Constructors/Destructors ----------------------------------
            /** @brief Value Constructor (2 coordinates). */
            template<typename X, typename Y>
                requires std::is_arithmetic_v<X> && std::is_arithmetic_v<Y>
            inline _CtrlDelay(const X x_, const Y y_, const bool enabled = true, const bool active = false) noexcept
                : _CtrlBase{ x_, y_, enabled, active }
            {
                m_create_slider(x, y);  // remember: x and y are base class attributes
            }

            /** @brief Value Constructor (1 position). */
            inline _CtrlDelay(const avt::utils::Coords2D& pos, const bool enabled = true, const bool active = false) noexcept
                : _CtrlBase{ pos, enabled, active }
            {
                m_create_slider(x, y);  // remember: x and y are base class attributes
            }

            /** @brief Value Constructor (1 2D-container position). */
            template<typename P>
                requires avt::is_pair_type_v<P>
            inline _CtrlDelay( /*avt::cameras::Camera& camera, */const P& pos) noexcept
                : _CtrlBase{ pos, enabled, active }
            {
                m_create_slider(x, y);  // remember: x and y are base class attributes
            }

            /** @brief Default Constructor. */
            _CtrlDelay() noexcept = default;

            /** @brief Default Destructor. */
            virtual ~_CtrlDelay() noexcept = default;

            //--- Drawing operation -----------------------------------------
            /** @brief Draws a control in its embedding content. */
            virtual void draw(View& view_image) noexcept;

            //--- Attributes ------------------------------------------------
            //avt::gui::items::Slider slider;


            //--- Class Attributes   ----------------------------------------
            static inline Icon   ICON_OFF{ "controls/delay-off.png" };
            static inline Icon   ICON_ON{ "controls/delay-on.png" };
            static inline Icon   ICON_DISABLED{ "controls/delay-disabled.png" };
            static inline int    SIZE = ICON_ON.width();
            static constexpr int TICKS_FONT_SIZE = 8;
            static inline Font   TICKS_FONT_ENABLED{ TICKS_FONT_SIZE, RGBColor::YELLOW / 1.33 };


        private:
            /** @brief Creates the associated slider. */
            void m_create_slider(const avt::CoordsType x, const avt::CoordsType y) noexcept;
        };

        //===================================================================
        //---   Class for the Exit Control   --------------------------------
        /** @brief Manages the Exit control. */
        class _CtrlExit : public _CtrlBase
        {
        public:
            //--- Constructors/Destructors ----------------------------------
            /** @brief Value Constructor. */
            _CtrlExit(const int view_width, const int view_height) noexcept;

            /** @brief Default Constructor. */
            _CtrlExit() noexcept = default;

            /** @brief Default Destructor. */
            virtual ~_CtrlExit() noexcept = default;

            //--- Drawing operation -----------------------------------------
            /** @brief Draws a control in its embedding content. */
            void draw(View& view_image) noexcept;


            //--- Class Attributes   ----------------------------------------
            static inline Icon ICON_EXIT{ "controls/exit-48.png" };
            static inline int WIDTH  = ICON_EXIT.width();
            static inline int HEIGHT = ICON_EXIT.height();
            int x;
            int y;
        };

        //===================================================================
        //---   Class for the Lines Control   --------------------------------
        /** @brief Manages the lines control. */
        class _CtrlLines : public _CtrlBase
        {
        public:
            //--- Constructors/Destructors ----------------------------------
            /** @brief Value Constructor (2 coordinates). */
            template<typename X, typename Y>
                requires std::is_arithmetic_v<X> && std::is_arithmetic_v<Y>
            inline _CtrlLines(const X x_, const Y y_, const bool enabled = true, const bool active = false) noexcept
                : _CtrlBase{ x_, y_, enabled, active }
            {}

            /** @brief Value Constructor (1 position). */
            inline _CtrlLines(const avt::utils::Coords2D& pos, const bool enabled = true, const bool active = false) noexcept
                : _CtrlBase{ pos, enabled, active }
            {}

            /** @brief Value Constructor (1 2D-coordinates position). */
            template<typename P>
                requires avt::is_pair_type_v<P>
            inline _CtrlLines(const P& pos, const bool enabled = true, const bool active = false) noexcept
                : _CtrlBase{ pos, enabled, active }
            {}

            /** @brief Default Constructor. */
            _CtrlLines() noexcept = default;

            /** @brief Default Destructor. */
            virtual ~_CtrlLines() noexcept = default;

            //--- Drawing operation -----------------------------------------
            /** @brief Draws a control in its embedding content. */
            void draw(View& view_image) noexcept;


            //--- Class Attributes   ----------------------------------------
            static constexpr int LINE_LENGTH    = 35;
            static constexpr int LINE_THICKNESS =  7;
        };

        //===================================================================
        //---   Class for the Match Control   --------------------------------
        /** @brief Manages the Match control. */
        class _CtrlMatch : public _CtrlBase
        {
        public:
            //--- Constructors/Destructors ----------------------------------
            /** @brief Value Constructor (2 coordinates). */
            template<typename X, typename Y>
                requires std::is_arithmetic_v<X>&& std::is_arithmetic_v<Y>
            inline _CtrlMatch(const X x_, const Y y_, const bool enabled = true, const bool active = false) noexcept
                : _CtrlBase{ x_, y_, enabled, active }
            {}

            /** @brief Value Constructor (1 position). */
            inline _CtrlMatch(const avt::utils::Coords2D& pos, const bool enabled = true, const bool active = false) noexcept
                : _CtrlBase{ pos, enabled, active }
            {}

            /** @brief Value Constructor (1 2D-coordinates position). */
            template<typename P>
                requires avt::is_pair_type_v<P>
            inline _CtrlMatch(const P& pos, const bool enabled = true, const bool active = false) noexcept
                : _CtrlBase{ pos, enabled, active }
            {}

            /** @brief Default Constructor. */
            _CtrlMatch() noexcept = default;

            /** @brief Default Destructor. */
            virtual ~_CtrlMatch() noexcept = default;

            //--- Drawing operation -----------------------------------------
            /** @brief Draws a control in its embedding content. */
            void draw(View& view_image) noexcept;


            //--- Class Attributes   ----------------------------------------
            static inline Icon ICON_OFF{ "controls/match-off.png" };
            static inline Icon ICON_ON{ "controls/match-on.png" };
            static inline Icon ICON_DISABLED{ "controls/match-disabled.png" };
            static inline int  SIZE = ICON_ON.width();
        };

        //===================================================================
        //---   Class for the Overlays Control   --------------------------------
        /** @brief Manages the Overlays control. */
        class _CtrlOverlays : public _CtrlBase
        {
        public:
            //--- Constructors/Destructors ----------------------------------
            /** @brief Value Constructor (2 coordinates). */
            template<typename X, typename Y>
                requires std::is_arithmetic_v<X>&& std::is_arithmetic_v<Y>
            inline _CtrlOverlays(const X x_, const Y y_, const bool enabled = true, const bool active = false) noexcept
                : _CtrlBase{ x_, y_, enabled, active }
            {}

            /** @brief Value Constructor (1 position). */
            inline _CtrlOverlays(const avt::utils::Coords2D& pos, const bool enabled = true, const bool active = false) noexcept
                : _CtrlBase{ pos, enabled, active }
            {}

            /** @brief Value Constructor (1 2D-container position). */
            template<typename P>
                requires avt::is_pair_type_v<P>
            inline _CtrlOverlays(const P& pos, const bool enabled = true, const bool active = false) noexcept
                : _CtrlBase{ pos, enabled, active }
            {}

            /** @brief Default Constructor. */
            _CtrlOverlays() noexcept = default;

            /** @brief Default Destructor. */
            virtual ~_CtrlOverlays() noexcept = default;

            //--- Drawing operation -----------------------------------------
            /** @brief Draws a control in its embedding content. */
            void draw(View& view_image) noexcept;


            //--- Class Attributes   ----------------------------------------
            static inline Icon ICON_OFF{ "controls/overlays-off.png" };
            static inline Icon ICON_ON{ "controls/overlays-on.png" };
            static inline Icon ICON_DISABLED{ "controls/overlays-disabled.png" };
            static inline int  SIZE = ICON_ON.width();
        };

        //===================================================================
        //---   Class for the Record Control   --------------------------------
        /** @brief Manages the Record control. */
        class _CtrlRecord : public _CtrlBase
        {
        public:
            //--- Constructors/Destructors ----------------------------------
            /** @brief Value Constructor (2 coordinates). */
            template<typename X, typename Y>
                requires std::is_arithmetic_v<X>&& std::is_arithmetic_v<Y>
            inline _CtrlRecord(const X x_, const Y y_, const bool enabled = true, const bool active = false) noexcept
                : _CtrlBase{ x_, y_, enabled, active }
            {
                m_create_slider();
            }

            /** @brief Value Constructor (1 position). */
            inline _CtrlRecord(const avt::utils::Coords2D& pos, const bool enabled = true, const bool active = false) noexcept
                : _CtrlBase{ pos, enabled, active }
            {
                m_create_slider();
            }

            /** @brief Value Constructor (1 2D-container position). */
            template<typename P>
                requires avt::is_pair_type_v<P>
            inline _CtrlRecord(const P& pos, const bool enabled = true, const bool active = false) noexcept
                : _CtrlBase{ pos, enabled, active }
            {
                m_create_slider();
            }

            /** @brief Default Constructor. */
            _CtrlRecord() noexcept = default;

            /** @brief Default Destructor. */
            virtual ~_CtrlRecord() noexcept = default;

            //--- Drawing operation -----------------------------------------
            /** @brief Draws a control in its embedding content. */
            void draw(View& view_image) noexcept;

            //--- Attributes ------------------------------------------------
            //FloatSlider slider{};


            //--- Class Attributes   ----------------------------------------
            static inline Icon   ICON_OFF{ "controls/record-off.png" };
            static inline Icon   ICON_ON{ "controls/record-on.png" };
            static inline Icon   ICON_DISABLED{ "controls/record-disabled.png" };
            static inline int    ICON_SIZE = ICON_ON.width();
            static constexpr int FONT_3_SIZE = 8;
            static constexpr int FONT_2_SIZE = 11;
            static inline Font   FONT_3_DISABLED   { FONT_3_SIZE, RGBColor::GRAY };
            static inline Font   FONT_3_OFF        { FONT_3_SIZE, RGBColor::LIGHT_GRAY };
            static inline Font   FONT_3_ON         { FONT_3_SIZE, RGBColor::YELLOW };
            static inline Font   FONT_2_DISABLED   { FONT_2_SIZE, RGBColor::GRAY };
            static inline Font   FONT_2_OFF        { FONT_2_SIZE, RGBColor::LIGHT_GRAY };
            static inline Font   FONT_2_ON         { FONT_2_SIZE, RGBColor::YELLOW };
            static constexpr int TICKS_FONT_SIZE = 8;
            static inline Font   TICKS_FONT_ENABLED{ TICKS_FONT_SIZE, RGBColor::YELLOW / 1.33 };


        private:
            /** @brief Creates the associated slider. */
            void m_create_slider() noexcept;
        };

        //===================================================================
        //---   Class for the _CtrlReplay Control   --------------------------------
        /** @brief Manages the Replay control. */
        class _CtrlReplay : public _CtrlBase
        {
        public:
            //--- Constructors/Destructors ----------------------------------
            /** @brief Value Constructor (2 coordinates). */
            template<typename X, typename Y>
                requires std::is_arithmetic_v<X>&& std::is_arithmetic_v<Y>
            inline _CtrlReplay(const X x_, const Y y_, const bool enabled = true, const bool active = false) noexcept
                : _CtrlBase{ x_, y_, enabled, active }
            {}

            /** @brief Value Constructor (1 position). */
            inline _CtrlReplay(const avt::utils::Coords2D& pos, const bool enabled = true, const bool active = false) noexcept
                : _CtrlBase{ pos, enabled, active }
            {}

            /** @brief Value Constructor (1 2D-container position). */
            template<typename P>
                requires avt::is_pair_type_v<P>
            inline _CtrlReplay(const P& pos, const bool enabled = true, const bool active = false) noexcept
                : _CtrlBase{ pos, enabled, active }
            {}

            /** @brief Default Constructor. */
            _CtrlReplay() noexcept = default;

            /** @brief Default Destructor. */
            virtual ~_CtrlReplay() noexcept = default;

            //--- Drawing operation -----------------------------------------
            /** @brief Draws a control in its embedding content. */
            void draw(View& view_image) noexcept;


            //--- Class Attributes   ----------------------------------------
            static inline Icon ICON_FBW_OFF{ "controls/fbw-25-off.png" };
            static inline Icon ICON_FBW_ON{ "controls/fbw-25-on.png" };
            static inline Icon ICON_FBW_DISABLED{ "controls/fbw-25-disabled.png" };

            static inline Icon ICON_FFW_OFF{ "controls/ffw-25-off.png" };
            static inline Icon ICON_FFW_ON{ "controls/ffw-25-on.png" };
            static inline Icon ICON_FFW_DISABLED{ "controls/ffw-25-disabled.png" };

            static inline Icon ICON_PAUSE_OFF{ "controls/pause-25-off.png" };
            static inline Icon ICON_PAUSE_ON{ "controls/pause-25-on.png" };
            static inline Icon ICON_PAUSE_DISABLED{ "controls/pause-25-disabled.png" };

            static inline Icon ICON_PLAY_OFF{ "controls/play-25-off.png" };
            static inline Icon ICON_PLAY_ON{ "controls/play-25-on.png" };
            static inline Icon ICON_PLAY_DISABLED{ "controls/play-25-disabled.png" };

            static inline Icon ICON_STEP_BW_OFF{ "controls/step-bw-25-off.png" };
            static inline Icon ICON_STEP_BW_ON{ "controls/step-bw-25-on.png" };
            static inline Icon ICON_STEP_BW_DISABLED{ "controls/step-bw-25-disabled.png" };

            static inline Icon ICON_STEP_FW_OFF{ "controls/step-fw-25-off.png" };
            static inline Icon ICON_STEP_FW_ON{ "controls/step-fw-25-on.png" };
            static inline Icon ICON_STEP_FW_DISABLED{ "controls/step-fw-25-disabled.png" };

            static inline int SIZE = 25; //_ICON_PLAY_ON.rows;
        };

        //===================================================================
        //---   Class for the Target Control   --------------------------------
        /** @brief Manages the Target control. */
        class _CtrlTarget : public _CtrlBase
        {
        public:
            //--- Constructors/Destructors ----------------------------------
            /** @brief Value Constructor (2 coordinates). */
            template<typename X, typename Y>
                requires std::is_arithmetic_v<X>&& std::is_arithmetic_v<Y>
            inline _CtrlTarget(const X x_, const Y y_, const bool enabled = true, const bool active = false) noexcept
                : _CtrlBase{ x_, y_, enabled, active }
            {}

            /** @brief Value Constructor (1 position). */
            inline _CtrlTarget(const avt::utils::Coords2D& pos, const bool enabled = true, const bool active = false) noexcept
                : _CtrlBase{ pos, enabled, active }
            {}

            /** @brief Value Constructor (1 2D-container position). */
            template<typename P>
                requires avt::is_pair_type_v<P>
            inline _CtrlTarget(const P& pos, const bool enabled = true, const bool active = false) noexcept
                : _CtrlBase{ pos, enabled, active }
            {}

            /** @brief Default Constructor. */
            _CtrlTarget() noexcept = default;

            /** @brief Default Destructor. */
            virtual ~_CtrlTarget() noexcept = default;

            //--- Drawing operation -----------------------------------------
            /** @brief Draws a control in its embedding content. */
            void draw(View& view_image) noexcept;


            //--- Class Attributes   ----------------------------------------
            static inline Icon ICON_OFF{ "controls/target-off.png" };
            static inline Icon ICON_ON{ "controls/target-on.png" };
            static inline Icon ICON_DISABLED{ "controls/target-disabled.png" };
            static inline int  SIZE = ICON_ON.width();
        };

        //===================================================================
        //---   Class for the Time Control   --------------------------------
        /** @brief Manages the Time control. */
        class _CtrlTime : public _CtrlBase
        {
        public:
            //--- Constructors/Destructors ----------------------------------
            /** @brief Value Constructor (2 coordinates). */
            template<typename X, typename Y>
                requires std::is_arithmetic_v<X>&& std::is_arithmetic_v<Y>
            inline _CtrlTime(const X x_, const Y y_, const bool enabled = true, const bool active = false) noexcept
                : _CtrlBase{ x_, y_, false, true }
            {
                m_create_labels();
            }

            /** @brief Value Constructor (1 position). */
            inline _CtrlTime(const avt::utils::Coords2D& pos, const bool enabled = true, const bool active = false) noexcept
                : _CtrlBase{ pos, false, true }
            {
                m_create_labels();
            }

            /** @brief Value Constructor (1 2D-container position). */
            template<typename P>
                requires avt::is_pair_type_v<P>
            inline _CtrlTime(const P& pos, const bool enabled = true, const bool active = false) noexcept
                : _CtrlBase{ pos, enabled, active }
            {
                m_create_labels();
            }

            /** @brief Default Constructor. */
            _CtrlTime() noexcept = default;

            /** @brief Default Destructor. */
            virtual ~_CtrlTime() noexcept = default;

            //--- Drawing operation -----------------------------------------
            /** @brief Draws a control in its embedding content. */
            void draw(View& view_image) noexcept;


            //--- Class Attributes   ----------------------------------------
            using _clock = std::chrono::steady_clock;

            static constexpr int   DURATION_TEXT_SIZE   { 11 };
            static constexpr int   PADDING              { 11 };
            static constexpr int   TIME_TEXT_SIZE       { 15 };
            static constexpr int   FULL_HEIGHT          { TIME_TEXT_SIZE + PADDING + DURATION_TEXT_SIZE };
            static inline Font     DURATION_FONT        { DURATION_TEXT_SIZE, RGBColor::YELLOW };
            static inline BoldFont TIME_FONT            { TIME_TEXT_SIZE, RGBColor::YELLOW - 32 };

            static inline Label    duration_label       {};
            static inline Label    time_label           {};

            static inline _clock::time_point  start_time{ _clock::now() };


        private:
            /** @brief Creates the labels that are associated with the Time control. */
            void m_create_labels() noexcept;
        };

        //===================================================================
        //---   Class for the Timer Control   --------------------------------
        /** @brief Manages the Timer control. */
        class _CtrlTimer : public _CtrlBase
        {
        public:
            //--- Constructors/Destructors ----------------------------------
            /** @brief Value Constructor (2 coordinates). */
            template<typename X, typename Y>
                requires std::is_arithmetic_v<X>&& std::is_arithmetic_v<Y>
            inline _CtrlTimer(const X x_, const Y y_, const bool enabled = true, const bool active = false) noexcept
                : _CtrlBase{ x_, y_, enabled, active }
            {}

            /** @brief Value Constructor (1 position). */
            inline _CtrlTimer(const avt::utils::Coords2D& pos, const bool enabled = true, const bool active = false) noexcept
                : _CtrlBase{ pos, enabled, active }
            {}

            /** @brief Value Constructor (1 2D-container position). */
            template<typename P>
                requires avt::is_pair_type_v<P>
            inline _CtrlTimer(const P& pos, const bool enabled = true, const bool active = false) noexcept
                : _CtrlBase{ pos, enabled, active }
            {}

            /** @brief Default Constructor. */
            _CtrlTimer() noexcept = default;

            /** @brief Default Destructor. */
            virtual ~_CtrlTimer() noexcept = default;

            //--- Drawing operation -----------------------------------------
            /** @brief Draws a control in its embedding content. */
            void draw(View& view_image) noexcept;


            //--- Class Attributes   ----------------------------------------
            static inline Icon ICON_OFF{ "controls/timer-off.png" };
            static inline Icon ICON_ON{ "controls/timer-on.png" };
            static inline Icon ICON_DISABLED{ "controls/timer-disabled.png" };
            static inline int  SIZE = ICON_ON.width();
        };


        //---   Attributes   ------------------------------------------------
        std::vector<_CtrlCamera> m_cameras_ctrls;
        std::vector<_CtrlBase>   m_controls_list;
        _CtrlDelay               m_delay_ctrl;
        _CtrlExit                m_exit_ctrl;
        _CtrlLines               m_lines_ctrl;
        _CtrlMatch               m_match_ctrl;
        _CtrlOverlays            m_overlays_ctrl;
        _CtrlRecord              m_record_ctrl;
        _CtrlReplay              m_replay_ctrl;
        _CtrlTarget              m_target_ctrl;
        _CtrlTime                m_time_ctrl;
        _CtrlTimer               m_timer_ctrl;

    };

}