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
from typing import ForwardRef, Optional

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
    def top_left(self, pt: Point) -> None:
        self.x, self.y = pt.x, pt.y
    
    @property
    def bottom_right(self) -> Point:
        return Point( self.right_x, self.bottom_y )
    @bottom_right.setter
    def bottom_right(self, pt: Point) -> None:
        self.right_x  = pt.x
        self.bottom_y = pt.y
    
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
    def __contains__(self, point: Point) -> bool:
        '''Implementation of keyword 'in'.
        
        Args:
            point: Point
                A reference to a point whose inclusion in this
                rectangle is to be checked.
        
        Returns:
            True if the specified point belongs to this  rect-
            angle, or returns False otherwise.
        '''
        return ( self.x <= point.x <= self.right_x  and
                 self.y <= point.y <= self.bottom_y     )

#=====   end of   src.Shapes.rect   =====#
