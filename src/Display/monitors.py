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
import sys
from PyQt5.QtWidgets     import QApplication


#=============================================================================
class Monitors:
    """The class of monitors features.
    
    This is a singleton. It will be instantiated only once.
    Once instantiated, it provides access to methods:
        - get_dpi()
        - get_dpcm()
    """
    #-------------------------------------------------------------------------
    def __init__(self) -> None:
        '''Constructor.
        '''
        if self._ME is None:
            app = QApplication(sys.argv)
            self.monitors = app.screens()
            self.monitors_count = len( self.monitors )
            self.dpis = [ scr.physicalDotsPerInch() for scr in self.monitors ]
            self.all_same_dpis = (self.monitors_count <= 1  or
                                  all( [self.dpis[i] == self.dpis[0] for i in range(1, self.monitors_count) ]) )
            app.quit()
            self._ME = self
        
    #-------------------------------------------------------------------------
    def get_dpcm(self, monitor_index: int = None) -> float:
        '''Returns the dots-per-cm for the specified monitor.
        
        Args:
            monitor_index: int
                The index of the monitor for which  the  number 
                of  dots per cm is wanted.  If None,  checks if
                all  monitors  get  the  same  dpcm  value  and
                returns it if they all are the same.
        
        Returns:
            The number (float) of dots per cm for the specified
            indexed monitor (index starts at 0).
        
        Raises:
            KeyError: the index passed as  argument  is  not  a
                valid  index,  either because it is negative or
                because it is out of bounds (greater  or  equal
                to 'self.monitors_count').
            ValueError: argument 'monitor' is None,  there  are
                many monitors connected to the PC and dpcms are
                not the same for every monitors.
        '''
        return self.get_dpi( monitor_index ) / 2.54

    #-------------------------------------------------------------------------
    def get_dpi(self, monitor_index: int = None) -> float:
        '''Returns the dots-per-inch for the specified monitor.
        
        Args:
            monitor: int
                The index of the monitor for which  the  number 
                of  dots per cm is wanted.  If None,  checks if
                all  monitors  get  the  same  dpcm  value  and
                returns it if they all are the same.
        
        Returns:
            The number (float) of dots per cm for the specified
            indexed monitor (index starts at 0).
        
        Raises:
            KeyError: the index passed as  argument  is  not  a
                valid  index,  either because it is negative or
                because it is out of bounds (greater  or  equal
                to 'self.monitors_count').
            ValueError: argument 'monitor' is None,  there  are
                many monitors connected to the PC and dpcms are
                not the same for every monitors.
        '''
        if monitor_index is None:
            if self.all_same_dpis:
                return self.dpis[ 0 ]
            else:
                raise ValueError( 'Connected displays are not of the same dpi value while no display index has been provided to know its specific valu' )
        else:
            return self.dpis[ monitor_index ]
        
    #-------------------------------------------------------------------------
    def __len__(self) -> int:
        return self.monitors_count
        
    #-------------------------------------------------------------------------
    _ME = None

#=====   end of   src.Display.monitors   =====#
