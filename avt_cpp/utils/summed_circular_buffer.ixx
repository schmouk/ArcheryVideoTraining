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

#include <type_traits>


export module utils.summed_circular_buffer;

import utils.circular_buffer;


//===========================================================================
export namespace avt::utils
{
    //=======================================================================
    /** @brief The class of Summed Circular Buffers.
    *
    * Summed buffers are circular buffers Which contain scalar values or 
    * objects for which scalar casting operators are defined.
    * 
    * The 'sum' of these values is maintained at any time. The 'mean' of 
    * this sum is also available at any time and is provided as a method
    * of this class.
    *
    * Notice: currently this class is not thread safe.
    */
    export template<typename TItem, const int SIZE>
        requires std::is_arithmetic_v<TItem> && (SIZE > 1)
    class SummedCircularBuffer : public CircularBuffer<TItem, SIZE>
    {
    public:
        //---   Wrappers   --------------------------------------------------
        using MyBaseType = CircularBuffer<TItem, SIZE>;
        static inline constexpr int MAX_SIZE = SIZE;


        //---   Constructors / Destructors   --------------------------------
        /** @brief Constructor. */
        inline SummedCircularBuffer() noexcept
            : MyBaseType{}, sum{ TItem(0) }
        {}

        /** @brief Default Copy constructor. */
        SummedCircularBuffer(const SummedCircularBuffer&) noexcept = default;

        /** @brief Default Move constructor. */
        SummedCircularBuffer(SummedCircularBuffer&&) noexcept = default;

        /** @brief Default destructor. */
        virtual ~SummedCircularBuffer() noexcept = default;


        //---   Operations   ------------------------------------------------
        /** @brief Appends a new item to this circular buffer.
        *
        * If this buffer is full, the oldest item is removed from it.
        *
        * @param new_item : const Item&
        *   A reference to the new item to be stored in this buffer.
        * @return A reference to this circular buffer.
        */
        virtual SummedCircularBuffer& append(const TItem& value) noexcept override
        {
            if (MyBaseType::is_full()) {
                sum -= MyBaseType::get_oldest();
            }
            sum += value;

            return dynamic_cast<SummedCircularBuffer&>(MyBaseType::append(value));
        }

        /** @brief Evaluates the current mean on this summed circular buffer. */
        inline const double mean() const noexcept
        {
            return (this->count == 0) ? 0.0 : double(sum) / double(this->count);
        }


        //---   Attributes   ------------------------------------------------
        TItem sum;

    };

}


/*** /
class SummedCircularBuffer( CircularBuffer ):

    #-------------------------------------------------------------------------
    @property
    def mean(self) -> float:
        if self.count != 0:
            return self.sum / self.count
        else:
            return 0.0

    #-------------------------------------------------------------------------
    def __iadd__(self, value: Numeric) -> None:
        '''Appends a new value into this summed buffer.

        If this buffer is full,  the oldest item is removed
        from it.

        Args:
            item: Any
                The new item to be stored in  this  buffer.
                This  may  be a reference to an instance of
                any class.

        Returns:
            a reference to this circular buffer.

        Raises:
            AssertionError: value is neither an integer nor
                a float.
        '''
        assert isinstance( value, int ) or isinstance( value, float ), \
               f"passed argument must be either an integer or a float, and not of type {type(value)}"

        if self.is_full():
            self.sum -= self.get_oldest()
        self.sum += value

        return super().__iadd__( value )

#=====   end of   src.Buffers.summed_circular_buffer   =====#
/***/
