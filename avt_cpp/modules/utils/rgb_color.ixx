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
#include <vector>

export module avt.utils;

//===========================================================================
namespace avt::utils
{
    //=======================================================================
    /** \brief The class of RGB components colors.
    *
    * Library OpenCV uses a default BGR coding for colors which is not the 
    * usual RGB one.  To help developments, class RGBColor codes colors in 
    * a usual way while the BGR conversion is internally automated for its
    * use with library OpenCV.
    */
    export class RGBColor final
    {
    public:
        //---   Constructors / Destructor   ---------------------------------
        /** \brief Default constructor. */
        inline RGBColor() noexcept
            : comp{ 0, 0, 0 }
        {}

        /** \brief Valued Constructor (three R, G, B bytes). */
        template<typename T, typename U, typename V>
        requires std::is_arithmetic_v<T> && std::is_arithmetic_v<U> && std::is_arithmetic_v<V>
        inline RGBColor(const T r, const U g, const V b) noexcept
        {
            set(r, g, b);
        }

        /** \brief Valued Constructor (one 3-bytes buffer). */
        template<typename T>
        requires std::is_arithmetic_v<T>
        inline RGBColor(const unsigned char buffer[3]) noexcept
        {
            set(buffer);
        }

        /** \brief Valued Constructor (std::vector). */
        template<typename T>
        requires std::is_arithmetic_v<T>
        inline RGBColor(const std::vector<T>& vect) noexcept(false)
        {
            set(vect);
        }

        /** \brief Valued Constructor (std::array). */
        template<typename T>
        requires std::is_arithmetic_v<T>
        inline RGBColor(const std::array<T, 3>& arr) noexcept
        {
            set(arr);
        }

        /** \brief Copy and Move constructors. */
        RGBColor(const RGBColor&) = default;
        RGBColor(RGBColor&&) = default;

        /** \brief Destructor. */
        ~RGBColor() = default;


        //---   Assignments   -----------------------------------------------
        /** \brief Copy and Move assignments. */
        RGBColor& operator= (const RGBColor&) = default;
        RGBColor& operator= (RGBColor&&) = default;

        /** \brief Assignment operator (one 3-bytes buffer). */
        template<typename T>
        requires std::is_arithmetic_v<T>
        inline RGBColor& operator= (const unsigned char buffer[3]) noexcept
        {
            set(buffer);
            return *this;
        }

        /** \brief Assignment operator (std::vector). */
        template<typename T>
        requires std::is_arithmetic_v<T>
        inline RGBColor& operator= (const std::vector<T>& vect) noexcept(false)
        {
            set(vect);
            return *this;
        }

        /** \brief Assignment operator (std::array). */
        template<typename T>
        requires std::is_arithmetic_v<T>
        inline RGBColor& operator= (const std::array<T, 3>& arr) noexcept
        {
            set(arr);
            return *this;
        }


        //---   Set color   -------------------------------------------------
        /** \brief Sets this color to BLACK. */
        inline void clr()
        {
            set(BLACK);
        }

        /** \brief Sets color (copy). */
        inline void set(const RGBColor& copy)
        {
            r = copy.r;
            g = copy.g;
            b = copy.b;
        }

        /** \brief Sets color (three R, G, B bytes). */
        template<typename T, typename U, typename V>
            requires std::is_arithmetic_v<T>&& std::is_arithmetic_v<U>&& std::is_arithmetic_v<V>
        inline void set(const T r, const U g, const V b) noexcept
        {
            this->r = r;
            this->g = g;
            this->b = b;
        }

        /** \brief Sets color (one 3-bytes buffer). */
        template<typename T>
            requires std::is_arithmetic_v<T>
        inline void set(const unsigned char buffer[3]) noexcept
        {
            set(buffer[0], buffer[1], buffer[2]);
        }

        /** \brief Sets color (std::vector). */
        template<typename T>
            requires std::is_arithmetic_v<T>
        inline void set(const std::vector<T>& vect) noexcept(false)
        {
            assert(vect.size() == 3);
            set(vect[0], vect[1], vect[2]);
        }

        /** \brief Sets color (std::array). */
        template<typename T>
            requires std::is_arithmetic_v<T>
        inline void set(const std::array<T, 3>& arr) noexcept
        {
            set(arr[0], arr[1], arr[2]);
        }


        //---   Gray Value   ------------------------------------------------
        /** \brief Returns the intensity of the luminance for this RGB color. */
        inline const unsigned char get_lum() const
        {
            return unsigned char(0.299f * r + 0.587f * g + 0.114f * b + 0.5f);
        }

