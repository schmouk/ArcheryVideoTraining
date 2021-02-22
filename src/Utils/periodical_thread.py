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
from threading import Event, Thread 
import time


#=============================================================================
class PeriodicalThread( Thread ):
    """The class of periodical threads.
    
    Periodical threads get their processing score called at 
    periodical periods of time.
    """
    #-------------------------------------------------------------------------
    def __init__(self, period_s: float, thread_name: str = None) -> None:
        '''Constructor.
        
        Args:
            period_s: float
                A fractional count of seconds. Defines the 
                period of time that separates two calls to
                the processing method of this thread.
            thread_name: str
                the name of this thread.  May be not  set, 
                in which case the underlying platform will
                define one by default.  Defaults  to  None
                (i.e. not set).
        '''
        super().__init__( name=thread_name )
        self.period_s = period_s
        self.stop_event = Event()

    #-------------------------------------------------------------------------
    def finalize_run_loop(self):
        '''Finalization step after exiting running loop.
        
        This method may be overwritten in inheriting classes
        to  release  any allocated resource after processing 
        has completed.
        In this base class, does nothing.
        
        See method 'initialize_run_loop()'.
        '''
        pass

    #-------------------------------------------------------------------------
    def initialize_run_loop(self):
        '''Initialization step before entering running loop.
        
        This method may be overwritten in inheriting classes to
        initialize / allocate  any  resource  before processing
        takes place.
        In this base class, does nothing.
        
        See method 'finalize_run_loop()'.
        '''
        pass

    #-------------------------------------------------------------------------
    def process(self) -> bool:
        '''The processing core of this periodical thread.
        
        This method MUST BE IMPLEMENTED in inheriting classes.
        
        Returns:
            True if processing is to be kept on,  or False  if
            this thread must be definitively stopped.
        
        Raises:
            NotImplementedError:  method  'process()'  is  not
                implemented in inheriting class.
        '''
        raise NotImplementedError( f"method 'process()' is not implemented in class '{self.__class__.__name__}'" )

    #-------------------------------------------------------------------------
    def run(self) -> None:
        '''The looping method of this thread.
        '''
        self.initialize_run_loop()
        
        self.stop_event.clear()
        
        start_time = time.perf_counter()
        loops_count = 0
        
        wait_s = self.period_s
        
        while not self.stop_event.wait( 0.001 ):
            
            current_time = time.perf_counter()

            # calls the processing core of this periodical thread
            if not self.process():
                break
            
            # evaluates the next time for call
            loops_count += 1
            next_time = loops_count * self.period_s + start_time
            
            # evaluates the waiting period of time
            wait_s = next_time - current_time
            ##wait_s = 0.001 if wait_s < 0.001 else wait_s - 0.002
            time.sleep( 0.001 if wait_s < 0.002 else wait_s - 0.002 )
        
        self.finalize_run_loop()

    #-------------------------------------------------------------------------
    def stop(self) -> None:
        '''Definitively stops this thread.
        '''
        self.stop_event.set()

#=====   end of   src.Utils.periodical_thread   =====#
