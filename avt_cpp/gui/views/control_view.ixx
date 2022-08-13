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
#include <cstring>
#include <exception>
#include <format>

#include <opencv2/core/cvstd.hpp>
#include <opencv2/highgui.hpp>

#include "gui/views/view.h"


export module gui.views.control_view;

import gui.fonts.bold_font;
import avt.config;
import utils.coords2d;
import gui.items.cursor;
import gui.fonts.font;
import mtmp.mutex;
import utils.rgb_color;
import utils.size;
import mtmp.timer;


//===========================================================================
export namespace avt::gui::views
{
    //=======================================================================
    /** @brief The class of Control Views.
    *
    * Control views are the views within which all graphical control items
    * are displayed for the controlling of application AVT.
    */
    class ControlView : public avt::gui::views::View, public avt::mtmp::Timer
    {
    private:
        using RGBColor = avt::utils::RGBColor;      //!< internal wrapper to the class of colors.
        using Font     = avt::gui::fonts::Font;     //!< internal wrapper to the class of fonts.
        using BoldFont = avt::gui::fonts::BoldFont; //!< internal wrapper to the class of bolded fonts.


    public:
        //---   Wrappers   --------------------------------------------------
        using ThreadType = avt::mtmp::Timer;
        using ViewType   = avt::gui::views::View;


        //---   Configuration constants   -----------------------------------
        static constexpr int ICON_HEIGHT  = 40;
        static constexpr int ICON_PADDING = ICON_HEIGHT / 2;
        static constexpr int WIDTH        = 96;


        //---   Constructors / Destructors   --------------------------------
        /** @brief Value Constructor. */
        ControlView(ViewType* p_parent_view) noexcept(false);  //, const avt::cameras::CamerasPool& cameras_pool) noexcept(false);

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
        class NullParentException
        {
            const char* what()
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
        void m_create_controls() noexcept;  // (const avt::cameras::CamerasPool& cameras_pool) noexcept;

        /** @brief Draws lines on this view borders. */
        void m_draw_borders() noexcept;

        /** @brief Draws all controls in this control view. */
        void m_draw_controls() noexcept;

        //===================================================================
        //---   Base class for all controls types   -------------------------
        /** @brief The base class for all internal controls.
        *
        * \sa _CtrlCamera,  _CtrlDelay,   _CtrlExit,   _CtrlLines, _CtrlMatch,
        *     _Ctrl_Record, _Ctrl_Replay, _CtrlTarget, _CtrlTime,  _CtrlTimer.
        */
        class _CtrlBase
        {
        public:
            //--- Constructors/Destructors ----------------------------------
            /** @brief Value Constructor (2 coordinates). */
            template<typename X, typename Y>
                requires std::is_arithmetic_v<X> && std::is_arithmetic_v<Y>
            _CtrlBase(const X x_, const Y y_, const bool enabled_ = true, const bool active_ = false) noexcept
                : x{ avt::utils::clamp_s(x_) },
                  y{ avt::utils::clamp_s(y_) },
                  enabled{ enabled_ },
                  active{ active_ }
            {
                text_pos = avt::utils::Coords2D(x_, y_ + ControlView::ICON_HEIGHT + _FONT_SIZE);
            }

            /** @brief Value Constructor (1 position). */
            _CtrlBase(const avt::utils::Coords2D& pos, const bool enabled_ = true, const bool active_ = false) noexcept
                : x{ pos.x },
                  y{ pos.y },
                  enabled{ enabled_ },
                  active{ active_ }
            {
                text_pos = avt::utils::Coords2D(pos.x, pos.y + ControlView::ICON_HEIGHT + _FONT_SIZE);
            }

            /** @brief Default Empty Constructor. */
            _CtrlBase() noexcept = default;

            /** @brief Default Destructor. */
            virtual ~_CtrlBase() noexcept = default;

            //--- Drawing operation -----------------------------------------
            /** @brief Draws a control in its embedding content.
            *
            * This method SHOULD BE overwritten in inheriting classes.
            */
            virtual void draw(avt::ImageType& image) noexcept;

            //--- Attributes ------------------------------------------------
            avt::utils::Coords2D text_pos;  //!< Position of text associated with this control
            avt::CoordsType x;              //!< left-most position on X-axis of this control
            avt::CoordsType y;              //!< top-most position on Y-axis of this control
            bool enabled;                   //!< true if this control is currently enabled, or false otherwise
            bool active;                    //!< true if this control is currently active, or false otherwise