        /** \brief Instantiates a new RGB color with the gray value of this RGB color. */
        inline RGBColor to_gray_color() const
        {
            const unsigned char lum = get_lum();
            return RGBColor(lum, lum, lum);
        }


        //---   Data   ------------------------------------------------------
        union {
            unsigned char comp[3];
            struct {
                unsigned char b;
                unsigned char g;
                unsigned char r;
            };
        };

        //---   Predefined colors   -----------------------------------------
        static const RGBColor
            ANTHRACITE,
            BLACK,
            BLUE,
            BROWN,
            DARK_RED,
            DEEP_GRAY,
            DEEP_GREEN,
            GRAY,
            LIGHT_BLUE,
            LIGHT_GRAY,
            LIGHT_GREEN,
            NAVY_BLUE,
            ORANGE,
            RED,
            YELLOW,
            WHITE,

            TARGET_WHITE,
            TARGET_BLACK,
            TARGET_BLUE,
            TARGET_RED,
            TARGET_GOLD,

            TARGET_BLUE_6,
            TARGET_BLUE_NFAA;

    protected:


    private:
        /** \brief Returns the clipped value acccording to color components type. */
        template<typename T>
        requires std::is_arithmetic_v<T>
        inline static const unsigned char _clipped(const T value)
        {
            return (value >= 255) ? 255 : ((value <= 0) ? 0 : (unsigned char)value);
        }
    };


    const RGBColor RGBColor::ANTHRACITE(32, 32, 32);
    const RGBColor RGBColor::BLACK(0, 0, 0);
    const RGBColor RGBColor::BLUE(0, 0, 255);
    const RGBColor RGBColor::BROWN(96, 48, 0);
    const RGBColor RGBColor::DARK_RED(128, 0, 0);
    const RGBColor RGBColor::DEEP_GRAY(64, 64, 64);
    const RGBColor RGBColor::DEEP_GREEN(0, 96, 0);
    const RGBColor RGBColor::GRAY(128, 128, 128);
    const RGBColor RGBColor::LIGHT_BLUE(0, 255, 255);
    const RGBColor RGBColor::LIGHT_GRAY(192, 192, 192);
    const RGBColor RGBColor::LIGHT_GREEN(0, 255, 0);
    const RGBColor RGBColor::NAVY_BLUE(0, 0, 64);
    const RGBColor RGBColor::ORANGE(255, 128, 0);
    const RGBColor RGBColor::RED(255, 0, 0);
    const RGBColor RGBColor::YELLOW(255, 255, 0);
    const RGBColor RGBColor::WHITE(255, 255, 255);

    const RGBColor RGBColor::TARGET_WHITE(255, 255, 255);
    const RGBColor RGBColor::TARGET_BLACK(0, 0, 0);
    const RGBColor RGBColor::TARGET_BLUE(65, 181, 200);
    const RGBColor RGBColor::TARGET_RED(255, 37, 21);
    const RGBColor RGBColor::TARGET_GOLD(255, 245, 55);
    
    const RGBColor RGBColor::TARGET_BLUE_6(17, 165, 255);
    const RGBColor RGBColor::TARGET_BLUE_NFAA(63, 63, 95);
}

