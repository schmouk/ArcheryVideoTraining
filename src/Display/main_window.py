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
import cv2
import numpy as np
import time
from typing import Tuple

from src.App                     import __version__
from .avt_window                 import AVTWindow
from src.Cameras.cameras_pool    import CamerasPool
from .camera_view                import CameraView
from .control_view               import ControlView
from src.Shapes.rect             import Rect
from .target_view                import TargetView
from .view                       import View


#=============================================================================
class MainWindow( AVTWindow):
    """The class of the AVT app main window singleton.
    
    This is an OpenCV window which is split in views.
    """
    #-------------------------------------------------------------------------
    def __init__(self) -> None:
        '''Constructor.
        '''
        if self.__SINGLETON is None:
            # creates the Main Window for app AVT
            print( f"creating MainWindow( 0, 0, {self.DEFAULT_WIDTH}, {self.DEFAULT_HEIGHT} )" )
            super().__init__( name="MainAVT",
                              title=f"Archery Video Training - {__version__}",
                              width=self.DEFAULT_WIDTH,
                              height=self.DEFAULT_HEIGHT )
            MainWindow.__SINGLETON = self
            
            # creates the embedded views, according to the pool of cameras
            print( "creates cameras-pool" )
            self.cameras_pool = CamerasPool()
            print( "creates main-window views" )
            self.create_views( self.cameras_pool, False )
            
        else:
            self = MainWindow.__SINGLETON

    #-------------------------------------------------------------------------
    def __del__(self) -> None:
        '''Destructor.
        
        Releases all allocated resources.
        '''
        del self.cameras_pool

    #-------------------------------------------------------------------------
    def create_views(self, cameras_pool : CamerasPool,
                           b_target_view: bool = False) -> None:
        '''Creates all the views that are contained in this window.
        
        Args:
            cameras_pool: CamerasPool
                A reference to the pool of cameras that have
                been detected as connected to the AVT app.
            b_target_view: bool
                Set this to True to a target view that  will
                be used to help at aiming at screen,  or set
                it to False otherwise. Defaults to False.
        '''
        cameras_count = len( cameras_pool )
        
        camera_views_width = self.width - ControlView.WIDTH
        rect = Rect( 0, 0, camera_views_width, self.height )
        
        if cameras_count == 0:
            self.views = [ ControlView( self ),
                           TargetView( self, 0.0, 0.0, 1.0, 1.0, rect ) ]
                  
        elif cameras_count == 1:
            if b_target_view:
                self.views = [ ControlView( self ),
                               CameraView( self, cameras_pool[0], 0.0, 0.0, 1.0, 0.5, rect ),
                               TargetView( self, 0.5, 0.5, 0.5, 1.0, rect )     ]
            else:
                self.views = [ ControlView( self ),
                               CameraView( self, cameras_pool[0], 0.0, 0.0, 1.0, 1.0, rect ) ]
                  
        elif cameras_count == 2:
            self.views = [ ControlView( self ),
                           CameraView( self, cameras_pool[0], 0.0, 0.0, 0.5, 0.5, rect ),
                           CameraView( self, cameras_pool[1], 0.5, 0.0, 0.5, 0.5, rect )  ]
            if b_target_view:
                self.views.append( TargetView( self, 0.0, 0.5, 1.0, 0.5, rect ) )
                  
        elif cameras_count == 3:
            print( "creates views for 3 cameras" )
            self.views = [ ControlView( self ),
                           CameraView( self, cameras_pool[0], 0.0, 0.0, 0.5, 0.5, rect ),
                           CameraView( self, cameras_pool[1], 0.5, 0.0, 0.5, 0.5, rect ),
                           CameraView( self, cameras_pool[2], 0.0, 0.5, 0.5, 0.5, rect )  ]
            if b_target_view:
                self.views.append( TargetView( self, 0.5, 0.5, 0.5, 0.5, rect ) )
          
        elif cameras_count >= 4:
            self.views = [ ControlView( self ),
                           CameraView( self, cameras_pool[0], 0.0, 0.0, 0.5, 0.5, rect ),
                           CameraView( self, cameras_pool[1], 0.5, 0.0, 0.5, 0.5, rect ),
                           CameraView( self, cameras_pool[2], 0.0, 0.5, 0.5, 0.5, rect ),
                           CameraView( self, cameras_pool[3], 0.5, 0.5, 0.5, 0.5, rect )  ]
            if b_target_view:
                self.views.append( TargetView( self, 0.0, 0.0, 1.0, 1.0, rect, True ) )

    #===========================================================================
    # #-------------------------------------------------------------------------
    # def draw(self) -> None:
    #     '''Draws all the views that are contained in this window.
    #     '''
    #     for view in self.views:
    #         view.draw()
    #     super().draw()
    #===========================================================================

    #-------------------------------------------------------------------------
    def get_cameras_area_size(self) -> Tuple[int, int]:
        '''Returns the (width, height) of the cameras displays size in this main window.
        '''
        dims = self.get_size()
        return [ dims[0] - self.DEFAULT_CONTROL_WIDTH, dims[1] ]

    #-------------------------------------------------------------------------
    def run_views(self) -> None:
        '''Runs the threads associated with views, if any.
        '''
        for view in self.views:
            view.start()

    #-------------------------------------------------------------------------
    def stop_views(self) -> None:
        '''Definitively stops the threads associated with views, if any.
        '''
        for view in self.views:
            view.stop()

    #-------------------------------------------------------------------------
    # Class data
    DEFAULT_WIDTH  = 2 * 640 + ControlView.WIDTH
    DEFAULT_HEIGHT = 2 * 480

    __SINGLETON = None

#=====   end of   src.Display.main_window   =====#
