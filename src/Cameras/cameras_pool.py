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
from types   import TracebackType
from typing  import ForwardRef, Tuple, Type

from src.App.avt_config      import AVTConfig
from .camera                 import Camera


#=============================================================================
BaseExceptionType = Type[ BaseException ]
CamerasPoolRef    = ForwardRef( "CamerasPool" )
ExceptionInfo     = Tuple[ BaseExceptionType, BaseException, TracebackType ]


#=============================================================================
class  CamerasPool( list ):
    """The class of the pool of cameras.
    
    The AVT application may involve many  cameras. 
    They all are managed within a pool of cameras.
    """
    #-------------------------------------------------------------------------
    def __init__(self) -> None:
        '''Constructor.
        
        Instantiates the pool of cameras.
        '''
        super().__init__()
        self.evaluate_connected_cameras()

    #-------------------------------------------------------------------------
    def __del__(self) -> None:
        '''Releases all allocated resources.
        '''
        for camera in self:
            del camera

    #-------------------------------------------------------------------------
    def evaluate_connected_cameras(self) -> None:
        '''Evaluates all the connected cameras.
        
        Initializes the pool of cameras according
        to the currently connected ones.
        '''
        self.clear()

        for camera_index in range( AVTConfig.CAMERAS_MAX_COUNT ):
            
            camera = Camera( camera_index )
            
            if camera.is_ok():
                self.append( camera )
            else:
                ##del camera
                break

#=====   end of   src.Cameras.cameras_pool   =====#
