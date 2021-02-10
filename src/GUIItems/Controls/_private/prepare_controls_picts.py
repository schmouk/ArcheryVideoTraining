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

from src.App.avt_config  import AVTConfig
from src.Utils.rgb_color import *


#=============================================================================

#-------------------------------------------------------------------------
def create_targets() -> None:
    '''Creation of the shot targets.
    '''
    print( 'targets creation: ', end='' )
    try:
        # 122 cm target
        img = np.zeros( (1223,1223,3), np.uint8 ) + 255
        img = cv2.circle( img, (611,611), 610, TARGET_BLACK.color, 1, cv2.LINE_AA )
        img = cv2.circle( img, (611,611), 610-61, TARGET_BLACK.color, 1, cv2.LINE_AA )
        img = cv2.circle( img, (611,611), 610-122, TARGET_BLACK.color, cv2.FILLED, cv2.LINE_AA )
        img = cv2.circle( img, (611,611), 610-183, TARGET_WHITE.color, 1, cv2.LINE_AA )
        img = cv2.circle( img, (611,611), 610-244, TARGET_BLUE.color, cv2.FILLED, cv2.LINE_AA )
        img = cv2.circle( img, (611,611), 610-305, TARGET_BLACK.color, 1, cv2.LINE_AA )
        img = cv2.circle( img, (611,611), 610-366, TARGET_RED.color, cv2.FILLED, cv2.LINE_AA )
        img = cv2.circle( img, (611,611), 610-366, TARGET_BLACK.color, 1, cv2.LINE_AA )
        img = cv2.circle( img, (611,611), 610-427, TARGET_BLACK.color, 1, cv2.LINE_AA )
        img = cv2.circle( img, (611,611), 610-488, TARGET_GOLD.color, cv2.FILLED, cv2.LINE_AA )
        img = cv2.circle( img, (611,611), 610-488, TARGET_BLACK.color, 1, cv2.LINE_AA )
        img = cv2.circle( img, (611,611), 610-549, TARGET_BLACK.color, 1, cv2.LINE_AA )
        img = cv2.circle( img, (611,611), 610-581, TARGET_BLACK.color, 1, cv2.LINE_AA )
        img = cv2.line( img, (608,611), (614,611), TARGET_BLACK.color, 1, cv2.LINE_AA )
        img = cv2.line( img, (611,608), (611,614), TARGET_BLACK.color, 1, cv2.LINE_AA )
        cv2.imwrite( '../../../../picts/targets/target-122.png', img )

        # 80 cm target
        img = np.zeros( (803,803,3), np.uint8 ) + 255
        img = cv2.circle( img, (401,401), 400, TARGET_BLACK.color, 1, cv2.LINE_AA )
        img = cv2.circle( img, (401,401), 400-40, TARGET_BLACK.color, 1, cv2.LINE_AA )
        img = cv2.circle( img, (401,401), 400-80, TARGET_BLACK.color, cv2.FILLED, cv2.LINE_AA )
        img = cv2.circle( img, (401,401), 400-120, TARGET_WHITE.color, 1, cv2.LINE_AA )
        img = cv2.circle( img, (401,401), 400-160, TARGET_BLUE.color, cv2.FILLED, cv2.LINE_AA )
        img = cv2.circle( img, (401,401), 400-200, TARGET_BLACK.color, 1, cv2.LINE_AA )
        img = cv2.circle( img, (401,401), 400-240, TARGET_RED.color, cv2.FILLED, cv2.LINE_AA )
        img = cv2.circle( img, (401,401), 400-240, TARGET_BLACK.color, 1, cv2.LINE_AA )
        img = cv2.circle( img, (401,401), 400-280, TARGET_BLACK.color, 1, cv2.LINE_AA )
        img = cv2.circle( img, (401,401), 400-320, TARGET_GOLD.color, cv2.FILLED, cv2.LINE_AA )
        img = cv2.circle( img, (401,401), 400-320, TARGET_BLACK.color, 1, cv2.LINE_AA )
        img = cv2.circle( img, (401,401), 400-360, TARGET_BLACK.color, 1, cv2.LINE_AA )
        img = cv2.circle( img, (401,401), 400-380, TARGET_BLACK.color, 1, cv2.LINE_AA )
        img = cv2.line( img, (398,401), (404,401), TARGET_BLACK.color, 1, cv2.LINE_AA )
        img = cv2.line( img, (401,398), (401,404), TARGET_BLACK.color, 1, cv2.LINE_AA )
        cv2.imwrite( '../../../../picts/targets/target-80.png', img )

        # 80 cm compound target
        img = np.zeros( (483,483,3), np.uint8 ) + 255
        img = cv2.circle( img, (241,241), 240, TARGET_BLUE_6.color, cv2.FILLED, cv2.LINE_AA )
        img = cv2.circle( img, (241,241), 240, TARGET_BLACK.color, 1, cv2.LINE_AA )
        img = cv2.circle( img, (241,241), 240-40, TARGET_BLACK.color, 1, cv2.LINE_AA )
        img = cv2.circle( img, (241,241), 240-80, TARGET_RED.color, cv2.FILLED, cv2.LINE_AA )
        img = cv2.circle( img, (241,241), 240-80, TARGET_BLACK.color, 1, cv2.LINE_AA )
        img = cv2.circle( img, (241,241), 240-120, TARGET_BLACK.color, 1, cv2.LINE_AA )
        img = cv2.circle( img, (241,241), 240-160, TARGET_GOLD.color, cv2.FILLED, cv2.LINE_AA )
        img = cv2.circle( img, (241,241), 240-160, TARGET_BLACK.color, 1, cv2.LINE_AA )
        img = cv2.circle( img, (241,241), 240-200, TARGET_BLACK.color, 1, cv2.LINE_AA )
        img = cv2.circle( img, (241,241), 240-220, TARGET_BLACK.color, 1, cv2.LINE_AA )
        img = cv2.line( img, (238,241), (244,241), TARGET_BLACK.color, 1, cv2.LINE_AA )
        img = cv2.line( img, (241,238), (241,244), TARGET_BLACK.color, 1, cv2.LINE_AA )
        cv2.imwrite( '../../../../picts/targets/target-compoud-80.png', img )

        # 60 cm target
        img = np.zeros( (603,603,3), np.uint8 ) + 255
        img = cv2.circle( img, (301,301), 300, TARGET_BLACK.color, 1, cv2.LINE_AA )
        img = cv2.circle( img, (301,301), 300-30, TARGET_BLACK.color, 1, cv2.LINE_AA )
        img = cv2.circle( img, (301,301), 300-60, TARGET_BLACK.color, cv2.FILLED, cv2.LINE_AA )
        img = cv2.circle( img, (301,301), 300-90, TARGET_WHITE.color, 1, cv2.LINE_AA )
        img = cv2.circle( img, (301,301), 300-120, TARGET_BLUE.color, cv2.FILLED, cv2.LINE_AA )
        img = cv2.circle( img, (301,301), 300-150, TARGET_BLACK.color, 1, cv2.LINE_AA )
        img = cv2.circle( img, (301,301), 300-180, TARGET_RED.color, cv2.FILLED, cv2.LINE_AA )
        img = cv2.circle( img, (301,301), 300-180, TARGET_BLACK.color, 1, cv2.LINE_AA )
        img = cv2.circle( img, (301,301), 300-210, TARGET_BLACK.color, 1, cv2.LINE_AA )
        img = cv2.circle( img, (301,301), 300-240, TARGET_GOLD.color, cv2.FILLED, cv2.LINE_AA )
        img = cv2.circle( img, (301,301), 300-240, TARGET_BLACK.color, 1, cv2.LINE_AA )
        img = cv2.circle( img, (301,301), 300-270, TARGET_BLACK.color, 1, cv2.LINE_AA )
        img = cv2.circle( img, (301,301), 300-285, TARGET_BLACK.color, 1, cv2.LINE_AA )
        img = cv2.line( img, (299,301), (303,301), TARGET_BLACK.color, 1, cv2.LINE_AA )
        img = cv2.line( img, (301,299), (301,303), TARGET_BLACK.color, 1, cv2.LINE_AA )
        cv2.imwrite( '../../../../picts/targets/target-60.png', img )

        # 60 cm spot target
        img = np.zeros( (603,603,3), np.uint8 ) + 255
        img = cv2.circle( img, (301,301), 300-150, TARGET_BLUE.color, cv2.FILLED, cv2.LINE_AA )
        img = cv2.circle( img, (301,301), 300-150, TARGET_BLACK.color, 1, cv2.LINE_AA )
        img = cv2.circle( img, (301,301), 300-180, TARGET_RED.color, cv2.FILLED, cv2.LINE_AA )
        img = cv2.circle( img, (301,301), 300-180, TARGET_BLACK.color, 1, cv2.LINE_AA )
        img = cv2.circle( img, (301,301), 300-210, TARGET_BLACK.color, 1, cv2.LINE_AA )
        img = cv2.circle( img, (301,301), 300-240, TARGET_GOLD.color, cv2.FILLED, cv2.LINE_AA )
        img = cv2.circle( img, (301,301), 300-240, TARGET_BLACK.color, 1, cv2.LINE_AA )
        img = cv2.circle( img, (301,301), 300-270, TARGET_BLACK.color, 1, cv2.LINE_AA )
        img = cv2.circle( img, (301,301), 300-285, TARGET_BLACK.color, 1, cv2.LINE_AA )
        img = cv2.line( img, (299,301), (303,301), TARGET_BLACK.color, 1, cv2.LINE_AA )
        img = cv2.line( img, (301,299), (301,303), TARGET_BLACK.color, 1, cv2.LINE_AA )
        cv2.imwrite( '../../../../picts/targets/target-spot-60.png', img )

        # 40 cm target
        img = np.zeros( (403,403,3), np.uint8 ) + 255
        img = cv2.circle( img, (201,201), 200, TARGET_BLACK.color, 1, cv2.LINE_AA )
        img = cv2.circle( img, (201,201), 200-20, TARGET_BLACK.color, 1, cv2.LINE_AA )
        img = cv2.circle( img, (201,201), 200-40, TARGET_BLACK.color, cv2.FILLED, cv2.LINE_AA )
        img = cv2.circle( img, (201,201), 200-60, TARGET_WHITE.color, 1, cv2.LINE_AA )
        img = cv2.circle( img, (201,201), 200-80, TARGET_BLUE.color, cv2.FILLED, cv2.LINE_AA )
        img = cv2.circle( img, (201,201), 200-100, TARGET_BLACK.color, 1, cv2.LINE_AA )
        img = cv2.circle( img, (201,201), 200-120, TARGET_RED.color, cv2.FILLED, cv2.LINE_AA )
        img = cv2.circle( img, (201,201), 200-120, TARGET_BLACK.color, 1, cv2.LINE_AA )
        img = cv2.circle( img, (201,201), 200-140, TARGET_BLACK.color, 1, cv2.LINE_AA )
        img = cv2.circle( img, (201,201), 200-160, TARGET_GOLD.color, cv2.FILLED, cv2.LINE_AA )
        img = cv2.circle( img, (201,201), 200-160, TARGET_BLACK.color, 1, cv2.LINE_AA )
        img = cv2.circle( img, (201,201), 200-180, TARGET_BLACK.color, 1, cv2.LINE_AA )
        img = cv2.circle( img, (201,201), 200-190, TARGET_BLACK.color, 1, cv2.LINE_AA )
        img = cv2.line( img, (199,201), (203,201), TARGET_BLACK.color, 1, cv2.LINE_AA )
        img = cv2.line( img, (201,199), (201,203), TARGET_BLACK.color, 1, cv2.LINE_AA )
        cv2.imwrite( '../../../../picts/targets/target-40.png', img )

        # 40 cm target B/W
        img = cv2.cvtColor( img, cv2.COLOR_RGB2GRAY )
        cv2.imwrite( '../../../../picts/targets/target-gray-40.png', img )
        
        # 40 cm recurve target
        img = np.zeros( (403,403,3), np.uint8 ) + 255
        img = cv2.circle( img, (201,201), 200, TARGET_BLACK.color, 1, cv2.LINE_AA )
        img = cv2.circle( img, (201,201), 200-20, TARGET_BLACK.color, 1, cv2.LINE_AA )
        img = cv2.circle( img, (201,201), 200-40, TARGET_BLACK.color, cv2.FILLED, cv2.LINE_AA )
        img = cv2.circle( img, (201,201), 200-60, TARGET_WHITE.color, 1, cv2.LINE_AA )
        img = cv2.circle( img, (201,201), 200-80, TARGET_BLUE.color, cv2.FILLED, cv2.LINE_AA )
        img = cv2.circle( img, (201,201), 200-100, TARGET_BLACK.color, 1, cv2.LINE_AA )
        img = cv2.circle( img, (201,201), 200-120, TARGET_RED.color, cv2.FILLED, cv2.LINE_AA )
        img = cv2.circle( img, (201,201), 200-120, TARGET_BLACK.color, 1, cv2.LINE_AA )
        img = cv2.circle( img, (201,201), 200-140, TARGET_BLACK.color, 1, cv2.LINE_AA )
        img = cv2.circle( img, (201,201), 200-160, TARGET_GOLD.color, cv2.FILLED, cv2.LINE_AA )
        img = cv2.circle( img, (201,201), 200-160, TARGET_BLACK.color, 1, cv2.LINE_AA )
        img = cv2.circle( img, (201,201), 200-180, TARGET_BLACK.color, 1, cv2.LINE_AA )
        img = cv2.line( img, (199,201), (203,201), TARGET_BLACK.color, 1, cv2.LINE_AA )
        img = cv2.line( img, (201,199), (201,203), TARGET_BLACK.color, 1, cv2.LINE_AA )
        cv2.imwrite( '../../../../picts/targets/target-recurve-40.png', img )

        # 40 cm compound target
        img = np.zeros( (403,403,3), np.uint8 ) + 255
        img = cv2.circle( img, (201,201), 200, TARGET_BLACK.color, 1, cv2.LINE_AA )
        img = cv2.circle( img, (201,201), 200-20, TARGET_BLACK.color, 1, cv2.LINE_AA )
        img = cv2.circle( img, (201,201), 200-40, TARGET_BLACK.color, cv2.FILLED, cv2.LINE_AA )
        img = cv2.circle( img, (201,201), 200-60, TARGET_WHITE.color, 1, cv2.LINE_AA )
        img = cv2.circle( img, (201,201), 200-80, TARGET_BLUE.color, cv2.FILLED, cv2.LINE_AA )
        img = cv2.circle( img, (201,201), 200-100, TARGET_BLACK.color, 1, cv2.LINE_AA )
        img = cv2.circle( img, (201,201), 200-120, TARGET_RED.color, cv2.FILLED, cv2.LINE_AA )
        img = cv2.circle( img, (201,201), 200-120, TARGET_BLACK.color, 1, cv2.LINE_AA )
        img = cv2.circle( img, (201,201), 200-140, TARGET_BLACK.color, 1, cv2.LINE_AA )
        img = cv2.circle( img, (201,201), 200-160, TARGET_GOLD.color, cv2.FILLED, cv2.LINE_AA )
        img = cv2.circle( img, (201,201), 200-160, TARGET_BLACK.color, 1, cv2.LINE_AA )
        img = cv2.circle( img, (201,201), 200-190, TARGET_BLACK.color, 1, cv2.LINE_AA )
        img = cv2.line( img, (199,201), (203,201), TARGET_BLACK.color, 1, cv2.LINE_AA )
        img = cv2.line( img, (201,199), (201,203), TARGET_BLACK.color, 1, cv2.LINE_AA )
        cv2.imwrite( '../../../../picts/targets/target-compound-40.png', img )

        # 40 cm spot target
        img = np.zeros( (403,403,3), np.uint8 ) + 255
        img = cv2.circle( img, (201,201), 200-100, TARGET_BLUE.color, cv2.FILLED, cv2.LINE_AA )
        img = cv2.circle( img, (201,201), 200-100, TARGET_BLACK.color, 1, cv2.LINE_AA )
        img = cv2.circle( img, (201,201), 200-120, TARGET_RED.color, cv2.FILLED, cv2.LINE_AA )
        img = cv2.circle( img, (201,201), 200-120, TARGET_BLACK.color, 1, cv2.LINE_AA )
        img = cv2.circle( img, (201,201), 200-140, TARGET_BLACK.color, 1, cv2.LINE_AA )
        img = cv2.circle( img, (201,201), 200-160, TARGET_GOLD.color, cv2.FILLED, cv2.LINE_AA )
        img = cv2.circle( img, (201,201), 200-160, TARGET_BLACK.color, 1, cv2.LINE_AA )
        img = cv2.circle( img, (201,201), 200-180, TARGET_BLACK.color, 1, cv2.LINE_AA )
        img = cv2.circle( img, (201,201), 200-190, TARGET_BLACK.color, 1, cv2.LINE_AA )
        img = cv2.line( img, (199,201), (203,201), TARGET_BLACK.color, 1, cv2.LINE_AA )
        img = cv2.line( img, (201,199), (201,203), TARGET_BLACK.color, 1, cv2.LINE_AA )
        cv2.imwrite( '../../../../picts/targets/target-spot-40.png', img )

        # 40 cm recurve spot target
        img = np.zeros( (403,403,3), np.uint8 ) + 255
        img = cv2.circle( img, (201,201), 200-100, TARGET_BLUE.color, cv2.FILLED, cv2.LINE_AA )
        img = cv2.circle( img, (201,201), 200-100, TARGET_BLACK.color, 1, cv2.LINE_AA )
        img = cv2.circle( img, (201,201), 200-120, TARGET_RED.color, cv2.FILLED, cv2.LINE_AA )
        img = cv2.circle( img, (201,201), 200-120, TARGET_BLACK.color, 1, cv2.LINE_AA )
        img = cv2.circle( img, (201,201), 200-140, TARGET_BLACK.color, 1, cv2.LINE_AA )
        img = cv2.circle( img, (201,201), 200-160, TARGET_GOLD.color, cv2.FILLED, cv2.LINE_AA )
        img = cv2.circle( img, (201,201), 200-160, TARGET_BLACK.color, 1, cv2.LINE_AA )
        img = cv2.circle( img, (201,201), 200-180, TARGET_BLACK.color, 1, cv2.LINE_AA )
        img = cv2.line( img, (199,201), (203,201), TARGET_BLACK.color, 1, cv2.LINE_AA )
        img = cv2.line( img, (201,199), (201,203), TARGET_BLACK.color, 1, cv2.LINE_AA )
        cv2.imwrite( '../../../../picts/targets/target-recurve-spot-40.png', img )

        # 40 cm compound spot target
        img = np.zeros( (403,403,3), np.uint8 ) + 255
        img = cv2.circle( img, (201,201), 200-100, TARGET_BLUE.color, cv2.FILLED, cv2.LINE_AA )
        img = cv2.circle( img, (201,201), 200-100, TARGET_BLACK.color, 1, cv2.LINE_AA )
        img = cv2.circle( img, (201,201), 200-120, TARGET_RED.color, cv2.FILLED, cv2.LINE_AA )
        img = cv2.circle( img, (201,201), 200-120, TARGET_BLACK.color, 1, cv2.LINE_AA )
        img = cv2.circle( img, (201,201), 200-140, TARGET_BLACK.color, 1, cv2.LINE_AA )
        img = cv2.circle( img, (201,201), 200-160, TARGET_GOLD.color, cv2.FILLED, cv2.LINE_AA )
        img = cv2.circle( img, (201,201), 200-160, TARGET_BLACK.color, 1, cv2.LINE_AA )
        img = cv2.circle( img, (201,201), 200-190, TARGET_BLACK.color, 1, cv2.LINE_AA )
        img = cv2.line( img, (199,201), (203,201), TARGET_BLACK.color, 1, cv2.LINE_AA )
        img = cv2.line( img, (201,199), (201,203), TARGET_BLACK.color, 1, cv2.LINE_AA )
        cv2.imwrite( '../../../../picts/targets/target-compound-spot-40.png', img )

        # 40 cm NFAA spot target
        img = np.zeros( (403,403,3), np.uint8 ) + 225
        img = cv2.circle( img, (201,201), 200-120, TARGET_BLUE_NFAA.color, cv2.FILLED, cv2.LINE_AA )
        img = cv2.circle( img, (201,201), 200-120, TARGET_WHITE.color, 1, cv2.LINE_AA )
        img = cv2.circle( img, (201,201), 200-140, TARGET_WHITE.color, 1, cv2.LINE_AA )
        img = cv2.circle( img, (201,201), 200-160, TARGET_WHITE.color, cv2.FILLED, cv2.LINE_AA )
        img = cv2.circle( img, (201,201), 200-160, TARGET_BLACK.color, 1, cv2.LINE_AA )
        img = cv2.circle( img, (201,201), 200-180, TARGET_BLACK.color, 1, cv2.LINE_AA )
        img = cv2.line( img, (195,195), (207,207), TARGET_BLUE_NFAA.color, 2, cv2.LINE_AA )
        img = cv2.line( img, (207,195), (195,207), TARGET_BLUE_NFAA.color, 2, cv2.LINE_AA )
        cv2.imwrite( '../../../../picts/targets/target-nfaa-spot-40.png', img )

        # 80 cm field target
        img = np.zeros( (803,803,3), np.uint8 ) + 255
        img = cv2.circle( img, (401,401), 400, TARGET_BLACK.color, cv2.FILLED, cv2.LINE_AA )
        img = cv2.circle( img, (401,401), 400-80, TARGET_WHITE.color, 1, cv2.LINE_AA )
        img = cv2.circle( img, (401,401), 400-160, TARGET_WHITE.color, 1, cv2.LINE_AA )
        img = cv2.circle( img, (401,401), 400-240, TARGET_WHITE.color, 1, cv2.LINE_AA )
        img = cv2.circle( img, (401,401), 400-320, TARGET_GOLD.color, cv2.FILLED, cv2.LINE_AA )
        img = cv2.circle( img, (401,401), 400-360, TARGET_BLACK.color, 1, cv2.LINE_AA )
        img = cv2.line( img, (398,401), (404,401), TARGET_BLACK.color, 1, cv2.LINE_AA )
        img = cv2.line( img, (401,398), (401,404), TARGET_BLACK.color, 1, cv2.LINE_AA )
        cv2.imwrite( '../../../../picts/targets/target-field-80.png', img )

        # 60 cm field target
        img = np.zeros( (603,603,3), np.uint8 ) + 255
        img = cv2.circle( img, (301,301), 300, TARGET_BLACK.color, cv2.FILLED, cv2.LINE_AA )
        img = cv2.circle( img, (301,301), 300-60, TARGET_WHITE.color, 1, cv2.LINE_AA )
        img = cv2.circle( img, (301,301), 300-120, TARGET_WHITE.color, 1, cv2.LINE_AA )
        img = cv2.circle( img, (301,301), 300-180, TARGET_WHITE.color, 1, cv2.LINE_AA )
        img = cv2.circle( img, (301,301), 300-240, TARGET_GOLD.color, cv2.FILLED, cv2.LINE_AA )
        img = cv2.circle( img, (301,301), 300-270, TARGET_BLACK.color, 1, cv2.LINE_AA )
        img = cv2.line( img, (298,301), (303,301), TARGET_BLACK.color, 1, cv2.LINE_AA )
        img = cv2.line( img, (301,298), (301,303), TARGET_BLACK.color, 1, cv2.LINE_AA )
        cv2.imwrite( '../../../../picts/targets/target-field-60.png', img )

        # 40 cm field target
        img = np.zeros( (403,403,3), np.uint8 ) + 255
        img = cv2.circle( img, (201,201), 200, TARGET_BLACK.color, cv2.FILLED, cv2.LINE_AA )
        img = cv2.circle( img, (201,201), 200-40, TARGET_WHITE.color, 1, cv2.LINE_AA )
        img = cv2.circle( img, (201,201), 200-80, TARGET_WHITE.color, 1, cv2.LINE_AA )
        img = cv2.circle( img, (201,201), 200-120, TARGET_WHITE.color, 1, cv2.LINE_AA )
        img = cv2.circle( img, (201,201), 200-160, TARGET_GOLD.color, cv2.FILLED, cv2.LINE_AA )
        img = cv2.circle( img, (201,201), 200-180, TARGET_BLACK.color, 1, cv2.LINE_AA )
        img = cv2.line( img, (198,201), (204,201), TARGET_BLACK.color, 1, cv2.LINE_AA )
        img = cv2.line( img, (201,198), (201,204), TARGET_BLACK.color, 1, cv2.LINE_AA )
        cv2.imwrite( '../../../../picts/targets/target-field-40.png', img )

        # 20 cm field target
        img = np.zeros( (203,203,3), np.uint8 ) + 255
        img = cv2.circle( img, (101,101), 100, TARGET_BLACK.color, cv2.FILLED, cv2.LINE_AA )
        img = cv2.circle( img, (101,101), 100-20, TARGET_WHITE.color, 1, cv2.LINE_AA )
        img = cv2.circle( img, (101,101), 100-40, TARGET_WHITE.color, 1, cv2.LINE_AA )
        img = cv2.circle( img, (101,101), 100-60, TARGET_WHITE.color, 1, cv2.LINE_AA )
        img = cv2.circle( img, (101,101), 100-80, TARGET_GOLD.color, cv2.FILLED, cv2.LINE_AA )
        img = cv2.circle( img, (101,101), 100-90, TARGET_BLACK.color, 1, cv2.LINE_AA )
        img = cv2.line( img, (99,101), (103,101), TARGET_BLACK.color, 1, cv2.LINE_AA )
        img = cv2.line( img, (101,99), (101,103), TARGET_BLACK.color, 1, cv2.LINE_AA )
        cv2.imwrite( '../../../../picts/targets/target-field-20.png', img )

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
def prepare_match_icons() -> None:
    '''Preparation of the icons for matches.
    '''
    print( 'match icons: ', end='' )
    try:
        img = cv2.imread( '../../../../picts/controls/raw/cup-48.png' )
        img[ img < 64 ] = 32
        
        cv2.circle( img, (23,24), 21, WHITE.color, 2, cv2.LINE_AA )
        cv2.line( img, (20,0), (27,0), WHITE.color, 2, cv2.LINE_AA )
        cv2.line( img, (37,2), (44,8), WHITE.color, 2, cv2.LINE_AA )
        
        cup_img = img.copy()
        cup_img[ cup_img > 32 ] //= 3
        cv2.imwrite( '../../../../picts/controls/match-disabled.png', cup_img )
        
        cup_img = img.copy().astype( np.float )
        cup_img[ cup_img > 32 ] //= 1.19
        cv2.imwrite( '../../../../picts/controls/match-off.png', cup_img.round().astype(np.uint8) )
        
        cup_img = img.copy()
        cup_img[:,:,0][ cup_img[:,:,0] > 32 ] = 0
        cv2.imwrite( '../../../../picts/controls/match-on.png', cup_img )
                
        print( ' ok' )

    except Exception as e:
        print( 'failed due to exception', str(e) )

    
