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
## This module defines
#
#    class FPSRate
#    class FPSRateSeconds
#    class FPSRateFrames
#

#=============================================================================
import time

from src.Utils.summed_circular_buffer import SummedCircularBuffer


#=============================================================================
class FPSRate:
    """The base class for the evaluation of frames per second rates.
    """
    #-------------------------------------------------------------------------
    def __init__(self, summed_size: int = 10) -> None:
        '''Constructor.
        
        Args:
            summed_size: int
                The size of the summed circular buffer that is 
                associated with this rate - used to filter its
                value over time.
        '''
        self.frames_count = 0
        self.elapsed_time = 0.0
        self.b_first = True
        self.scb = SummedCircularBuffer( summed_size )

    #-------------------------------------------------------------------------
    def get_text(self) -> str:
        '''Returns the currently evaluated frame rate.
        
        Returns:
            Either the lastly evaluated frame rate, express-
            ed as a fractional count of rames per second, or 
            None if no frame rate has yet been evaluated.
        '''
        return f"{self.scb.mean:.1f}"

    #-------------------------------------------------------------------------
    def get_value(self) -> float:
        '''Returns the currently evaluated frame rate (0.0 if not yet evaluated).
        '''
        return self.scb.mean
    
    #-------------------------------------------------------------------------
    def new_frame(self) -> None:
        '''To be called each time a new frame is available.
        
        Internally calls protected and abstract method 
        '_evaluate_fps()' which must be implemented in 
        inheriting classes.
        '''
        self.frames_count += 1
        self._evaluate_fps()

    #-------------------------------------------------------------------------
    def start(self) -> None:
        '''Starts the evaluation process.
        '''
        self.last_time = time.perf_counter()

    #-------------------------------------------------------------------------
    def _evaluate_fps(self) -> None:
        '''Evaluates the fps rate value.
        
        This protected method must be implemented in inheriting
        classes.
        
        Raises:
            NotImplementedError:  this abstract method  is  not
                implemented in inheriting class.
        '''
        raise NotImplementedError()
    

#=============================================================================
class FPSRateSeconds( FPSRate ):
    '''The class of FPS evaluations according to periods expressed in seconds.
    '''
    #-------------------------------------------------------------------------
    def __init__(self, period_s: float = 0.500) -> None:
        '''Constructor.
        
        Args:
            period_s: float
                The period, expressed as a fractional second,
                over which the frame rate will be evaluated /
                refreshed.  Defaults to 0.5 second,  i.e. 500
                milliseconds  (2 Hz).  Should be positive and
                greater than 0.2 s.
        
        Raises:
            ValueError:  frames_count is not set and period_s
                is less than 200 ms
        '''
        super().__init__()
        if period_s <= 0.2:
            raise ValueError( f"argument period_s ({period_s:.3f} s) must be at least 0.200 s")
        self.period_s = period_s

    #-------------------------------------------------------------------------
    def _evaluate_fps(self) -> None:
        '''Evaluates the fps rate value.
        '''
        current_time = time.perf_counter()
        elapsed_time = current_time - self.last_time
        if elapsed_time >= self.period_s:
            if self.b_first:
                self.b_first = False
            else:
                self.scb += self.frames_count / elapsed_time
            self.frames_count = 0
            self.last_time += self.period_s


#=============================================================================
class FPSRateFrames( FPSRate ):
    '''The class of FPS evaluations according to frames count periods.
    '''
    #-------------------------------------------------------------------------
    def __init__(self, frames_period: int = 20) -> None:
        '''Constructor.
        
        Args:
            frames_period: int
                The number of frames after which the frame rate 
                is evaluated / refreshed. Defaults to 20.
        
        Raises:
            ValueError: frames_count is less than 10 or greater
                than 100.
        '''
        if frames_period < 10 or frames_period > 100:
            raise ValueError( f"argument frames_period ({frames_period}) must be in interval [10, 100]")

        super().__init__()
        self.frames_period = frames_period

    #-------------------------------------------------------------------------
    def _evaluate_fps(self) -> None:
        '''Evaluates the fps rate value.
        '''
        if self.frames_count >= self.frames_period:
            current_time = time.perf_counter()
            elapsed_time = current_time - self.last_time
            if self.b_first:
                self.b_first = False
            else:
                self.scb += self.frames_count / elapsed_time
            self.frames_count = 0
            self.last_time = current_time

#=====   end of   src.Display.fps_rate   =====#
