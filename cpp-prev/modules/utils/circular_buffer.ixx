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
#include <mutex>


export module utils.circular_buffer;

namespace avt::utils
{
    //===================================================================
    /** \brief The class of circular buffers.
    *
    Circular  buffers  are  buffers  that contain at most 'n' items
    and that remember the 'n' lastly appended ones.
    */
    export template<typename T>
        class CircularBuffer<T>
    {
    public:
        //---   constructors   ------------------------------------------
        /** \brief Constructor.
        *
        * \arg buffer_size: the overall number of slots that are available
        *    in this circular buffer. Must be greater than 1.
        */
        CircularBuffer<T>(const long buffer_size) noexcept(false)
            : m_buffer(nullptr),
              m_max_count(buffer_size),
              m_current_index(0),
              m_current_count(0),
              m_mutex()
        {
            assert(buffer_size > 1);
            m_buffer = new[buffer_size] T;
            m_mutex.unlock();
        }

        CircularBuffer<T>(const CircualrBuffer<T>&) = delete;       // no copy constructor
        CircularBuffer<T>(CircualrBuffer<T>&&) noexcept = default;  // move constructor is allowed

        CircularBuffer<T> operator = (const CircularBuffer<T>&) = delete;       // no copy assignment
        CircularBuffer<T> operator = (CircularBuffer<T>&&) noexcept = default;  // move assignment is allowed


        //---   Appending new items   -----------------------------------
        /** \brief Non blocking appending of a new item.
        *
        * If this circular buffer is full, the newly appended item just
        * takes place over the oldest item contained in the buffer.
        *
        * \arg item: a reference to the appended item.
        */
        void push(const T& item)
        {
            if (is_ok()) {
                const std::lock_guard<std::mutex> guarded_lock(m_mutex);

                m_buffer[m_current_index] = item;
                m_current_index = (++m_current_index) % m_max_count;
                if (m_current_count < m_max_count)
                    m_current_count++;
            }
        }

        /** \brief Blocking appending of a new item.
        *
        * If this circular buffer is full, the appending waits
        * until the oldest item contained in  the  buffer  has
        * been removed from it.
        *
        * Caution: dead lock prone when the  application  runs
        * one thread only.
        *
        * \arg item: a reference to the appended item.
        * \arg wait: the milliseconds  count  to  wait  before
        *   attempting  to append again the item.  Defaults to
        *   20 ms.
        */
        void push_wait(const T& item, const std::chrono::milliseconds wait = 20)
        {
            if (is_ok()) {
                while (is_full())
                    std::this_thread::sleep_for(wait);
                push(item);
            }
        }

        /** \brief Removes the oldest item from this circular buffer and gives back its content.
        *
        * \arg item: a reference to the popped item. If no item is
        *   currently available in the buffer, the content of item
        *   passed at call time remains unchanged.
        * \return true if an item has been popped from this buffer
        *   or false otherwise.
        */
        const bool pop(T& item) noexcept
        {
            if (is_ok()) {
                const std::lock_guard<std::mutex> guarded_lock(m_mutex);
                
                if (is_empty)
                    return false;
                
                item = (*this)[-m_current_count];
                return true;
            }
        }


        //--- Accessing indexed items   ---------------------------------
        /** \biref /


        //---   Miscelaneous   ------------------------------------------
        /** \brief Returns true if this circular buffer is ok. */
        inline const bool is_ok() const noexcept
        {
            return m_buffer != nullptr;
        }

        /** \brief Returns true if this circular buffer is empty. */
        inline bool is_empty() const noexcept
        {
            const std::lock_guard<std::mutex> guarded_lock(m_mutex);
            return m_current_count == 0;
        }

        /** \brief Returns true if this circular buffer is full. */
        inline bool is_full() const noexcept
        {
            const std::lock_guard<std::mutex> guarded_lock(m_mutex);
            return m_current_count == m_max_count;
        }

        /** \brief Returns True if this buffer is full or nearly full.
        * 
        *\arg free_slots: The number of slots that have not yet been 
        *   set before this method returns true.
        */
        inline bool is_nearly_empty(const long free_slots = 1) const noexcept(false)
        {
            assert(free_slots >= 0);
            const std::lock_guard<std::mutex> guarded_lock(m_mutex);
            return m_max_count - m_current_count <= free_slots;
        }

        /** \brief Returns the actual count of items containes withihn the buffer. */
        inline const long size() const noexcept
        {
            return m_current_count;
        }


    protected:


    private:
        T*          m_buffer;
        long        m_max_count;
        long        m_current_index;
        long        m_current_count;
        std::mutex  m_mutex;
    };
}

#=============================================================================
class CircularBuffer :

#-------------------------------------------------------------------------
    def get_latest(self)->Any:
'''Returns the latest item stored in this buffer.
'''
return self[-1]

#-------------------------------------------------------------------------
def get_oldest(self)->Any:
'''Returns the oldest item stored in this buffer.
'''
return self[0]

#-------------------------------------------------------------------------
    def __getitem__(self, index: int)->Any:
'''Returns an indexed item from this circular buffer.

Notice : index 0 references the oldest item that has
been stored in this buffer.Index - 1 references
the latest one, i.e.the most recently  stored
in this buffer.Indexes 1 up to 'self.count-1'
reference the item that are contained  in  this
buffer in their chronological order of storing.

Args :
    index : int
    The index of the stored item to give back.

    Returns :
    The  item stored in this buffer that is refer -
    enced by the specified index, or None if  the
    index is out of bounds.Notice : never raises
    any exception, such as KeyError for instance.
    '''
    if - self.max_count <= index < self.max_count:
return self.buf[(index + self.ndx) % self.count]
    else:
return None


#=====   end of   src.Buffers.circular_buffer   =====#