#-------------------------------------------------------------------------
def prepare_overlay_icons() -> None:
    '''Preparation of the icons for overlays.
    '''
    print( 'overlay icons: ', end='' )
    try:
        archer_img = 255 - cv2.imread( '../../../../picts/controls/raw/archer.png' )

        archer_img[ archer_img == 254 ] = 32
        img_64_2 = cv2.resize( archer_img, (64,64), interpolation=cv2.INTER_LINEAR ) // 2
        
        offset = 3
        img = np.zeros( (64 + offset, 64 + offset, 3), np.uint8 )
        img[ :64, :64, : ]  = img_64_2[ :, :, : ]
        img[ offset:, offset:, :   ] += img_64_2[ :, :, : ]

        cv2.imwrite( '../../../../picts/controls/raw/overlays_icon.png', img )
        
        _img = img.copy()
        _img[ _img > 32 ] //= 4
        cv2.imwrite( '../../../../picts/controls/overlays-disabled.png', _img )
        
        _img = img.copy().astype( np.float )
        _img[ _img > 32 ] //= 1.19
        cv2.imwrite( '../../../../picts/controls/overlays-off.png', _img.round().astype(np.uint8) )
        
        _img = img.copy()
        _img[:,:,0][ _img[:,:,0] >= 96 ] = 0
        cv2.imwrite( '../../../../picts/controls/overlays-on.png', _img )
                
        print( ' ok' )

    except Exception as e:
        print( 'failed due to exception', str(e) )

    
