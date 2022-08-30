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

#include <exception>
#include <filesystem>
#include <format>
#include <string>

#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>

#include "utils/types.h"


export module gui.items.target;

import avt.config;
import gui.views.view;


//===========================================================================
export namespace avt::gui::items
{
    //=======================================================================
    /** @brief The class of archery Targets. */
    class Target
    {
    public:
        //---   Constructors / Destructors   --------------------------------
        /** @brief Constructor.
        *
        * This constructor does not load the target  picture  into  memory.
        * Further calls to method 'get_picture()' or 'get_resized()'  load 
        * it into memory.
        * 
        * @param filename: string,
        *   The name of the file that contains the picture of this  target,
        *   with its extension.  This is NOT the full path to the file,  it
        *   is just its name and extension.
        * @param true_size_cm: int,
        *   The real diameter size of this target, provided in centimeters.
        */
        inline Target(const std::string& filename, const int true_size_cm) noexcept
            : filepath{ avt::config::PICTURES_DIR / filename },
              size_cm{ true_size_cm },
              picture{}
        {}

        /** @brief Default Constructor. */
        Target() noexcept = default;

        /** @brief Default Copy Constructor. */
        Target(const Target&) noexcept = default;

        /** @brief Default Move Constructor. */
        Target(Target&&) noexcept = default;

        /** @brief Default Destructor. */
        ~Target() noexcept = default;


        //---   Assignments   -----------------------------------------------
        /** @brief Default Copy assignment. */
        Target& operator= (const Target&) noexcept = default;

        /** @brief Default Move assignment. */
        Target& operator= (Target&&) noexcept = default;


        //---   Operations   ------------------------------------------------
        /** @brief Draws this target resized in the specified view.
        *
        * @param view: View
        *   A reference to the embedding view.
        * @param ratio: float
        *   The ratio to apply for the resizing of the  picture  of 
        *   this target.
        */
        void draw(avt::gui::views::View& view, const float ratio);


        /** @brief Loads the whole picture of this target.
        *
        * Notice: Throws exception if path to file cannot be opened,  if
        *   file cannot be read or if file coding scheme is not readable 
        *   by library OpenCV.
        */
        inline avt::ImageType& get_picture() noexcept(false);


        /** @brief Resizes the picture of this target according to the specified ratio. */
        avt::ImageType get_resized(const float ratio) noexcept(false);


        //---   Exceptions   ------------------------------------------------
        /** @brief Target picture cannot be loaded from file. */
        class TargetPictureLoadException : std::exception
        {
        public:
            const char* what() noexcept { return "!!! ERROR: can't load an archery target picture file"; }
        };



    protected:
        //---   Attributes   ------------------------------------------------
        avt::ImageType        picture;   //!< reference to the picture associated with this target
        std::filesystem::path filepath;  //!< the full path to this target image file
        int                   size_cm;   //!< the actual real size of this target, set in centimeters
    };

}
