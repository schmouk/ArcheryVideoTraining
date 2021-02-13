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
    def draw(self, view: View, new_size_cm: int ) -> None:
        '''Draws this target resized in specified view.
        
        Args:
            view: View
                A reference to the embedding view.
            new_size_cm: int
                The size for the resizing  of  the 
                picture of this target.
        '''
        target_pict = self.get_resized( new_size_cm )
        
        # evaluates x-offsets
        if new_size_cm < view.width:
            view_x = (view.width - new_size_cm) // 2
            targ_x = 0
            width = new_size_cm
        else:
            view_x = 0
            targ_x = (new_size_cm - view.width) // 2
            width = view.width
        
        # evaluates y-offsets
        if new_size_cm < view.height:
            view_y = (view.height - new_size_cm) // 2
            targ_y = 0
            height = new_size_cm
        else:
            view_y = 0
            targ_y = (new_size_cm - view.height) // 2
            height = view.height
        
        # insertion of target picture in view content
        if new_size_cm < view.width or new_size_cm < view.height:
            view.fill_background()
        view.content[ view_y:view_y+height,
                      view_x:view_x+width , : ] = target_pict[ targ_y:targ_y+height,
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
    def get_resized(self, new_size_cm: int) -> np.ndarray:
        '''Returns a resized picture of this target.
        
        Args:
            new_size_cm: int
                The size for the resizing of the picture of
                this target.
        
        Returns:
            A reference to a numpy array which contains the
            resized picture of this target.
        
        Raises:
            FileNotFoundErr: the picture file of this target 
                cannot be found.
            PermissionError: access cannot be granted to the
                file that contains this target picture.
        '''
        ratio = new_size_cm / self.size_cm
        return cv2.resize( self.get_picture(), 
                           None, 
                           fx=ratio, fy=ratio, 
                           interpolation=cv2.INTER_CUBIC )

#=====   end of   src.Display.target   =====#