#-------------------------------------------------------------------------
def prepare_switch_buttons() -> None:
    '''Preparation of the camera buttons.
    '''
    print( 'switch buttons: ', end='' )
    try:
        img = cv2.imread( '../../../../picts/controls/raw/switch-off.png' )
        gray_comp = 96
        for y in range(30,38):
            for x in range(104,134):
                if img[y][x][0] >= gray_comp:
                    img[y][x] //= 2
        img_ = cv2.resize( img, (86,35), interpolation=cv2.INTER_CUBIC )
        cv2.imwrite( f'../../../../picts/controls/switch-off.png', img_ )
        
        img_ = cv2.resize( img, (37,15), interpolation=cv2.INTER_CUBIC )
        cv2.imwrite( f'../../../../picts/controls/switch-off-40.png', img_ )

        img = cv2.imread( '../../../../picts/controls/raw/switch-on.png' )
        gray_comp = 96
        for y in range(22,49):
            for x in range(38,63):
                if img[y][x][0] >= gray_comp:
                    img[y][x] //= 2
        img_ = cv2.resize( img, (86,35), interpolation=cv2.INTER_CUBIC )
        cv2.imwrite( f'../../../../picts/controls/switch-on.png', img_ )

        img_ = cv2.resize( img, (37,15), interpolation=cv2.INTER_CUBIC )
        cv2.imwrite( f'../../../../picts/controls/switch-on-40.png', img_ )

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
        img_ = cv2.resize( img, (86,35), interpolation=cv2.INTER_CUBIC )
        cv2.imwrite( f'../../../../picts/controls/switch-disabled.png', img_ )

        img_ = cv2.resize( img, (37,15), interpolation=cv2.INTER_CUBIC )
        cv2.imwrite( f'../../../../picts/controls/switch-disabled-40.png', img_ )

        print( ' ok' )

    except Exception as e:
        print( 'failed due to exception', str(e) )


