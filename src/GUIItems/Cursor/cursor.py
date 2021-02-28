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
## This module defines:
#
#    class Cursor
#
#  and specific instances:
#    Cursor_NORMAL         
#    Cursor_CROSSHAIR      
#    Cursor_HAND           
#    Cursor_HELP           
#    Cursor_NO             
#    Cursor_SIZE           
#    Cursor_SIZE_DOWN      
#    Cursor_SIZE_DOWN_LEFT 
#    Cursor_SIZE_DOWN_RIGHT
#    Cursor_SIZE_LEFT      
#    Cursor_SIZE_LEFT_RIGHT
#    Cursor_SIZE_RIGHT     
#    Cursor_SIZE_UP        
#    Cursor_SIZE_UP_DOWN   
#    Cursor_SIZE_UP_LEFT   
#    Cursor_SIZE_UP_RIGHT  
#    Cursor_TEXT           
#    Cursor_WAIT           
#    Cursor_WAIT_ARROW     


#=============================================================================
from .          import _Cursor  # notice: automatically specified according to the underlying operating system
from .cursor_id import CursorID


#=============================================================================
class Cursor( _Cursor ):
    """The base class for all cursors.
    
    This base class inherits from class _Cursor which is implemented
    in private modules that are each dedicated to one platform (i.e.
    Windows, MacOS, Linux or Java).
    
    As such, class Cursor gets access to next methods:
    
        - __init__(self, cursor_id            : int        ,
                         restore_normal_cursor: bool = True ) -> None:
        '''Constructor.
        
        Args:
            cursor_id: int
                The applicative identifier of this cursor.
            restore_normal_cursor: bool
                Set this to True to restore the 'normal' cursor
                image  after deletion of this cursor.  Set this
                to False to not  restore  the  'normal'  cursor
                otherwise. Defaults to True.
        '''

        - activate(self) -> None:
            '''Activates this cursor and shows it.'''
    
        - get_pos(self) -> Tuple[int,int]:
            '''Returns the (x, y) current position of the cursor.'''
    
        - hide(self) -> None:
            '''Hides this cursor.'''
    
        - set_pos(self, x: int, y: int) -> None:
            '''Forces the cursor position.'''
    
        - show(self) -> None:
            '''Shows this cursor.'''
    """

#=============================================================================

Cursor_NORMAL          = Cursor( CursorID.NORMAL         , True )
Cursor_CROSSHAIR       = Cursor( CursorID.CROSSHAIR      , True )
Cursor_HAND            = Cursor( CursorID.HAND           , True )
Cursor_HELP            = Cursor( CursorID.HELP           , True )
Cursor_NO              = Cursor( CursorID.NO             , True )
Cursor_SIZE            = Cursor( CursorID.SIZE           , True )
Cursor_SIZE_DOWN       = Cursor( CursorID.SIZE_DOWN      , True )
Cursor_SIZE_DOWN_LEFT  = Cursor( CursorID.SIZE_DOWN_LEFT , True )
Cursor_SIZE_DOWN_RIGHT = Cursor( CursorID.SIZE_DOWN_RIGHT, True )
Cursor_SIZE_LEFT       = Cursor( CursorID.SIZE_LEFT      , True )
Cursor_SIZE_LEFT_RIGHT = Cursor( CursorID.SIZE_LEFT_RIGHT, True )
Cursor_SIZE_RIGHT      = Cursor( CursorID.SIZE_RIGHT     , True )
Cursor_SIZE_UP         = Cursor( CursorID.SIZE_UP        , True )
Cursor_SIZE_UP_DOWN    = Cursor( CursorID.SIZE_UP_DOWN   , True )
Cursor_SIZE_UP_LEFT    = Cursor( CursorID.SIZE_UP_LEFT   , True )
Cursor_SIZE_UP_RIGHT   = Cursor( CursorID.SIZE_UP_RIGHT  , True )
Cursor_TEXT            = Cursor( CursorID.TEXT           , True )
Cursor_WAIT            = Cursor( CursorID.WAIT           , True )
Cursor_WAIT_ARROW      = Cursor( CursorID.WAIT_ARROW     , True )

#=====   end of   src.GUIItems.Cursor.cursor   =====#