        protected:
            static constexpr int  _FONT_SIZE = 14;
            static inline Font    _FONT_ACTIVE{ _FONT_SIZE, RGBColor::YELLOW };
            static inline Font    _FONT_DISABLED{ _FONT_SIZE, RGBColor::DEEP_GRAY };
            static inline Font    _FONT_ENABLED{ _FONT_SIZE, RGBColor::LIGHT_GRAY };
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
            _CtrlCamera( /*avt::cameras::Camera& camera, */const X x, const Y y)
                : //camera{ camera },
                  _CtrlBase{ x, y }
            {
                //is_on = camera.is_ok();
            }

            /** @brief Value Constructor (1 position). */
            _CtrlCamera( /*avt::cameras::Camera& camera, */const avt::utils::Coords2D& pos)
                : //camera{ camera },
                  _CtrlBase{ pos }
            {
                //is_on = camera.is_ok();
            }

            /** @brief Default Destructor. */
            virtual ~_CtrlCamera() noexcept = default;

            //--- Drawing operation -----------------------------------------
            /** @brief Draws a control in its embedding content. */
            virtual void draw(avt::ImageType& image) noexcept;

            //--- Other operations ------------------------------------------
            inline void toggle_switch() noexcept
            {
                //is_on = !is_on;
            }

            //--- Attributes ------------------------------------------------
            //avt::cameras::Camera camera;  //!< reference to the related camera
            //bool is_on;                   //!< true if this camera control-switch os ON, or false if it is OFF


        protected:
            static inline BoldFont _FONT_NOT_OK{ 13, RGBColor::ANTHRACITE };
            static inline BoldFont _FONT_OFF{ 13, RGBColor::GRAY };
            static inline BoldFont _FONT_ON{ 13, RGBColor::YELLOW };
            //static inline avt::Image _ICON_OFF = cv2.imread('../picts/controls/switch-off.png');
            //static inline avt::Image _ICON_ON = cv2.imread('../picts/controls/switch-on.png');
            //static inline avt::Image _ICON_DISABLED = cv2.imread('../picts/controls/switch-disabled.png');
            //static const int WIDTH = _ICON_ON.cols;
            //static const int HEIGHT = _ICON_ON.rows;
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

            /** @brief Default Destructor. */
            virtual ~_CtrlDelay() noexcept = default;

            //--- Drawing operation -----------------------------------------
            /** @brief Draws a control in its embedding content. */
            virtual void draw(avt::ImageType& image) noexcept;

            //--- Attributes ------------------------------------------------
            //avt::gui::items::Slider slider;


        protected:
            //static inline avt::Image _ICON_DISABLED = cv2.imread('../picts/controls/delay-disabled.png');
            //static inline avt::Image _ICON_OFF = cv2.imread('../picts/controls/delay-off.png');
            //static inline avt::Image _ICON_ON = cv2.imread('../picts/controls/delay-on.png');
            //static inline int _SIZE = _ICON_ON.rows;
            static constexpr int _TICKS_FONT_SIZE = 8;
            static inline Font   _TICKS_FONT_ENABLED{ _TICKS_FONT_SIZE, RGBColor::YELLOW / 1.33 };


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

            /** @brief Default Destructor. */
            virtual ~_CtrlExit() noexcept = default;

            //--- Drawing operation -----------------------------------------
            /** @brief Draws a control in its embedding content. */
            void draw(avt::ImageType& image) noexcept;


        protected:
            //static inline avt::Image _ICON_EXIT = cv2.imread( '../picts/controls/exit-48.png' );
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

            /** @brief Default Destructor. */
            virtual ~_CtrlLines() noexcept = default;

            //--- Drawing operation -----------------------------------------
            /** @brief Draws a control in its embedding content. */
            void draw(avt::ImageType& image) noexcept;


        protected:
            static constexpr int _LINE_LENGTH = 35;
            static constexpr int _LINE_THICKNESS = 7;
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

            /** @brief Default Destructor. */
            virtual ~_CtrlMatch() noexcept = default;

            //--- Drawing operation -----------------------------------------
            /** @brief Draws a control in its embedding content. */
            void draw(avt::ImageType& image) noexcept;


        protected:
            //static inline avt::Image _ICON_DISABLED = cv2.imread('../picts/controls/match-disabled.png');
            //static inline avt::Image _ICON_OFF = cv2.imread('../picts/controls/match-off.png');
            //static inline avt::Image _ICON_ON = cv2.imread('../picts/controls/match-on.png');
            //static inline int _SIZE = _ICON_ON.rows;
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

            /** @brief Default Destructor. */
            virtual ~_CtrlOverlays() noexcept = default;

            //--- Drawing operation -----------------------------------------
            /** @brief Draws a control in its embedding content. */
            void draw(avt::ImageType& image) noexcept;


        protected:
            //static inline avt::Image _ICON_DISABLED = cv2.imread('../picts/controls/overlays-disabled.png');
            //static inline avt::Image _ICON_OFF = cv2.imread('../picts/controls/overlays-off.png');
            //static inline avt::Image _ICON_ON = cv2.imread('../picts/controls/overlays-on.png');
            //static inline int _SIZE = _ICON_ON.rows;
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

