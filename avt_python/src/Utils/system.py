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
import platform
##import sys


#=============================================================================
class System:
    """A wrapper to Python built-in libraries 'sys' and 'platform'.
    """
    
    #-------------------------------------------------------------------------
    @classmethod
    def is_java(cls) -> bool:
        '''Returns True if the underlying OS is Windows.
        '''
        return cls.os == 'Java'
    
    #-------------------------------------------------------------------------
    @classmethod
    def is_linux(cls) -> bool:
        '''Returns True if the underlying OS is Windows.
        '''
        return cls.os[:3] == 'Lin'
    
    #-------------------------------------------------------------------------
    @classmethod
    def is_macos(cls) -> bool:
        '''Returns True if the underlying OS is Windows.
        '''
        return cls.os[:3] == 'Dar'
    
    #-------------------------------------------------------------------------
    @classmethod
    def is_windows(cls) -> bool:
        '''Returns True if the underlying OS is Windows.
        '''
        return cls.os[:3] == 'Win'

    #-------------------------------------------------------------------------
    @staticmethod
    def python_version() -> str:
        '''Returns the current version of the running Python interpreter.
        ''' 
        return platform.python_version()

    #-------------------------------------------------------------------------
    # Class data
    os = platform.system()

#=====   end of   src.Utils.system   =====#
