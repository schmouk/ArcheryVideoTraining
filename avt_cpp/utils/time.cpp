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

#include <chrono>
#include <ctime>
#include <string>


module utils.time;


//===========================================================================
namespace avt::utils::time
{
   /** Returns the full "ctime" string related to local current date and time. */
    std::string now() noexcept
    {
        using _clock = std::chrono::system_clock;
        const std::time_t current_time = _clock::to_time_t(_clock::now());
        constexpr rsize_t BUFFER_SIZE{ 32 };
        char time_buffer[BUFFER_SIZE]{ char(0) };
        ctime_s(time_buffer, BUFFER_SIZE, &current_time);
        return std::string(time_buffer);
    }


    /** Returns the current date as "YYYY-MM-DD". */
    std::string date() noexcept
    {
        const std::tm current_time{ _get_current_time() };
        try {
            constexpr int BUFFER_SIZE{ 12 };
            char buffer[BUFFER_SIZE]{ 0 };
            std::strftime(buffer, BUFFER_SIZE, "%Y-%m-%d", &current_time);
            return buffer;
        }
        catch (...) {
            return "0000-00-00";
        }
    }


    /** Returns the current time as "HH:MM". */
    std::string time() noexcept
    {
        return avt::utils::time::time_s().substr(0, 5);
    }


    /** Returns the current time as "HH:MM:SS". */
    std::string time_s() noexcept
    {
        const std::tm current_time{ _get_current_time() };
        try {
            constexpr int BUFFER_SIZE{ 12 };
            char buffer[BUFFER_SIZE]{ 0 };
            std::strftime(buffer, BUFFER_SIZE, "%H:%M:%S", &current_time);
            return buffer;
        }
        catch (...) {
            return "00:00:00";
        }
    }


    /** Internal private function. */
    std::tm _get_current_time() noexcept
    {
        std::time_t current_time = std::time(nullptr);
        std::tm     time;
        localtime_s(&time, &current_time);
        return time;
    }

}