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

#include <array>
#include <stdexcept>


export module utils.buffers;

import mtmp.guarded_block;
import mtmp.mutex;


//===========================================================================
namespace avt::utils
{
    //=======================================================================
    /** @brief The class of Circular Buffers.
    *
    * Circular buffers are buffers which contain at most 'n' items and which 
    * remember the 'n' last ones.  When appending a new item, the oldest one
    * is removed from the buffer as soon as the buffer is full.
    * 
    * Notice: this class is thread safe.
    */
    export template<typename TItem, const int SIZE>
        requires (SIZE > 1)
    class CircularBuffer
    {
    public:
        //---   Wrappers   --------------------------------------------------
        static inline constexpr int MAX_SIZE = SIZE;


        //---   Constructors / Destructors   --------------------------------
        /** @brief Constructor. */
        inline CircularBuffer() noexcept
            : m_mutex{}, buffer{}, index{ 0 }, count{ 0 }
        {}

        /** @brief Default Copy constructor. */
        CircularBuffer(const CircularBuffer&) noexcept = default;

        /** @brief Default Move constructor. */
        CircularBuffer(CircularBuffer&&) noexcept = default;

        /** @brief Default destructor. */
        virtual ~CircularBuffer() noexcept = default;


        //---   Operations   ------------------------------------------------
        /** @brief Appends a new item to this circular buffer.
        *
        * If this buffer is full, the oldest item is removed from it.
        * @sa operator+=
        *
        * @param new_item : TItem
        *   A reference to the new item to be stored in this buffer.
        * @return A reference to this circular buffer.
        */
        CircularBuffer& append(const TItem& new_item) noexcept
        {
            avt::mtmp::GuardedBlock{ &m_mutex };

            buffer[index] = new_item;
            index = (index + 1) % SIZE;

            if (count < SIZE)
                count++;

            return *this;
        }

        /** @brief Returns the latest item stored in this buffer. */
        inline TItem& get_latest() noexcept
        {
            return (*this)[-1];
        }

        /** @brief Returns the oldest item stored in this buffer. */
        inline TItem& get_oldest() noexcept
        {
            return (*this)[0];
        }

        /** @brief Returns True if this buffer contains no item, or false otherwise. */
        inline const bool is_empty() const noexcept
        {
            return count == 0;
        }

        /** @brief Returns True if this buffer is full, or false otherwise. */
        inline const bool is_full() const noexcept
        {
            return count == SIZE;
        }

        /** @brief Returns True if this buffer is full or nearly full.
        * 
        * @param free_slots: int
        *   The number of slots that have not yet been set before  this
        *   method returns False.  Values <= 0 make  'is_nearly_full()' 
        *   acting as 'is_full()'.
        */
        inline const bool is_nearly_full(const int free_slots = 1) const noexcept
        {
            return (free_slots <= 0) ? is_full() : (SIZE - count <= free_slots);
        }

        /** @brief Stores a new item to this circular buffer).
        *
        * If this buffer is full, the oldest item is removed from it.
        *
        * @param new_item : TItem
        *   A reference to the new item to be stored in this  buffer.
        * @return A reference to this circular buffer.
        */
        inline CircularBuffer& store(const TItem& new_item) noexcept
        {
            return append(new_item);
        }


        //---   Operators   -------------------------------------------------
        /** @brief Appending operator (appends a new item to this circular buffer).
        *
        * If this buffer is full, the oldest item is removed from it.
        *
        * @param new_item : TItem
        *   A reference to the new item to be stored in this  buffer.
        * @return A reference to this circular buffer.
        */
        inline CircularBuffer& operator+= (const TItem& new_item) noexcept
        {
            return append(new_item);
        }

        /** @brief Returns an indexed item from this circular buffer..
        *
        * Notice: index 0 references the oldest item that has  been  stored 
        * in this buffer. Index -1 references the latest one, i.e. the most 
        * recently stored in this buffer.  Indexes 1 up  to  'self.count-1'
        * reference  the  item  that  are contained in this buffer in their 
        * chronological order of storing.
        * 
        * @param index : int
        *   The index of the stored item to give back.
        * 
        * @return  the item stored in this buffer that is referenced by the 
        *   specified index.
        */
        TItem& operator[] (const int index_) noexcept(false)
        {
            if (-SIZE <= index_ && index_ < SIZE)
                return buffer[(index_ + index + SIZE) % SIZE];
            else
                throw std::invalid_argument("!!! Error: index out of range");
        }


        //---   Attributes   ------------------------------------------------
        std::array<TItem, SIZE> buffer;
        int                     index;
        int                     count;


    private:
        //---   Attributes   ------------------------------------------------
        avt::mtmp::Mutex m_mutex;
    };

}