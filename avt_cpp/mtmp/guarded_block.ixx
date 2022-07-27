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
export module mtmp.guarded_block;

import mtmp.mutex;


//===========================================================================
export namespace mtmp
{
    //=======================================================================
    /** @brief The class for Guarded Blocks of code. */
    class GuardedBlock
    {
    public:
        //---   Constructors / Destructor   ---------------------------------
        /** @brief Default Constructor. */
        inline GuardedBlock(mtmp::Mutex* p_mtx) noexcept
            : mp_mtx{ p_mtx }
        {
            if (mp_mtx != nullptr)
                mp_mtx->lock();
        }

        /** @brief Default Destructor. */
        virtual inline ~GuardedBlock() noexcept
        {
            if (mp_mtx != nullptr)
                mp_mtx->unlock();
        }

        GuardedBlock(const GuardedBlock&) = delete;
        GuardedBlock(GuardedBlock&&) = delete;
        GuardedBlock& operator=(const GuardedBlock&) = delete;
        GuardedBlock& operator=(GuardedBlock&&) = delete;


    private:
        mtmp::Mutex* mp_mtx;
    };

}