/****

#-------------------------------------------------------------------------
            def __eq__(self, other: Color) -> bool:
        '''Returns True if both colors have same color components.
            '''
            try :
            return self.color == other.color
            except :
        try :
            return self.color == other
            except :
        return self.color == (other, other, other)

#-------------------------------------------------------------------------
            def __ne__(self, other: Color) -> bool :
            '''Returns True if any same color component differs in this and in other.
            '''
            try :
            return self.color != other.color
            except :
        try :
            return self.color != other
            except :
        return self.color != (other, other, other)

#-------------------------------------------------------------------------
            def __add__(self, other: Color)->RGBColorRef :
            '''
            '''
            try :
            return RGBColor(self.r + other.r, self.g + other.g, self.r + other.b)
            except :
            try :
            return RGBColor(self.r + other[0], self.g + other[1], self.b + other[2])
            except :
            return RGBColor(self.r + other, self.g + other, self.b + other)

#-------------------------------------------------------------------------
            def __iadd__(self, other: Color)->RGBColorRef :
            '''
            '''
            try :
            self.set(self.r + other.r, self.g + other.g, self.r + other.b)
            except :
            try :
            self.set(self.r + other[0], self.g + other[1], self.b + other[2])
            except :
            self.set(self.r + other, self.g + other, self.b + other)
            return self

#-------------------------------------------------------------------------
            def __radd__(self, other: Color)->RGBColorRef :
            '''
            '''
            return self.add(other)

#-------------------------------------------------------------------------
            def __floordiv__(self, den: Numeric)->RGBColorRef:
        '''
            '''
            return RGBColor(int(self.r / den), int(self.g / den), int(self.b / den))

#-------------------------------------------------------------------------
            def __ifloordiv__(self, den: Numeric)->RGBColorRef:
        '''
            '''
            self.set(int(self.r / den), int(self.g / den), int(self.b / den))
            return self

#-------------------------------------------------------------------------
            def __mul__(self, coeff: Numeric)->RGBColorRef:
        '''
            '''
            return RGBColor(round(self.r * coeff), round(self.g * coeff), round(self.b * coeff))

#-------------------------------------------------------------------------
            def __imul__(self, coeff: Numeric)->RGBColorRef:
        '''
            '''
            self.set(round(self.r * coeff), round(self.g * coeff), round(self.b * coeff))
            return self

#-------------------------------------------------------------------------
            def __rmul__(self, coeff: Numeric)->RGBColorRef:
        '''
            '''
            return RGBColor(round(self.r * coeff), round(self.g * coeff), round(self.b * coeff))

#-------------------------------------------------------------------------
            def __sub__(self, other: Color)->RGBColorRef:
        '''
            '''
            try :
            return RGBColor(self.r - other.r, self.g - other.g, self.r - other.b)
            except :
            try :
            return RGBColor(self.r - other[0], self.g - other[1], self.b - other[2])
            except :
            return RGBColor(self.r - other, self.g - other, self.b - other)

#-------------------------------------------------------------------------
            def __isub__(self, other: Color)->RGBColorRef :
            '''
            '''
            try :
            self.set(self.r - other.r, self.g - other.g, self.r - other.b)
            except :
            try :
            self.set(self.r - other[0], self.g - other[1], self.b - other[2])
            except :
            self.set(self.r - other, self.g - other, self.b - other)
            return self

#-------------------------------------------------------------------------
            def __rsub__(self, other: Color)->RGBColorRef :
            '''
            '''
            try :
            return RGBColor(other.r - self.r, other.g - self.g, other.b - self.b)
            except :
            try :
            return RGBColor(other[0] - self.r, other[1] - self.g, other[2] - self.b)
            except :
            return RGBColor(other - self.r, other - self.g, other - self.b)

#-------------------------------------------------------------------------
            def __truediv__(self, den: Numeric)->RGBColorRef :
            '''
            '''
            return RGBColor(round(self.r / den), round(self.g / den), round(self.b / den))

#-------------------------------------------------------------------------
            def __itruediv__(self, den: Numeric)->RGBColorRef:
        '''
            '''
            self.set(round(self.r / den), round(self.g / den), round(self.b / den))
            return self


#=============================================================================
            class GrayColor(RGBColor) :
            """The class of RGB gray colors.

            """
#-------------------------------------------------------------------------
            def __init__(self, comp: int)->None :
            '''Constructor.

            Args :
            comp : int
            The value of the gray level of this gray color,
            clipped within interval[0, 255].
            Notice : value 0 stands for  black, value  255
            stands for white.
            '''
            super().__init__(comp, comp, comp)


#=============================================================================
            ANTHRACITE = RGBColor(32, 32, 32)
            BLACK = RGBColor(0, 0, 0)
            BLUE = RGBColor(0, 0, 255)
            BROWN = RGBColor(96, 48, 0)
            DARK_RED = RGBColor(128, 0, 0)
            DEEP_GRAY = RGBColor(64, 64, 64)
            DEEP_GREEN = RGBColor(0, 96, 0)
            GRAY = RGBColor(128, 128, 128)
            LIGHT_BLUE = RGBColor(0, 255, 255)
            LIGHT_GRAY = RGBColor(192, 192, 192)
            LIGHT_GREEN = RGBColor(0, 255, 0)
            NAVY_BLUE = RGBColor(0, 0, 64)
            ORANGE = RGBColor(255, 128, 0)
            RED = RGBColor(255, 0, 0)
            YELLOW = RGBColor(255, 255, 0)
            WHITE = RGBColor(255, 255, 255)

            TARGET_WHITE = RGBColor(255, 255, 255)
            TARGET_BLACK = RGBColor(0, 0, 0)
            TARGET_BLUE = RGBColor(65, 181, 200)
            TARGET_RED = RGBColor(255, 37, 21)
            TARGET_GOLD = RGBColor(255, 245, 55)

            TARGET_BLUE_6 = RGBColor(17, 165, 255)
            TARGET_BLUE_NFAA = RGBColor(63, 63, 95)

#=====   end of   src.Utils.rgb_color   =====#
***/