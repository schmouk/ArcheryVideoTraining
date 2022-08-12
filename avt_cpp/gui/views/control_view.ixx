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
        /** @brief Manages the exit control. */
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
        /** @brief Manages the match control. */
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
        /** @brief Manages the match control. */
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





        /** /

    #-------------------------------------------------------------------------
    class _CtrlRecord( _CtrlBase ):
        '''The video recording control.
        '''
        #---------------------------------------------------------------------
        def __init__(self, x: int = None,
                           y: int = None,
                           enabled: bool = True,
                           active : bool = False,
                           *,
                           pos: Point = None) -> None:
            '''Constructor

            Args:
                x, y: int
                    The top-left position of  this  control  in
                    the  ControlView.  Ignored if 'pos' is set.
                    Must be set if 'pos' is  None.  Defaults to
                    None (i.e. 'pos' should be set instead).
                enabled: bool
                    Set this to True to get this control enabl-
                    ed  or set it to False otherwise.  Defaults
                    to False.
                active: bool
                    Set this to True to get this control active
                    or  set  it  to  False  to get it inactive.
                    Defaults to False.
                pos: Point
                    The top-left position of  this  control  in
                    the ControlView.  Takes precedence over 'x'
                    and 'y' if set. This argument must be named
                    if set. Defaults to None.

            Raises:
                AssertionError:  x, y and pos are all None, or
                    pos is None and either x or y is None also.
            '''
            super().__init__( x, y, enabled, active, pos=pos )
            self.slider = FloatSlider( x = (x if x else pos.x) + 5,
                                       y = (y if y is not None else pos.y) + self._ICON_SIZE + 8,
                                       width = ControlView.WIDTH - 12*2,
                                       height = 5,
                                       min_value = 20,
                                       max_value = 130,
                                       current_value = 60,
                                       bar_color = GRAY,
                                       cursor_color = self._TICKS_FONT_ENABLED.color,
                                       text_font = self._TICKS_FONT_ENABLED,
                                       shadow_height = 0,
                                       visible = True,
                                       enabled = enabled,
                                       active = active,
                                       show_cursor_text = False )

        #---------------------------------------------------------------------
        def draw(self, view: View) -> None:
            '''Draws a control in its embedding content.
            Args:
                view: View
                    A reference to the embedding view.
            '''
            cursor_text = str( self.slider.value )

            if self.enabled:
                if self.is_active:
                    img = self._ICON_ON
                    font = self._FONT_2_ON if len(cursor_text) < 3 else self._FONT_3_ON
                else:
                    img = self._ICON_OFF
                    font = self._FONT_2_OFF if len(cursor_text) < 3 else self._FONT_3_OFF
            else:
                img = self._ICON_DISABLED
                font = self._FONT_2_DISABLED if len(cursor_text) < 3 else self._FONT_3_DISABLED

            x = (view.WIDTH - self._ICON_SIZE) // 2
            y = self.y + 1

            view.content[ y:y+self._ICON_SIZE, x:x+self._ICON_SIZE, : ] = img[ :, :, : ]

            cursor_text_width = font.get_text_width( cursor_text )
            x = (view.WIDTH - cursor_text_width ) // 2 + 1
            y = self.y + (self._ICON_SIZE + self._FONT_SIZE) // 2 - (2 if len(cursor_text) < 3 else 4)
            font.draw_text( view, Point(x,y), cursor_text, True )

            ##font.draw_text( view, Point(self.x + 5, self.y + self._FONT_SIZE), 'Delay' )
            self.slider.draw( view )

        #---------------------------------------------------------------------
        _FONT_3_SIZE        = 8
        _FONT_2_SIZE        = 11
        _FONT_3_DISABLED    = Font( _FONT_3_SIZE, GRAY )
        _FONT_3_OFF         = Font( _FONT_3_SIZE, LIGHT_GRAY )
        _FONT_3_ON          = Font( _FONT_3_SIZE, YELLOW )
        _FONT_2_DISABLED    = Font( _FONT_2_SIZE, GRAY )
        _FONT_2_OFF         = Font( _FONT_2_SIZE, LIGHT_GRAY )
        _FONT_2_ON          = Font( _FONT_2_SIZE, YELLOW )
        _ICON_DISABLED      = cv2.imread( '../picts/controls/record-disabled.png' )
        _ICON_OFF           = cv2.imread( '../picts/controls/record-off.png' )
        _ICON_ON            = cv2.imread( '../picts/controls/record-on.png' )
        _ICON_SIZE          = _ICON_ON.shape[ 0 ]
        _TICKS_FONT_SIZE    = 8
        _TICKS_FONT_ENABLED = Font( _TICKS_FONT_SIZE, YELLOW // 1.33 )


    #-------------------------------------------------------------------------
    class _CtrlReplay( _CtrlBase ):
        '''The replay control.
        '''
        #---------------------------------------------------------------------
        def draw(self, view: View) -> None:
            '''Draws a control in its embedding content.
            Args:
                view: View
                    A reference to the embedding view.
            '''
            if self.enabled:
                if self.is_active:
                    icons = (self._ICON_STEP_BW_ON,
                             self._ICON_STEP_FW_ON,
                             self._ICON_PLAY_ON,
                             self._ICON_FBW_ON,
                             self._ICON_FFW_ON)
                else:
                    icons = (self._ICON_STEP_BW_OFF,
                             self._ICON_STEP_FW_OFF,
                             self._ICON_PLAY_OFF,
                             self._ICON_FBW_OFF,
                             self._ICON_FFW_OFF)
            else:
                icons = (self._ICON_STEP_BW_DISABLED,
                         self._ICON_STEP_FW_DISABLED,
                         self._ICON_PLAY_DISABLED,
                         self._ICON_FBW_DISABLED,
                         self._ICON_FFW_DISABLED)

            x0 = self.x + 5
            y0 = self.y + 23
            x1 = x0 + self._SIZE
            x2 = x1 + self._SIZE
            y1 = y0 + self._SIZE // 2 + 2
            y2 = y0 + self._SIZE + 3

            view.content[ y0:y0+self._SIZE,
                          x0:x0+self._SIZE, : ] = icons[0][:,:,:]

            view.content[ y0:y0+self._SIZE,
                          x2:x2+self._SIZE, : ] = icons[1][:,:,:]

            view.content[ y1:y1+self._SIZE,
                          x1:x1+self._SIZE, : ] = icons[2][:,:,:]

            view.content[ y2:y2+self._SIZE,
                          x0:x0+self._SIZE, : ] = icons[3][:,:,:]

            view.content[ y2:y2+self._SIZE,
                          x2:x2+self._SIZE, : ] = icons[4][:,:,:]

        #---------------------------------------------------------------------
        _ICON_FBW_DISABLED     = cv2.imread( '../picts/controls/fbw-25-disabled.png' )
        _ICON_FBW_OFF          = cv2.imread( '../picts/controls/fbw-25-off.png' )
        _ICON_FBW_ON           = cv2.imread( '../picts/controls/fbw-25-on.png' )
        _ICON_FFW_DISABLED     = cv2.imread( '../picts/controls/ffw-25-disabled.png' )
        _ICON_FFW_OFF          = cv2.imread( '../picts/controls/ffw-25-off.png' )
        _ICON_FFW_ON           = cv2.imread( '../picts/controls/ffw-25-on.png' )
        _ICON_PAUSE_DISABLED   = cv2.imread( '../picts/controls/pause-25-disabled.png' )
        _ICON_PAUSE_OFF        = cv2.imread( '../picts/controls/pause-25-off.png' )
        _ICON_PAUSE_ON         = cv2.imread( '../picts/controls/pause-25-on.png' )
        _ICON_PLAY_DISABLED    = cv2.imread( '../picts/controls/play-25-disabled.png' )
        _ICON_PLAY_OFF         = cv2.imread( '../picts/controls/play-25-off.png' )
        _ICON_PLAY_ON          = cv2.imread( '../picts/controls/play-25-on.png' )
        _ICON_STEP_BW_DISABLED = cv2.imread( '../picts/controls/step-bw-25-disabled.png' )
        _ICON_STEP_BW_OFF      = cv2.imread( '../picts/controls/step-bw-25-off.png' )
        _ICON_STEP_BW_ON       = cv2.imread( '../picts/controls/step-bw-25-on.png' )
        _ICON_STEP_FW_DISABLED = cv2.imread( '../picts/controls/step-fw-25-disabled.png' )
        _ICON_STEP_FW_OFF      = cv2.imread( '../picts/controls/step-fw-25-off.png' )
        _ICON_STEP_FW_ON       = cv2.imread( '../picts/controls/step-fw-25-on.png' )
        _SIZE = 25 ##_ICON_PLAY_ON.shape[0]


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