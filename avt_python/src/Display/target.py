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

from .view import View


#=============================================================================
class Target:
    """The class of targets.
    
    Describes their features and their "proportional" display.
    """
    #-------------------------------------------------------------------------
    def __init__(self, name    : str,
                       img_path: str,
                       size_cm : int ) -> None:
        '''Constructor.
        
        Args:
            name: str
                The name that identifies this type of target.
            img_path: str
                The file path to the related image.
            true_size: int
                The radius of this target, expressed in cm.

        Raises:
            FileNotFoundErr:  the picture file of this target 
                cannot be found.
            PermissionError:  access cannot be granted to the
                file that contains this target picture.
        '''
        self.name = name
        self.size_cm = size_cm
        self.file_path = img_path

    #-------------------------------------------------------------------------
    def draw(self, view: View, ratio: float) -> None:
        '''Draws this target resized in specified view.
        
        Args:
            view: View
                A reference to the embedding view.
            ratio: float
                The ratio to apply for the resizing 
                of the picture of this target.
        '''
        target_size = round( self.size_cm * ratio )
        
        # evaluates x-offsets
        if target_size < view.width:
            view_x = (view.width - target_size) // 2
            targ_x = 0
            width = target_size
        else:
            view_x = 0
            targ_x = (target_size - view.width) // 2
            width = view.width
        
        # evaluates y-offsets
        if target_size < view.height:
            view_y = (view.height - target_size) // 2
            targ_y = 0
            height = target_size
        else:
            view_y = 0
            targ_y = (target_size - view.height) // 2
            height = view.height
        
        # insertion of target picture in view content
        if target_size < view.width or target_size < view.height:
            view.fill_background()
        view.content[ view_y:view_y+height,
                      view_x:view_x+width , : ] = self.resized_picture[ targ_y:targ_y+height,
                                                                        targ_x:targ_x+width , : ]
        
    #-------------------------------------------------------------------------
    def get_picture(self) -> np.ndarray:
        '''Returns the whole picture of this target.
        
        Returns:
            A reference to a numpy array which contains  the
            whole picture of this target.
        
        Raises:
            FileNotFoundErr: the picture file of this target 
                cannot be found.
            PermissionError: access cannot be granted to the
                file that contains this target picture.
        '''
        return cv2.imread( self.file_path )

    #-------------------------------------------------------------------------
    def get_resized(self, ratio: float) -> np.ndarray:
        '''Returns a resized picture of this target.
        
        Internally modifies also the resized target content
        which is the internally used for its drawing.
        
        Args:
            ratio: float
                The ratio to apply for the resizing of  the 
                picture of this target.
        
        Returns:
            A reference to a numpy array which contains the
            resized picture of this target.
        
        Raises:
            FileNotFoundErr: the picture file of this target 
                cannot be found.
            PermissionError: access cannot be granted to the
                file that contains this target picture.
        '''
        self.resized_picture = cv2.resize( self.get_picture(), 
                                           None, 
                                           fx=ratio, fy=ratio, 
                                           interpolation=cv2.INTER_CUBIC )
        return self.self.resized_picture

#=====   end of   src.Display.target   =====#
