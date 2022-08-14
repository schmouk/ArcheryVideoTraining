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

#include <algorithm>
#include <cstring>
#include <filesystem>

#include <opencv2/imgcodecs.hpp>
#include <opencv2/core/types.hpp>

#include "utils/types.h"


export module gui.items.picture;

import avt.config;
import utils.coords2d;


//===========================================================================
export namespace avt::gui::items
{
    //=======================================================================
    // This module defines:
    //  - class Picture
    //  - using Icon
    // 
    
    //=======================================================================
    /** @brief The class of picture loaders.
    *
    * This is a helper class for the loading of pictures from disk.
    */
    class Picture
    {
    public:
        //---   Constructors / Destructors   --------------------------------
        /** @brief Default Empty constructor. */
        Picture() noexcept = default;

        /** @brief Value Constructor. */
        inline Picture(const std::string& filename) noexcept
        {
            load(filename);
        }

        /** @brief Default Copy constructor. */
        Picture(const Picture&) noexcept = default;

        /** @brief Default Move constructor. */
        Picture(Picture&&) noexcept = default;

        /** @brief Default Destructor. */
        ~Picture() noexcept = default;


        //---   Exceptions   ------------------------------------------------
        /** @brief Exception on erroneous loading of this cursor. */
        class PictureNotFoundException : public std::exception
        {
            const char* what() const noexcept { return ("!!! Error: Unable to load picture\n"); }
        };


        //---   Operations   ------------------------------------------------
        /** @brief Draws this picture into an Image (2 scalar coordinates). */
        template<typename X, typename Y>
            requires std::is_arithmetic_v<X> && std::is_arithmetic_v<Y>
        inline void draw(const X x, const Y y, avt::ImageType& image) noexcept
        {
            content.copyTo(cv::Mat3b(image, cv::Rect{ x, y, width(), height() }));
        }

        /** @brief Draws this picture into an Image (1 2D-coords position). */
        inline void draw(const avt::utils::Coords2D pos, avt::ImageType& image) noexcept
        {
            draw(pos.x, pos.y, image);
        }

        /** @brief Draws this picture into an Image (1 pair/container as position). */
        template<typename P>
            requires avt::is_pair_type_v<P>
        inline void draw(const P& pos, avt::ImageType& image) noexcept
        {
            draw(pos[0], pos[1], image);
        }

        /** @brief Loads this picture from file. */
        inline void load(const std::string& filename) noexcept
        {
            const std::filesystem::path file_path = avt::config::PICTURES_DIR / filename;
            content = cv::imread((file_path.string()));
        }


        //---   Accessors   -------------------------------------------------
        /** @brief Returns the height of this picture. */
        inline const int height() const noexcept
        {
            return content.rows;
        }

        /** @brief Returns the width of this picture. */
        inline const int width() const noexcept
        {
            return content.cols;
        }


        //---   Attributes   ------------------------------------------------
        avt::ImageType content{};  //!< the content of this picture.
    };


    //=======================================================================
    /** @brief The class of Icons. */
    using Icon = Picture;

}