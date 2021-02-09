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
from typing import Tuple

from src.GUIItems.Cursor.cursor_id import CursorID


#=============================================================================
class _CursorBase:
    """The base class for cursor classes dedicated to system platforms.
    """
    #-------------------------------------------------------------------------
    def __init__(self, cursor_id            : int        ,
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
        self._restore_normal_cursor = restore_normal_cursor
        self._cursor_id = cursor_id

    #-------------------------------------------------------------------------
    def __del__(self) -> None:
        '''Releases any allocated resource and restores previous cursor.
        '''
        if self._restore_normal_cursor:
            # TODO: implement this
            ...

    #-------------------------------------------------------------------------
    def activate(self) -> None:
        '''Activates this cursor and shows it.
        '''
        try:
            # sets cursor
            # TODO: implement this
            ...
            
            # shows cursor
            self.show()
        
        except:
            pass

    #-------------------------------------------------------------------------
    def get_pos(self) -> Tuple[int,int]:
        '''Returns the (x, y) current position of the cursor.
        '''
        # TODO: implement this
        return (0, 0)
    
    #-------------------------------------------------------------------------
    def hide(self) -> None:
        '''Hides this cursor.
        '''
        # TODO: implement this
        ...

    #-------------------------------------------------------------------------
    def set_pos(self, x: int, y: int) -> None:
        '''Forces the cursor position.
        
        Args:
            x, y: int
                The new position of the cursor.
        '''
        # TODO: implement this
        ...
        
    #-------------------------------------------------------------------------
    def show(self) -> None:
        '''Shows this cursor.
        '''
        # TODO: implement this
        ...

    #-------------------------------------------------------------------------
    _SYS_MAP = {
        # TODO: implement this
        CursorID.NORMAL         : 0,
        CursorID.CROSSHAIR      : 0,
        CursorID.HAND           : 0,
        CursorID.HELP           : 0,
        CursorID.NO             : 0,
        CursorID.SIZE           : 0,
        CursorID.SIZE_DOWN      : 0,
        CursorID.SIZE_DOWN_LEFT : 0,
        CursorID.SIZE_DOWN_RIGHT: 0,
        CursorID.SIZE_LEFT      : 0,
        CursorID.SIZE_LEFT_RIGHT: 0,
        CursorID.SIZE_RIGHT     : 0,
        CursorID.SIZE_UP        : 0,
        CursorID.SIZE_UP_DOWN   : 0,
        CursorID.SIZE_UP_LEFT   : 0,
        CursorID.SIZE_UP_RIGHT  : 0,
        CursorID.TEXT           : 0,
        CursorID.WAIT           : 0,
        CursorID.WAIT_ARROW     : 0,
    }

#=====   end of   src.GUIItems.Cursor._private._cursor_base   =====#
