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

from .camera                             import Camera
from src.Buffers.camera_frames_buffer    import CameraFramesBuffer
from src.Utils.periodical_thread         import PeriodicalThread


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
    def __init__(self, camera      : Camera            ,
                       frame_buffer: CameraFramesBuffer,
                       view        : CameraViewRef      ) -> None:
        '''Constructor.
        
        Args:
            cam_acq: CameraAcquisition
                A reference to the associated instance of a 
                camera acquisition.
        '''
        self.camera      = camera
        self.buffer      = frame_buffer
        self.first_frame = True
        self.cam_view    = view
        super().__init__( self.camera.get_period(), f"cam-displ-{camera.get_id()}-thrd" )

    #-------------------------------------------------------------------------
    @property
    def period(self) -> float:
        return self.period_s

    #-------------------------------------------------------------------------
    def process(self) -> bool:
        '''The processing core of this periodical thread.
        
        Returns:
            True if processing is to be kept on,  or False  if
            this thread must be definitively stopped.
        '''
        indexed_frame = self.buffer.get_oldest()
        
        if indexed_frame.frame is not None:
            if self.first_frame:
                self.first_frame = False
                self.set_start_time()
        
            self.cam_view.draw_frame( indexed_frame.frame )

        return True
        
#=====   end of   src.Cameras.camera_direct_display   =====#
