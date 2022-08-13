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
from typing  import Tuple, Type
from types   import TracebackType

from ...context_manager import ContextManager


#=============================================================================
BaseExceptionType = Type[ BaseException ]
ExceptionInfo     = Tuple[ BaseExceptionType, BaseException, TracebackType ]


#=============================================================================
class _SchedulerBase( ContextManager ):
    """The base class for all schedulers.
    """

    #-------------------------------------------------------------------------
    def __init__(self, slice_duration_ms: int) -> None:
        '''Modifies the time slices duration.
        
        Any  call to this method must be associated with a call
        to counterpart method 'restore_slice_duration()'.  This
        can be ensured by the use of Python statement 'with'.
        
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
        super().__init__()
        self.set_slice_duration( slice_duration_ms )

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
        ...
        
    #-------------------------------------------------------------------------
    def set_slice_duration(self, slice_duration_ms: int) -> None:
        '''Modifies the time slices duration.
        
        Any call to this method must be associated with a call
        to counterpart method 'restore_slice_duration()'. This
        can be ensured by the use of Python statement 'with'.
        
        Inheriting classes  must  implement  protected  method
        '_set_slice_duration()'  which is internally called in
        this API method.
        
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
        self._slice_duration = self._platform_clipped( slice_duration_ms )
        self._set_slice_duration( self._slice_duration )

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
        ...

    #-------------------------------------------------------------------------
    def set_thread_priority(self, priority_offset: int) -> None:
        '''Modifies the priority level of the currently active thread.
        
        Must be overwritten in inheriting class, according to the
        underlying OS platform.
        
        In this base class, does nothing.
        
        Args:
            priority_offset: int
                The relative offset applied to the priority level
                of the currently active  thread.  See  predefined
                values at the end of this base class definition.
        '''
        ...

    #-------------------------------------------------------------------------
    def cmp_enter(self) -> None:
        '''Defines the actions to be taken when entering the 'with' statement.
        
        May be overwritten in inheriting classes if the  related
        OS platforms must take specific actions before modifying
        the scheduler time slice duration.
        
        In this base class, does nothing specific.
        '''
        pass

    #-------------------------------------------------------------------------
    def cmp_exit(self, except_type     : BaseExceptionType = None,
                       except_value    : BaseException     = None,
                       except_traceback: TracebackType     = None ) -> None:
        '''Defines the actions to be taken when exiting the 'with' statement.
        
        May be overwritten in inheriting classes if the  related
        OS  platforms  must take specific actions when restoring
        the initial scheduler time slice duration.
        
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
        '''
        self.restore_slice_duration()
    
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
        return 1 if slice_duration_ms <= 1 else slice_duration_ms
    
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
        
        Raises:
            NotImplementedError():  this method is not  implem-
                ented in inheriting class.
        '''
        raise NotImplementedError( "protected method '_set_slice_method()' in not implemented in class", self.__class__.__name__ )
    
    #-------------------------------------------------------------------------
    # Class data
    # priorities offsets
    PRI_MINUS_2 =  -2
    PRI_MINUS_1 =  -1
    PRI_NORMAL  =   0
    PRI_PLUS_1  =   1
    PRI_PLUS_2  =   2
    PRI_IDLE    = -15
    PRI_MAX     =  15

#=====   end of   src.Utils.Scheduling._private._scheduler_base   =====#
