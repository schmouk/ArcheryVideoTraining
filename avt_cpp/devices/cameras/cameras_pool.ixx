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
    class CamerasPool : public std::vector<avt::devices::cameras::Camera>
    {
    public:
        //---   Wrappers   --------------------------------------------------
        using MyBaseType = std::vector<avt::devices::cameras::Camera>;


        //---   Constructors / Destructor   ---------------------------------
        /** @brief Empty Constructor. */
        inline CamerasPool() noexcept
            : MyBaseType{}
        {
            evaluate_connected_cameras();
        }

        /** @brief Constructor with console window. */
        inline CamerasPool(const avt::ImageType& console_window) noexcept
            : MyBaseType{}
        {
            evaluate_connected_cameras(console_window);
        }

        /** @brief Default Destructor. */
        virtual ~CamerasPool() noexcept = default;


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
