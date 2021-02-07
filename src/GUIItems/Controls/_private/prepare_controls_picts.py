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


#=============================================================================

#-------------------------------------------------------------------------
def prepare_camera_button() -> None:
    '''Preparation of the camera buttons.
    '''
    print( 'camera button: ', end='' )
    try:
        img = cv2.imread( '../../../../picts/controls/raw/switch-off.png' )
        gray_comp = 96
        for y in range(30,38):
            for x in range(104,134):
                if img[y][x][0] >= gray_comp:
                    img[y][x] //= 2
        img = cv2.resize( img, (86,35), interpolation=cv2.INTER_CUBIC )
        cv2.imwrite( f'../../../../picts/controls/switch-off.png', img )

        img = cv2.imread( '../../../../picts/controls/raw/switch-on.png' )
        gray_comp = 96
        for y in range(22,49):
            for x in range(38,63):
                if img[y][x][0] >= gray_comp:
                    img[y][x] //= 2
        img = cv2.resize( img, (86,35), interpolation=cv2.INTER_CUBIC )
        cv2.imwrite( f'../../../../picts/controls/switch-on.png', img )

        img = cv2.imread( '../../../../picts/controls/raw/switch-off.png' )
        gray_comp = 96
        for y in range(30,38):
            for x in range(104,134):
                if img[y][x][0] >= gray_comp:
                    img[y][x] //= 5
        for y in range(25,47):
            for x in range(32,54):
                if img[y][x][0] >= gray_comp:
                    img[y][x] //= 5
        img = cv2.resize( img, (86,35), interpolation=cv2.INTER_CUBIC )
        cv2.imwrite( f'../../../../picts/controls/switch-disabled.png', img )

        print( ' ok' )

    except Exception as e:
        print( 'failed due to exception', str(e) )
    
    
#-------------------------------------------------------------------------
def prepare_exit_button() -> None:
    '''Preparation of the exit button.
    '''
    print( 'exit button: ', end='' )
    try:
        comp_yellow = 225  ##115  ##
        comp_blue   = 135
        exit_img = cv2.imread( '../../../../picts/controls/raw/exit.png' )
        exit_img[ exit_img > comp_yellow ] = comp_yellow
        
        exit_img[ exit_img[ :,:,0 ] > comp_yellow-20, 0 ] = comp_blue

        if exit_img.shape[0] != exit_img.shape[1]:
            # lets square this image
            height, width = exit_img.shape[:2]
            if height < width:
                img = np.zeros( (width, width, 3), np.uint8 ) + 32
            else:
                img = np.zeros( (height, height, 3), np.uint8 ) + 32
            dx = (img.shape[1] - width) // 2
            dy = (img.shape[0] - height) // 2
            img[ dy:dy+height, dx:dx+width, : ] = exit_img[:,:,:]
        else:
            img = exit_img
            
        cv2.imwrite( f'../../../../picts/controls/exit-{img.shape[0]}.png', img )
        
        img_64 = cv2.resize( img, (64,64), interpolation=cv2.INTER_CUBIC )
        img_64 = cv2.circle( img_64, (31,31), 26, (comp_blue, comp_yellow, comp_yellow), 2, cv2.LINE_AA )
        img_64 = cv2.circle( img_64, (31,31), 28, (0,0,0), 2, cv2.LINE_AA )
        cv2.imwrite( '../../../../picts/controls/exit-64.png', img_64 )
        
        img_48 = cv2.resize( img, (48,48), interpolation=cv2.INTER_CUBIC )
        img_48 = cv2.circle( img_48, (23,23), 20, (comp_blue, comp_yellow, comp_yellow), 2, cv2.LINE_AA )
        img_48 = cv2.circle( img_48, (23,23), 22, (0,0,0), 2, cv2.LINE_AA )
        cv2.imwrite( '../../../../picts/controls/exit-48.png', img_48 )
        
        img_32 = cv2.resize( img, (32,32), interpolation=cv2.INTER_CUBIC )
        img_32 = cv2.circle( img_32, (15,15), 13, (comp_blue, comp_yellow, comp_yellow), 1, cv2.LINE_AA )
        img_32 = cv2.circle( img_32, (15,15), 14, (0,0,0), 1, cv2.LINE_AA )
        cv2.imwrite( '../../../../picts/controls/exit-32.png', img_32 )

        print( ' ok' )

    except Exception as e:
        print( 'failed due to exception', str(e) )


#-------------------------------------------------------------------------


#=============================================================================
if __name__ == '__main__':
    """Script description.
    """
    #-------------------------------------------------------------------------
    prepare_camera_button()
    prepare_exit_button()


#=====   end of   src.GUIItems.Controls._private.prepare_controls_picts   =====#
