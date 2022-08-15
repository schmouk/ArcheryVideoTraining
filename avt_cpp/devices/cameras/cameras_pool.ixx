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

#include <vector>

#include "devices/cameras/camera.h"
#include "utils/types.h"


export module devices.cameras.cameras_pool;


//===========================================================================
export namespace avt::devices::cameras
{
    //=======================================================================
    /** @brief The class of the pool of cameras.
    *
    * The AVT application may involve many  cameras.
    * They all are managed within a pool of cameras.
    */
    class CamerasPool : public std::vector<avt::devices::cameras::Camera*>
    {
    public:
        //---   Constructors / Destructor   ---------------------------------
        /** @brief Empty Constructor. */
        inline CamerasPool() noexcept
            : std::vector<avt::devices::cameras::Camera*>{}
        {
            evaluate_connected_cameras();
        }

        /** @brief Constructor with console window. */
        inline CamerasPool(const avt::ImageType& console_window) noexcept
            : std::vector<avt::devices::cameras::Camera*>{}
        {
            evaluate_connected_cameras(console_window);
        }

        /** @brief Default Destructor. */
        inline ~CamerasPool() noexcept
        {
            for (avt::devices::cameras::Camera* p_cam : *this)
                delete p_cam;
        }


        //---   Operations   ------------------------------------------------
        /** @brief Evaluates all the connected cameras.
        * 
        * Initializes the pool of cameras according to  the  currently 
        * connected ones.
        * 
        * @args p_console_window: avt::ImageType*
        *   A pointer to the console window in which progress messages
        *   are displayed. No message is displayed if pointer is null.
        */
        void evaluate_connected_cameras(const avt::ImageType& console_window = avt::ImageType{}) noexcept;
    };

}
    
    
    /***

#=============================================================================
import time
from types   import TracebackType
from typing  import ForwardRef, Tuple, Type

from src.App.avt_config      import AVTConfig
from src.GUIItems.avt_fonts  import AVTConsoleFont
from .camera                 import Camera
from src.Shapes.point        import Point


#=============================================================================
BaseExceptionType = Type[ BaseException ]
CamerasPoolRef    = ForwardRef( "CamerasPool" )
ExceptionInfo     = Tuple[ BaseExceptionType, BaseException, TracebackType ]
MainWindowRef     = ForwardRef( "MainWindow" )


#=============================================================================
class  CamerasPool( list ):
    """The class of the pool of cameras.

    The AVT application may involve many  cameras.
    They all are managed within a pool of cameras.
    """
    #-------------------------------------------------------------------------
    def __init__(self, parent_window: MainWindowRef) -> None:
        '''Constructor.

        Instantiates the pool of cameras.

        Args:
            parent_window: MainWindowRef
                A reference to the containing main window.
                Used to display progress messages.
        '''
        super().__init__()
        self.evaluate_connected_cameras( parent_window )

    #-------------------------------------------------------------------------
    def __del__(self) -> None:
        '''Releases all allocated resources.
        '''
        for camera in self:
            del camera

    #-------------------------------------------------------------------------
    def evaluate_connected_cameras(self, parent_window: MainWindowRef) -> None:
        '''Evaluates all the connected cameras.

        Initializes the pool of cameras according to  the
        currently connected ones.

        Args:
            parent_window: MainWindowRef
                A reference to the containing main window.
                Used to display progress messages.
        '''
        self.clear()

        x, y = 20, 40
        y_offset = 24

        for camera_index in range( AVTConfig.CAMERAS_MAX_COUNT ):

            AVTConsoleFont.forced_draw_text( parent_window,
                                             Point(x, y),
                                             f"testing connection of camera #{camera_index+1} " )

            camera = Camera( camera_index )

            if camera.is_ok():
                self.append( camera )

            else:
                AVTConsoleFont.forced_draw_text( parent_window,
                                                 Point(x, y+y_offset),
                                                 f"camera #{camera_index+1} not connected or not found" )
                time.sleep( 1.750 )
                del camera
                break

            y += y_offset

#=====   end of   src.Cameras.cameras_pool   =====#
/***/