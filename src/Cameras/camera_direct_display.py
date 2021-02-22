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
from typing import ForwardRef

from .camera_acquisition         import CameraAcquisition
from src.Utils.periodical_thread import PeriodicalThread


#=============================================================================
CameraViewRef = ForwardRef( "CameraView" )


#=============================================================================
class CameraDirectDisplay( PeriodicalThread ):
    """The class of cameras direct displays.
    
    Direct display is "unbuffered" display of captured  frames.
    The "unbuffered" nature of this display is to be understood 
    as an as little buffering  as  possible  to  ensure  smooth 
    display  of captured frames,  since it finally appears that 
    OpenCV capturing of webcams is not that periodical.
    """
    #-------------------------------------------------------------------------
    def __init__(self, cam_acq: CameraAcquisition, view: CameraViewRef) -> None:
        '''Constructor.
        
        Args:
            cam_acq: CameraAcquisition
                A reference to the associated instance of a 
                camera acquisition.
        '''
        self.cam_acq  = cam_acq
        self.cam_view = view
        super().__init__( self.period, f"cam-displ-{cam_acq.cam_id}-thrd" )

    #-------------------------------------------------------------------------
    @property
    def period(self) -> float:
        return self.cam_acq.period

    #-------------------------------------------------------------------------
    def process(self) -> bool:
        '''The processing core of this periodical thread.
        
        Returns:
            True if processing is to be kept on,  or False  if
            this thread must be definitively stopped.
        '''
        frame = self.cam_acq.get_frame()
        if frame is None:
            return False
        else:
            self.cam_view.draw_frame( frame )
            return True

    #-------------------------------------------------------------------------
    def run(self) -> None:
        '''Preparation of the running loop pf this thread.
        '''
        # synchronization with camera acquisition
        self.cam_acq.sync_event.wait()
        
        # then periodical processing
        super().run()
        
#=====   end of   src.Cameras.camera_direct_display   =====#
