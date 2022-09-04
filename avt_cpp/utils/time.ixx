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
#include <time.h>


export module utils.time;


//===========================================================================
namespace avt::utils::time
{
    //-----------------------------------------------------------------------
    /** @brief Returns the full "ctime" string related to local current date and time.
    *
    * The returned format of the string is:
    *   Ddd Mmm DD HH:MM:SS YYYY
    * where:
    *   Ddd are thr first 3 letters of the week-day
    *   Mmm are the first 3 letters of the month
    *   DD are the 2 digits of local day in month
    *   HH are the 2 digits or local time hours
    *   MM are the 2 digits of local time minutes
    *   SS are the 2 digits of local time seconds
    *   YYYY are the 4 digits of year.
    */
    export std::string now() noexcept;

    //-----------------------------------------------------------------------
    /** @brief Returns the current date as "YYYY-MM-DD". */
    export std::string date() noexcept;

    //-----------------------------------------------------------------------
    /** @brief Returns the current time as "HH:MM". */
    export std::string time() noexcept;

    //-----------------------------------------------------------------------
    /** @brief Returns the current time as "HH:MM:SS". */
    export std::string time_s() noexcept;


    //---   Internal definitions, not exported   ----------------------------
    std::tm _get_current_time() noexcept;

}