            /** @brief Default Destructor. */
            virtual ~_CtrlRecord() noexcept = default;

            //--- Drawing operation -----------------------------------------
            /** @brief Draws a control in its embedding content. */
            void draw(avt::ImageType& image) noexcept;

            //--- Attributes ------------------------------------------------
            //FloatSlider slider{};


        protected:
            //static inline avt::Image _ICON_DISABLED = cv2.imread('../picts/controls/record-disabled.png');
            //static inline avt::Image _ICON_OFF = cv2.imread('../picts/controls/record-off.png');
            //static inline avt::Image _ICON_ON = cv2.imread('../picts/controls/record-on.png');
            //static inline int _SIZE = _ICON_ON.rows;
            static constexpr int _FONT_3_SIZE = 8;
            static constexpr int _FONT_2_SIZE = 11;
            static inline Font   _FONT_3_DISABLED   { _FONT_3_SIZE, RGBColor::GRAY };
            static inline Font   _FONT_3_OFF        { _FONT_3_SIZE, RGBColor::LIGHT_GRAY };
            static inline Font   _FONT_3_ON         { _FONT_3_SIZE, RGBColor::YELLOW };
            static inline Font   _FONT_2_DISABLED   { _FONT_2_SIZE, RGBColor::GRAY };
            static inline Font   _FONT_2_OFF        { _FONT_2_SIZE, RGBColor::LIGHT_GRAY };
            static inline Font   _FONT_2_ON         { _FONT_2_SIZE, RGBColor::YELLOW };
            static constexpr int _TICKS_FONT_SIZE = 8;
            static inline Font   _TICKS_FONT_ENABLED{ _TICKS_FONT_SIZE, RGBColor::YELLOW / 1.33 };


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

            /** @brief Default Destructor. */
            virtual ~_CtrlReplay() noexcept = default;

            //--- Drawing operation -----------------------------------------
            /** @brief Draws a control in its embedding content. */
            void draw(avt::ImageType& image) noexcept;


        protected:
            //static inline avt::Image _ICON_FBW_DISABLED = cv2.imread('../picts/controls/fbw-25-disabled.png');
            //static inline avt::Image _ICON_FBW_OFF = cv2.imread('../picts/controls/fbw-25-off.png');
            //static inline avt::Image _ICON_FBW_ON = cv2.imread('../picts/controls/fbw-25-on.png');
            //static inline avt::Image _ICON_FFW_DISABLED = cv2.imread('../picts/controls/ffw-25-disabled.png');
            //static inline avt::Image _ICON_FFW_OFF = cv2.imread('../picts/controls/ffw-25-off.png');
            //static inline avt::Image _ICON_FFW_ON = cv2.imread('../picts/controls/ffw-25-on.png');
            //static inline avt::Image _ICON_PAUSE_DISABLED = cv2.imread('../picts/controls/pause-25-disabled.png');
            //static inline avt::Image _ICON_PAUSE_OFF = cv2.imread('../picts/controls/pause-25-off.png');
            //static inline avt::Image _ICON_PAUSE_ON = cv2.imread('../picts/controls/pause-25-on.png');
            //static inline avt::Image _ICON_PLAY_DISABLED = cv2.imread('../picts/controls/play-25-disabled.png');
            //static inline avt::Image _ICON_PLAY_OFF = cv2.imread('../picts/controls/play-25-off.png');
            //static inline avt::Image _ICON_PLAY_ON = cv2.imread('../picts/controls/play-25-on.png');
            //static inline avt::Image _ICON_STEP_BW_DISABLED = cv2.imread('../picts/controls/step-bw-25-disabled.png');
            //static inline avt::Image _ICON_STEP_BW_OFF = cv2.imread('../picts/controls/step-bw-25-off.png');
            //static inline avt::Image _ICON_STEP_BW_ON = cv2.imread('../picts/controls/step-bw-25-on.png');
            //static inline avt::Image _ICON_STEP_FW_DISABLED = cv2.imread('../picts/controls/step-fw-25-disabled.png');
            //static inline avt::Image _ICON_STEP_FW_OFF = cv2.imread('../picts/controls/step-fw-25-off.png');
            //static inline avt::Image _ICON_STEP_FW_ON = cv2.imread('../picts/controls/step-fw-25-on.png');
            static inline int _SIZE = 25; //_ICON_PLAY_ON.rows;
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

            /** @brief Default Destructor. */
            virtual ~_CtrlTarget() noexcept = default;

            //--- Drawing operation -----------------------------------------
            /** @brief Draws a control in its embedding content. */
            void draw(avt::ImageType& image) noexcept;


