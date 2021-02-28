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
import ctypes
kernel32 = ctypes.WinDLL( 'kernel32' )
winmm    = ctypes.WinDLL( 'winmm' )

from typing import Tuple

from .._scheduler_base import _SchedulerBase


#=============================================================================
class Win_TIMECAPS( ctypes.Structure ):
    _fields_ = [
        ('wPeriodMin', ctypes.c_uint),
        ('wPeriodMax', ctypes.c_uint)
    ]


#=============================================================================
class Scheduler( _SchedulerBase):
    """The class of the Windows scheduler.
    """

    #-------------------------------------------------------------------------
    def restore_slice_duration(self) -> None:
        '''Restores the previous time slice duration.
        
        This method must be called at the end of any use of the 
        modified  scheduler  time  slice  duration.  It  is the
        counterpart of method 'set_slice_duration()'.  The best
        way  to  ensure  proper  use  of both methods is to use
        schedulers with Python statement 'with'.
        
        In this base class, this method does nothing.  Inherit-
        ing classes SHOULD overwrite this method.  They can get
        access to protected  attribute  '._slice_duration'  set
        on call to method 'set_slice_duration()'.
        '''
        winmm.timeEndPeriod( self._slice_duration )

    #-------------------------------------------------------------------------
    def set_thread_background(self, bg: bool = True) -> None:
        '''Sets the background status of the currently active thread.
        
        Should be overwritten in inheriting classes, accord-
        ing to the underlying OS platform.
        
        In this base class, does nothing.
        
        Args:
            bg: bool
                Set this to Tue to put the currently  active
                thread  running  in background.  Set this to
                False to restore the currently active thread
                initial  running  status.  Defaults  to True
                (i.e. put thread in background).
        '''
        res = kernel32.SetThreadPriority( kernel32.GetCurrentThread(),
                                          0x00010000 if bg else 0x00020000 )
        if res == 0:
            print( "!!! Windows system error - thread background status is not modified." )
        
    #-------------------------------------------------------------------------
    def set_thread_priority(self, priority_offset: int) -> None:
        '''Modifies the priority level of the currently active thread.
        
        Must be overwritten in inheriting class, according to the
        underlying OS platform.
        
        Args:
            priority_offset: int
                The relative offset applied to the priority level
                of the currently active  thread.  See  predefined
                values at the end of this base class definition.
        '''
        if priority_offset not in (self.PRI_IDLE   , self.PRI_MAX    ,
                                   self.PRI_MINUS_1, self.PRI_MINUS_2, 
                                   self.PRI_NORMAL ,
                                   self.PRI_PLUS_1 , self.PRI_PLUS_2  ):
            print( f"warning - priority offset {priority_offset} may not be a valid value for Windows" )
        
        res = kernel32.SetThreadPriority( kernel32.GetCurrentThread(), priority_offset )
        if res == 0:
            print( "!!! Windows system error - thread priority is not modified." )
    
    #-------------------------------------------------------------------------
    def _platform_clipped(self, slice_duration_ms ) -> int:
        '''Returns a clipped value for the passed argument.
        
        This method  should  be  overwritten  in  inheriting
        classes, as long as the minb and max values for time
        slices  can  be  asked  for  to  the  underlying  OS 
        platform.
        
        In this base class, returns the passed value without
        any clipping but a minimal time slice of 1 ms.
        
        Returns:
            A time slice duration, expressed in milliseconds,
            that  is  clipped according to min and max values
            for time slices according to  the  underlying  OS
            platform.
        '''
        win_tc = Win_TIMECAPS()
        ret_value = winmm.timeGetDevCaps( ctypes.byref(win_tc) )
        
        if ret_value == 0:
            return (win_tc.wPeriodMin if slice_duration_ms <= win_tc.wPeriodMin
                                      else win_tc.wPeriodMax if slice_duration_ms >= win_tc.wPeriodMax
                                                             else slice_duration_ms)
    
    #-------------------------------------------------------------------------
    def _set_slice_duration(self, slice_duration_ms: int) -> None:
        '''Modifies the time slices duration.
        
        This is the protected part of the implementation  that
        relates  to the underlying OS platform.  It embeds all
        the code that is dedicated to the OS platform.
        
        It must be implemented in inheriting classes.
        
        Args:
            slice_duration_ms: int
                The new duration of time slices to be  set  for
                the OS platform scheduler, expressed as integer 
                milliseconds.  When they are accessible the  OS 
                platform  min  and  max values are used to clip 
                the passed duration  value.  If  they  are  not 
                accessible with the underlying OS platform,  it 
                is the responsibility of the caller  to  ensure 
                proper value for the passed argument.
        '''
        winmm.timeBeginPeriod( slice_duration_ms )
    
#=====   end of   src.Utils.Scheduling._private._win.__init__   =====#
