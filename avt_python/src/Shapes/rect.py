#!/usr/bin/env python
# -*- coding: utf-8 -*-
"""
Copyright (c) 2021 Philippe Schmouker

Permission is hereby granted,  free of charge,  to any person obtaining a copy
of this software and associated documentation files (the "Software"),  to deal
in the Software without restriction, including  without  limitation the rights
to use,  copy,  modify,  merge,  publish,  distribute, sublicense, and/or sell
copies of the Software,  and  to  permit  persons  to  whom  the  Software  is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS",  WITHOUT WARRANTY OF ANY  KIND,  EXPRESS  OR
IMPLIED,  INCLUDING  BUT  NOT  LIMITED  TO  THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT  SHALL  THE
AUTHORS  OR  COPYRIGHT  HOLDERS  BE  LIABLE  FOR  ANY CLAIM,  DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT,  TORT OR OTHERWISE, ARISING FROM,
OUT  OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
"""

#=============================================================================
from typing import ForwardRef, Optional, Union

from .offset import Offset
from .point  import Point
from .size   import Size


#=============================================================================
RectRef = ForwardRef( "Rect" )


#=============================================================================
class Rect:
    """The class of Rectangles.
    """
    #-------------------------------------------------------------------------
    def __init__(self, x     : int = 0,
                       y     : int = 0,
                       width : int = 0,
                       height: int = 0,
                       *,
                       pos   : Point   = None,
                       bot   : Point   = None,
                       size  : Size    = None,
                       copy  : RectRef = None ) -> None:
        '''Constructor.
        
        Args:
            x, y: int
                Position of top-left corner of this rectangle. 
                Defaults both to 0.
            width: int
                The width of this rectangle. Defaults to 0.
            height: int
                The height of this rectangle. Defaults to 0.
            pos: Point
                The position of the top-left  corner  of  this
                rectangle. If set, takes precedence over x and 
                y. Defaults to None.
            bot: Point
                The position of  the  bottom-right  corner  of
                this rectangle.  If set, takes precedence over
                width and height. Defaults to None.
            size: Size
                The size (i.e.  width  then  height)  of  this
                rectangle. If set, takes precedence over width
                and height.  If either pos or bot is  set  but
                are  not both set,  either width and height or 
                size must be set. Defaults to None. 
            copy: Rect
                A reference to a rectangle to  copy.  If  set, 
                takes  precedence  over  all  other arguments.
                Defaults to None.
        '''
        if copy is not None:
            self.copy( copy )
        
        elif pos is not None:
            self.x, self.y = pos.x, pos.y
            if size is not None:
                self.width, self.height = size.width, size.height
            elif bot is not None:
                self.width  = bot.x - pos.x + 1
                self.height = bot.y - pos.y + 1
            else:
                self.width, self.height = width, height
            
        elif bot is not None:
            if size is not None:
                self.x = bot.x - size.width + 1
                self.y = bot.y - size.height + 1
                self.width, self.height = size.width, size.height
            else:
                self.x, self.y = x, y
                self.width  = bot.x - x + 1
                self.height = bot.y - y + 1
            
        elif size is not None:
            self.x, self.y = x, y
            self.width, self.height = size.width, size.height
            
        else:
            self.x, self.y = x, y
            self.width, self.height = width, height

    #-------------------------------------------------------------------------
    @property
    def left_x(self) -> int:
        return self.x
    @left_x.setter
    def left_x(self, value: int) -> None:
        self.x = value

    @property
    def right_x(self) -> int:
        return self.x + self.width - 1
    @right_x.setter
    def right_x(self, value: int) -> None:
        self.width = value - self.x + 1
    
    @property
    def bottom_y(self) -> int:
        return self.y + self.height - 1
    @bottom_y.setter
    def bottom_y(self, value: int) -> None:
        self.height = value - self.y + 1
    
    @property
    def top_y(self) -> int:
        return self.y
    @top_y.setter
    def top_y(self, value: int) -> None:
        return self.y + self.height - 1

    @property
    def top_left(self) -> Point:
        return Point( self.x, self.y )
    @top_left.setter
    def top_left(self, pt: Union[Offset,Point]) -> None:
        self.x, self.y = pt.x, pt.y
    
    @property
    def bottom_right(self) -> Point:
        return Point( self.right_x, self.bottom_y )
    @bottom_right.setter
    def bottom_right(self, pt: Point) -> None:
        self.right_x  = pt.x
        self.bottom_y = pt.y
    
    @property
    def size(self) -> Size:
        return Size( self.width, self.height )
    @size.setter
    def size(self, sz: Size) -> None:
        self.width, self.height = sz.width, sz.height
    
    @property
    def surface(self) -> int:
        return self.width * self.height
        
    #-------------------------------------------------------------------------
    def copy(self, other: RectRef = None) -> Optional[ RectRef ]:
        '''Copy of rectangles.
        
        Args:
            other: Rect
                If set, this is a reference to a rectangle to
                be copied in this one.  If not set, returns a
                copy of this rectangle.
        
        Returns:
            If 'other' is set, returns nothing. If no argument
            is  provided,  returns a new instance of Rect with
            the attributes of this rect copied in it.
        '''
        if other is None:
            return Rect( self.x, self.y, self.width, self.height )
        else:
            self.x, self.y = other.x, other.y
            self.width, self.height = other.width, other.height
                
    #-------------------------------------------------------------------------
    def move(self, offset_or_dx: Union[Offset, int], dy: int = None) -> RectRef:
        '''Relative moves of this rectangl"e according to some offset.
        
        Args:
            offset_or_dx: Offset or int
                If this is an instance  of  class  Offset, 
                argument  dy must be None.  It defines the 
                offset to  be   applied  to  the  top-left 
                position  of  this  rectangle.  If this is
                an integer,  it  is  the  X-offset  to  be
                applied  to  the top-left position of this 
                rectangle and dy must be set.
            dy: int
                The Y-offset to be applied to the top-left
                position of this rectangle. Must be set if
                offset_or_dx is  an  integer.  Ignored  if
                offset_or_dx  is  an  instance  of Offset.
                Defaults to None.
        
        Returns:
            A reference to this rectangle. This allows the
            cascading of methods calls.
        
        Raises:
            AssertionError:  dy is None while  first  arg-
                ument is not.
        '''
        if isinstance( offset_or_dx, Offset ):
            self.top_left += offset_or_dx
        else:
            assert dy is not None
            self.x += offset_or_dx
            self.y += dy
        return self

    #-------------------------------------------------------------------------
    def resize(self, new_size_or_width: Size, new_height: int = None) -> RectRef:
        '''Resizes this rectangle.
        
        Args:
            new_size_or_width: Size or int
                Either a reference to a Size instance, or
                an  int  specifying the new width of this
                rectangle.  If instance of Size, argument
                height is ignored.  If integer,  argument
                height must be set.
            new_height: int 
                The new value  for  the  height  of  this 
                rectangle.  Must be set if frist argument
                is an integer.  Ignored if first argument
                is an instance of Size. Defaults to None.
        
        Returns:
            A  reference  to  this rectangle. This allows 
            the cascading of methods calls.
        '''
        if isinstance( new_size_or_width, Size ):
            self.size = new_size_or_width
        else:
            assert new_height is not None
            self.width, self.height = new_size_or_width, new_height

    #-------------------------------------------------------------------------
    def __contains__(self, point: Point) -> bool:
        '''Implementation of keyword 'in'.
        
        Args:
            point: Point
                A reference to a point whose inclusion  in 
                this rectangle is to be checked.
        
        Returns:
            True if the specified point  belongs  to  this 
            rectangle, or returns False otherwise.
        
        Raises:
            AssertionError:  ew_height is None while first
                argument is not.
        '''
        return ( self.left_x <= point.x <= self.right_x  and
                 self.top_y  <= point.y <= self.bottom_y     )

    #-------------------------------------------------------------------------
    def __mul__(self, coeff: float) -> RectRef:
        '''Scales this rectangle, in pos and size.
        
        Returns:
            A reference to a new Rect.
        '''
        return Rect( round( coeff * self.x ),
                     round( coeff * self.y ),
                     round( coeff * self.width ),
                     round( coeff * self.height ) )

    #-------------------------------------------------------------------------
    def __imul__(self, coeff: float) -> RectRef:
        '''In-place scales this rectangle, in pos and size.
        
        Returns:
            A reference to this rectangle.
        '''
        self.x = round( coeff * self.x )
        self.y = round( coeff * self.y )
        self.width = round( coeff * self.width )
        self.height = round( coeff * self.height )
        return self

    #-------------------------------------------------------------------------
    def __rmul__(self, coeff: float) -> RectRef:
        '''Scales this rectangle, in pos and size.
        
        Returns:
            A reference to a new Rect.
        '''
        return self.__mul__( coeff )

    #-------------------------------------------------------------------------
    def __str__(self) -> str:
        '''Well, debug purposes...
        '''
        return f"({self.x}, {self.y}, {self.width}, {self.height})"
    
#=====   end of   src.Shapes.rect   =====#
