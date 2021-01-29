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
from typing  import ForwardRef, List, Tuple, Type

from src.App.avt_config      import AVTConfig
from .camera                 import Camera
from .camera_acquisition     import CameraAcquisition
from src.Utils.indexed_frame import IndexedFrame


#=============================================================================
BaseExceptionType = Type[ BaseException ]
CamerasPoolRef    = ForwardRef( "CamerasPool" )
ExceptionInfo     = Tuple[ BaseExceptionType, BaseException, TracebackType ]


#=============================================================================
class CamerasPool( list ):
    """The class of the pool of cameras.
    
    The AVT application may involve many cameras. They all are
    managed within a pool of cameras.
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
        
        Initializes the pool  of  cameras  according  to  the
        currently connected ones. Sets attribute '.cams_pool'.
        '''
        self.clear()

        for camera_index in range( AVTConfig.CAMERAS_MAX_COUNT ):
            
            camera_acq = CameraAcquisition( Camera(camera_index) )
            
            if camera_acq.is_ok():
                self.append( camera_acq )
            else:
                del camera_acq
                break

    #-------------------------------------------------------------------------
    def get_frames(self) -> List[IndexedFrame]:
        '''Acquires next frames on all cameras.
        
        Returns:
            A list of acquired frames.
        '''
        return [ cam_acq.get() for cam_acq in self ]

    #-------------------------------------------------------------------------
    def start_acquisitions(self) -> None:
        '''Starts the acquisition of all videos.
        '''
        print( "cameras-pool starts acquisitions" )
        
        for cam_acq in self:
            cam_acq.start()

    #-------------------------------------------------------------------------
    def stop(self) -> None:
        '''Definitively stops the pool of cameras.
        '''
        # stops all cameras threads
        for cam_acq in self:
            cam_acq.stop()
        
        # then joins them
        for cam_acq in self:
            cam_acq.join()

    #-------------------------------------------------------------------------
    def __enter__(self) -> CamerasPoolRef:
        '''Implementation of the Context Manager Protocol (1/2).
        '''
        print( "entering cameras-pool" )
        
        self.start_acquisitions()
        return self

    #-------------------------------------------------------------------------
    def __exit__(self, except_type     : BaseExceptionType = None,
                       except_value    : BaseException     = None,
                       except_traceback: TracebackType     = None ) -> bool:
        '''Implementation of the Context Manager Protocol (2/2).
        
        Args:
            except_type: BaseExceptionType
                A reference to the  type  of  caught  exception.
                Defaults to None.
            except_value: BaseException
                A reference to the exception itself. Defaults to 
                None.
            except_traceback: TracebackType
                A reference to the type of traceback  associated
                with the caught exception. Defaults to None.
        
        Returns:
            False as a default for raised exception to be raised 
            again.
        '''
        try:
            self.stop()
            
            if except_type is not None:
                if except_value is not None:
                    print( f"!!! {except_value}" )
                else:
                    print( "!!! Unknown exception raised" )
                if except_traceback is not None:
                    print( f"{except_traceback}" )
                else:
                    print( "    No traceback to print" )
        
        finally:
            return False

#=====   end of   src.Cameras.cameras_pool   =====#