        protected:
            //static inline avt::Image _ICON_DISABLED = cv2.imread('../picts/controls/target-disabled.png');
            //static inline avt::Image _ICON_OFF = cv2.imread('../picts/controls/target-off.png');
            //static inline avt::Image _ICON_ON = cv2.imread('../picts/controls/target-on.png');
            //static inline int _SIZE = _ICON_ON.rows;
        };






        /** /


    #-------------------------------------------------------------------------
    class _CtrlTarget( _CtrlBase ):
        '''The target control.
        '''
        #---------------------------------------------------------------------
        def draw(self, view: View) -> None:
            '''Draws a control in its embedding content.
            Args:
                view: View
                    A reference to the embedding view.
            '''
            x = (ControlView.WIDTH  - self._SIZE) // 2
            y = self.y + (ControlView.ICON_HEIGHT - self._SIZE) // 2
            if self.enabled:
                img = self._ICON_ACTIVE if self.is_active else self._ICON_INACTIVE
            else:
                img = self._ICON_DISABLED
            view.content[ y:y+self._SIZE, x:x+self._SIZE, : ] = img[ :, :, : ]

        #---------------------------------------------------------------------
        _ICON_ACTIVE = cv2.imread( '../picts/controls/target-on.png' )
        _ICON_INACTIVE = cv2.imread( '../picts/controls/target-off.png' )
        _ICON_DISABLED = cv2.imread( '../picts/controls/target-disabled.png' )
        _SIZE          = _ICON_ACTIVE.shape[0]


    #-------------------------------------------------------------------------
    class _CtrlTime( _CtrlBase ):
        '''The time and session duration control.
        '''
        #---------------------------------------------------------------------
        def __init__(self, x:int, y: int) -> None:
            '''Constructor.

            Args:
                x, y: top-left position of this control.
            '''
            super().__init__( x, y, False, True )
            self.time_label = Label( x=x,
                                     y=y+self._TIME_TEXT_SIZE,
                                     text_font=Font(self._TIME_TEXT_SIZE, YELLOW-32, bold=True) )
            self.duration_label = Label( x=x,
                                         y=y+self._FULL_HEIGHT,
                                         text_font=Font(self._DURATION_TEXT_SIZE, YELLOW) )

        #---------------------------------------------------------------------
        def draw(self, view: View) -> None:
            '''Draws a control in its embedding content.
            Args:
                view: View
                    A reference to the embedding view.
            '''
            date = time.localtime()
            self.time_label.text = f"{date.tm_hour:02d}:{date.tm_min:02d}"
            time_label_width = self.time_label.get_text_width()

            duration = time.perf_counter()
            hr = int( duration // 3600 )
            mn = int( (duration - 3600 * hr) // 60 )
            sc = int( duration % 60 )
            self.duration_label.text = f"({hr:d}:{mn:02d}:{sc:02d})"
            duration_label_width = self.duration_label.get_text_width()

            cv2.rectangle( view.content,
                           (self.x, self.y-3),
                           (view.width-self.x-2, self.y+self._FULL_HEIGHT + 3),
                           AVTConfig.DEFAULT_BACKGROUND.color,
                           -1 )

            self.time_label.draw_at( (view.width - time_label_width) // 2,
                                     self.time_label.pos.y,
                                     view )
            self.duration_label.draw_at( (view.width - duration_label_width) // 2,
                                          self.duration_label.pos.y,
                                          view )

        #---------------------------------------------------------------------
        _DURATION_TEXT_SIZE = 11
        _PADDING            = 11
        _TIME_TEXT_SIZE     = 15
        _FULL_HEIGHT        = _TIME_TEXT_SIZE + _PADDING + _DURATION_TEXT_SIZE


    #-------------------------------------------------------------------------
    class _CtrlTimer( _CtrlBase ):
        '''The timer control.
        '''
        #---------------------------------------------------------------------
        def draw(self, view: View) -> None:
            '''Draws a control in its embedding content.
            Args:
                view: View
                    A reference to the embedding view.
            '''
            x = (view.WIDTH - self._SIZE) // 2
            y = self.y + 1
            if self.enabled:
                img = self._ICON_ON if self.is_active else self._ICON_OFF
            else:
                img = self._ICON_DISABLED
            view.content[ y:y+self._SIZE, x:x+self._SIZE, : ] = img[ :, :, : ]

        #---------------------------------------------------------------------
        _ICON_DISABLED = cv2.imread( '../picts/controls/timer-disabled.png' )
        _ICON_OFF      = cv2.imread( '../picts/controls/timer-off.png' )
        _ICON_ON       = cv2.imread( '../picts/controls/timer-on.png' )
        _SIZE          = _ICON_ON.shape[ 0 ]
    /**/
    };

}