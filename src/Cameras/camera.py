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
## This module defines:
#
#    class Camera
#    class NullCamera
#


#=============================================================================
from typing import Any
import cv2
import numpy as np

from src.Utils.types import Frame


#=============================================================================
class Camera:
    """The class of camera instances.
    
    This is based on OpenCV capturing mechanism.
    """
    
    #-------------------------------------------------------------------------
    def __init__(self, cam_id: int,
                       width : int = None,
                       height: int = None  ) -> None:
        '''Constructor.
        
        Args:
            cam_id: int
                The identifier of this camera. Identifiers are 
                integers,  as specified by OpenCV, which start
                with index 0 (embedded web cam on laptops, for
                instance)  and grows up. Providing an index for
                which no hardware video  capturing  device  is
                connected  leads to a faulty camera mode whose
                status  'is_ok()'  returns  False.   Connected
                cameras get their status as being True. 
            width: int
                The wished width for the acquired  frames.  If
                set to None,  the camera default value is used
                instead.  If set,  captures frames are resized
                to this width before being delivered.
            height:int
                The wished height for the acquired frames.  If
                set to None,  the camera default value is used
                instead.  If set,  captures frames are resized
                to this height before being delivered.
        '''
        self.cam_id = cam_id
        self.hndl = cv2.VideoCapture( cam_id )
        self._copy_default_hw_size()
        self.set_frames_size( width, height )
        if self.is_ok():
            self.period = 1.0 / self.get_fps()

    #-------------------------------------------------------------------------
    def __del__(self) -> None:
        '''Definitively stops the video capturing and releases any allocated resources.
        '''
        self.release()

    #-------------------------------------------------------------------------
    def get_fps(self) -> float:
        '''Returns the frame rate of this video capturing device.
        '''
        try:
            return self.hndl.get( cv2.CAP_PROP_FPS )
        except:
            return 0.0 
    
    #-------------------------------------------------------------------------
    def get_hw_height(self) -> int:
        '''Returns the height of rames as set in the H/W device.
        '''
        try:
            return int( self.hndl.get(cv2.CAP_PROP_FRAME_HEIGHT) )
        except:
            return 0

    #-------------------------------------------------------------------------
    def get_hw_width(self) -> int:
        '''Returns the width of rames as set in the H/W device.
        '''
        try:
            return int( self.hndl.get(cv2.CAP_PROP_FRAME_WIDTH) )
        except:
            return 0

    #-------------------------------------------------------------------------
    def get_id(self) -> int:
        '''Returns the identifier of this camera, starting at index 1.
        '''
        return self.cam_id + 1

    #-------------------------------------------------------------------------
    def get_period(self) -> float:
        '''Returns the frames period for this video capturing device.
        '''
        try:
            return 1.0 / self.get_fps()
        except:
            return 0.0

    #-------------------------------------------------------------------------
    def is_ok(self) -> bool:
        '''Returns True when status of this camera is ok, or False otherwise.
        '''
        return self.get_hw_width() != 0

    #-------------------------------------------------------------------------
    def read(self) -> Frame:
        '''Reads next frame.
        
        This is an overwritten version of inherited method, 
        since it only returns a reference to  the  acquired 
        frame or None if no frame has been acquired.
        
        Returns:
            A reference to the captured image,  or None  in 
            case of error.
        '''
        try:
            ok, frame = self.hndl.read()
            self.last_frame = frame
            return frame if ok else None
        except:
            try:
                return self.last_frame
            except:
                return np.zeros( (480,640,3), np.uint8 )
        
    #-------------------------------------------------------------------------
    def release(self) -> None:
        '''Releases all resources that have been allocated with this camera.
        '''
        try:
            self.hndl.release()
        except:
            pass

    #-------------------------------------------------------------------------
    def set_frames_size(self, width: int = None, height: int = None) -> None:
        '''Sets the size of captured frames when delivered.
        
        These values are used to resize delivered frames. See
        method  'read()'  to get an understanding of how they 
        are used.  Both must be either set or None.  If None,
        the  default  H /W  values are used instead - this is
        something like a reset to the H/W values.
        
        Args:
            width: int
                The wished width for the acquired  frames.  If
                set to None,  the camera default value is used
                instead.  If set,  captures frames are resized
                to this width before being delivered.
            height: int
                The wished height for the acquired frames.  If
                set to None,  the camera default value is used
                instead.  If set,  captures frames are resized
                to this height before being delivered.
        
        Raises:
            AssertionError: width  and  height  are  not  both 
                either set or None.
        '''
        if width is None:
            assert height is None, "both arguments must be None."
            self.width  = self.get_hw_width()
            self.height = self.get_hw_height()
        else:
            assert height is not None, "both arguments must be set."
            self.width, self.height = width, height

    #-------------------------------------------------------------------------
    def set_hw_dims(self, width: int = None, height: int = None) -> None:
        '''Sets the physical dimensions of captured frames.
        
        The width and height are fully dependent of the hardware
        of  the camera.  They should be consistent with the true
        resolution  of  the camera.  If both arguments are None,
        the size of the captured frames are set to  the  default
        resolution  of the camera.  If one argument is set,  the
        other must be also, otherwise they must be both None.
        
        Args:
            width: int
                The width of captured frames, expressed in pixel
                units. Defaults to None, in which case the 
                default H/W size is used instead. 
            height: int
                The height  of  captured  frames,  expressed  in 
                pixel units. Defaults to None, in which case the 
                default H/W size is used instead.
        
        Raises:
            AssertionError: dimensions must either be both  None
                or be both set.
            ValueError: one or both sizes are not  greater  than
                zero.
        '''
        if width is None:
            assert height is None
            self.hndl.set( cv2.CAP_PROP_FRAME_WIDTH , self.hw_default_width  )
            self.hndl.set( cv2.CAP_PROP_FRAME_HEIGHT, self.hw_default_height )
        
        else:
            assert height is not None
            if width <= 0 or height <= 0:
                raise ValueError( 'Dimensions must be greater than zero.' )
            self.hndl.set( cv2.CAP_PROP_FRAME_WIDTH , width  )
            self.hndl.set( cv2.CAP_PROP_FRAME_HEIGHT, height )
            self._copy_default_hw_size()  # this call is mandatory because passed arguments may be wrong according to the device H/W
 
    #-------------------------------------------------------------------------
    def __getattr__(self, name: str) -> Any:
        '''Gets the value of the cv2.VideoCapture named attribute.
        
        This is a convenient wrapper to  all  attributes  of
        class  cv2.VideoCapture which is used by composition
        in the definition of cameras.
        
        Args:
            name: str
                The name of a cv2.VideoCapture attribute for
                which the value is ti get back.
        
        Returns:
            The value of the  specified  attribute,  if  the
            specified name does exist. See Raises below.
        
        Raises:
            AttributeError:  the specified attribute name is
                not an attribute of cv2.VideoCapture.
        '''
        try:
            return self.hndl.__getattribute__( name )
        except:
            raise AttributeError( f"'{name}' is not an attribute of OpenCV VideoCapture cameras.")
 
    #-------------------------------------------------------------------------
    def _copy_default_hw_size(self)-> None:
        '''Remembers the currently H/W dimensions of captured frames.
        
        Only used internally.
        This method is to be considered as being private and 
        should not be called outside this class module code.
        '''
        self.hw_default_width  = self.get_hw_width()
        self.hw_default_height = self.get_hw_height()


#=============================================================================
class NullCamera( Camera ):
    '''The class of NULL cameras.
    
    Null cameras are used to set as not enabled the controls
    that are related with not connected cameras.
    '''
    
    #-------------------------------------------------------------------------
    def __init__(self, cam_id: int) -> None:
        '''Constructor.
        
        Args:
            cam_id: int
                The identifier of this camera. Identifiers are 
                integers,  as specified by OpenCV, which start
                with index 0 (embedded web cam on laptops, for
                instance)  and grows up. Providing an index for
                which no hardware video  capturing  device  is
                connected  leads to a faulty camera mode whose
                status  'is_ok()'  returns  False.   Connected
                cameras get their status as being True. 
        '''
        self.cam_id = cam_id   

    #-------------------------------------------------------------------------
    def is_ok(self) -> bool:
        '''Returns True when status of this camera is ok, or False otherwise.
        '''
        return False

#=====   end of   src.Cameras.camera   =====#
