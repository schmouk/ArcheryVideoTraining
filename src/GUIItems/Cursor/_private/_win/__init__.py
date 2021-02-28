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

import win32api
import win32con

from .._cursor_base                  import _CursorBase
from src.GUIItems.Cursor.cursor_id   import CursorID


#=============================================================================
class _Cursor( _CursorBase ):
    """The class of system cursor.
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
        super().__init__( cursor_id, restore_normal_cursor )

    #-------------------------------------------------------------------------
    def __del__(self) -> None:
        '''Releases any allocated resource and restores previous cursor.
        '''
        if self._restore_normal_cursor:
            win32api.SetCursor( self._SYS_MAP[ CursorID.NORMAL ] )

    #-------------------------------------------------------------------------
    def activate(self) -> None:
        '''Activates this cursor and shows it.
        '''
        try:
            win32api.SetCursor( win32api.LoadCursor(0, self._SYS_MAP[ self.cursor_id ]) )
            self.show()
        except:
            pass

    #-------------------------------------------------------------------------
    def get_pos(self) -> Tuple[int,int]:
        '''Returns the (x, y) current position of the cursor.
        '''
        return win32api.GetCursorPos()
    
    #-------------------------------------------------------------------------
    def hide(self) -> None:
        '''Hides this cursor.
        '''
        win32api.ShowCursor( False )

    #-------------------------------------------------------------------------
    def set_pos(self, x: int, y: int) -> None:
        '''Forces the cursor position.
        
        Args:
            x, y: int
                The new position of the cursor.
        '''
        win32api.SetCursorPos( (x, y) )
        
    #-------------------------------------------------------------------------
    def show(self) -> None:
        '''Shows this cursor.
        '''
        win32api.ShowCursor( True )

    #-------------------------------------------------------------------------
    _SYS_MAP = {
        CursorID.NORMAL         : win32con.IDC_ARROW      ,
        CursorID.CROSSHAIR      : win32con.IDC_CROSS      ,
        CursorID.HAND           : win32con.IDC_HAND       ,
        CursorID.HELP           : win32con.IDC_HELP       ,
        CursorID.NO             : win32con.IDC_NO         ,
        CursorID.SIZE           : win32con.IDC_SIZEALL    ,
        CursorID.SIZE_DOWN      : win32con.IDC_SIZENS     ,
        CursorID.SIZE_DOWN_LEFT : win32con.IDC_SIZENESW   ,
        CursorID.SIZE_DOWN_RIGHT: win32con.IDC_SIZENWSE   ,
        CursorID.SIZE_LEFT      : win32con.IDC_SIZEWE     ,
        CursorID.SIZE_LEFT_RIGHT: win32con.IDC_SIZEWE     ,
        CursorID.SIZE_RIGHT     : win32con.IDC_SIZEWE     ,
        CursorID.SIZE_UP        : win32con.IDC_UPARROW    ,
        CursorID.SIZE_UP_DOWN   : win32con.IDC_SIZENS     ,
        CursorID.SIZE_UP_LEFT   : win32con.IDC_SIZENWSE   ,
        CursorID.SIZE_UP_RIGHT  : win32con.IDC_SIZENESW   ,
        CursorID.TEXT           : win32con.IDC_IBEAM      ,
        CursorID.WAIT           : win32con.IDC_WAIT       ,
        CursorID.WAIT_ARROW     : win32con.IDC_APPSTARTING,
    }
    
#=====   end of   src.GUIItems.Cursor._private._win.__init__   =====#
