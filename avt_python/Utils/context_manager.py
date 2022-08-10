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
from types   import TracebackType
from typing  import ForwardRef, Tuple, Type


#=============================================================================
BaseExceptionType = Type[ BaseException ]
ExceptionInfo     = Tuple[ BaseExceptionType, BaseException, TracebackType ]

ContextManagerRef = ForwardRef( "ContextManager" )


#=============================================================================
class ContextManager:
    """The base class for the implementation of Context Managers.
    
    Minimally implements methods __enter__() and __exit__() 
    which define the Context Manager Protocol.
    
    Inheriting classes may overwrite internally called methods:
        - cmp_enter()
        - cmp_exit()
        - cmp_trace().
    """
    
    #-------------------------------------------------------------------------
    def __init__(self, intercept_except: bool = False) -> None:
        '''Constructor.
        
        Args:
            intercept_except: bool
                When this is set to True,  any raised exception
                gets caught within the 'with' block. This leads
                to the printing of an error message on console.
                When  this  is  set to False,  any exception is
                raised and its catching is under the responsab-
                ility  of  code  external  to the 'with' block.
                Defaults to False.
        '''
        self.intercept = intercept_except

    #-------------------------------------------------------------------------
    def cmp_enter(self) -> None:
        '''Defines the actions to be taken when entering the 'with' statement.
        
        May be overwritten by inheriting classes for their specific needs.
        '''
        pass

    #-------------------------------------------------------------------------
    def cmp_exit(self, except_type     : BaseExceptionType = None,
                       except_value    : BaseException     = None,
                       except_traceback: TracebackType     = None ) -> None:
        '''Defines the actions to be taken when exiting the 'with' statement.
        
        May be overwritten by inheriting classes for their specific needs.
        
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
        pass

    #-------------------------------------------------------------------------
    def cmp_trace(self, except_type     : BaseExceptionType = None,
                        except_value    : BaseException     = None,
                        except_traceback: TracebackType     = None ) -> None:
        '''This is the printing method associated with internally caught exceptions.
        
        May be overwritten by inheriting classes for their specific needs.
        
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
        try:
            if except_type is not None:
                if except_value is not None:
                    print( f"!!! {except_value}" )
                else:
                    print( "!!! Unknown exception raised" )
                if except_traceback is not None:
                    print( f"{except_traceback}" )
                else:
                    print( "    No traceback to print" )
        except:
            pass

    #-------------------------------------------------------------------------
    def __enter__(self) -> ContextManagerRef:
        '''Implementation of the Context Manager Protocol - the Entry point.
        
        Returns:
            A reference to this context manager.
        '''
        self.cmp_enter()
        return self

    #-------------------------------------------------------------------------
    def __exit__(self, except_type     : BaseExceptionType = None,
                       except_value    : BaseException     = None,
                       except_traceback: TracebackType     = None ) -> bool:
        '''Implementation of the Context Manager Protocol - the Exit point.
        
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
        
        Returns:
            True if any raised exception  is  considered  to  be
            caught  by  this  context  manager,  or False if any
            raised exception must be raised again.
        '''
        if self.intercept:
            self.cmp_trace( except_type, except_value, except_traceback )
        self.cmp_exit( except_type, except_value, except_traceback )
        
        return self.intercept
        
#=====   end of   src.Utils.context_manager   =====#