#-------------------------------------------------------------------------
def prepare_target_button() -> None:
    '''Preparation of the target button.
    '''
    print( 'target button: ', end='' )
    try:
        img = (np.zeros( (33,33,3)) + AVTConfig.DEFAULT_BACKGROUND.color).astype( np.uint8  )
        img = cv2.circle( img, (16,16), 15, TARGET_WHITE.color, cv2.FILLED, cv2.LINE_AA )
        img = cv2.circle( img, (16,16), 15-3, TARGET_BLACK.color, cv2.FILLED, cv2.LINE_AA )
        img = cv2.circle( img, (16,16), 15-6, TARGET_BLUE.color, cv2.FILLED, cv2.LINE_AA )
        img = cv2.circle( img, (16,16), 15-9, TARGET_RED.color, cv2.FILLED, cv2.LINE_AA )
        img = cv2.circle( img, (16,16), 15-12, TARGET_GOLD.color, cv2.FILLED, cv2.LINE_AA )
        cv2.imwrite( '../../../../picts/controls/target-active.png', img )

        img = cv2.circle( img, (16,16), 15, (TARGET_WHITE // 2.5).color, cv2.FILLED, cv2.LINE_AA )
        img = cv2.circle( img, (16,16), 15-3, (TARGET_BLACK // 2.5).color, cv2.FILLED, cv2.LINE_AA )
        img = cv2.circle( img, (16,16), 15-6, (TARGET_BLUE // 2.5).color, cv2.FILLED, cv2.LINE_AA )
        img = cv2.circle( img, (16,16), 15-9, (TARGET_RED // 2.5).color, cv2.FILLED, cv2.LINE_AA )
        img = cv2.circle( img, (16,16), 15-12, (TARGET_GOLD // 1.5).color, cv2.FILLED, cv2.LINE_AA )
        cv2.imwrite( '../../../../picts/controls/target-inactive.png', img )

        img = cv2.circle( img, (16,16), 15, (TARGET_WHITE // 5).color, cv2.FILLED, cv2.LINE_AA )
        img = cv2.circle( img, (16,16), 15-3, (TARGET_BLACK // 5).color, cv2.FILLED, cv2.LINE_AA )
        img = cv2.circle( img, (16,16), 15-6, (TARGET_BLUE // 5).color, cv2.FILLED, cv2.LINE_AA )
        img = cv2.circle( img, (16,16), 15-9, (TARGET_RED // 5).color, cv2.FILLED, cv2.LINE_AA )
        img = cv2.circle( img, (16,16), 15-12, (TARGET_GOLD // 5).color, cv2.FILLED, cv2.LINE_AA )
        cv2.imwrite( '../../../../picts/controls/target-disabled.png', img )

        print( ' ok' )

    except Exception as e:
        print( 'failed due to exception', str(e) )

    
#-------------------------------------------------------------------------
def prepare_timer_icons() -> None:
    '''Preparation of the icons for matches.
    '''
    print( 'match icons: ', end='' )
    try:
        img = (np.zeros( (48,48,3) ) + 32).astype( np.uint8 )
        
        cv2.circle( img, (23,24), 21, WHITE.color, 2, cv2.LINE_AA )
        cv2.circle( img, (24,25), 2, BLACK.color, cv2.FILLED, cv2.LINE_AA )
        cv2.circle( img, (23,24), 2, WHITE.color, cv2.FILLED, cv2.LINE_AA )
        cv2.line( img, (20,0), (27,0), WHITE.color, 2, cv2.LINE_AA )
        cv2.line( img, (37,2), (44,8), WHITE.color, 2, cv2.LINE_AA )
        cv2.line( img, (24,25), (28,9), BLACK.color, 1, cv2.LINE_AA )
        cv2.line( img, (23,24), (27, 8), WHITE.color, 1, cv2.LINE_AA )
        
        timer_img = img.copy()
        timer_img[ timer_img > 32 ] //= 3
        cv2.imwrite( '../../../../picts/controls/timer-disabled.png', timer_img )
        
        timer_img = img.copy().astype( np.float )
        timer_img[ timer_img > 32 ] //= 1.19
        cv2.imwrite( '../../../../picts/controls/timer-off.png', timer_img.round().astype(np.uint8) )
        
        timer_img = img.copy()
        timer_img[:,:,0][ timer_img[:,:,0] > 32 ] = 0
        cv2.imwrite( '../../../../picts/controls/timer-on.png', timer_img )
                
        print( ' ok' )

    except Exception as e:
        print( 'failed due to exception', str(e) )


#=============================================================================
if __name__ == '__main__':
    """Script description.
    """
    #-------------------------------------------------------------------------
    create_targets()
    prepare_exit_button()
    prepare_match_icons()
    prepare_overlay_icons()
    prepare_switch_buttons()
    prepare_target_button()
    prepare_timer_icons()

#=====   end of   src.GUIItems.Controls._private.prepare_controls_picts   =====#
