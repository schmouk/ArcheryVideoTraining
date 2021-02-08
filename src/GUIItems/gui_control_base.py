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
from src.Shapes.point    import Point
from src.Display.view    import View


#=============================================================================
class GUIControlBase:
    """The base class for all GUI controls.
    """
    #-------------------------------------------------------------------------
    def __init__(self, parent_view: View,
                       x          : int  = None,
                       y          : int  = None,
                       width      : int  = None,
                       height     : int  = None,
                       visible    : bool = True,
                       enabled    : bool = False,
                       active     : bool = False,
                       *,
                       pos        : Point = None,
                       ) -> None:
        '''Constructor.
            parent_view: View 
                A reference to the containing view.
            x, y : int
                The position of this label in the view.This
                is  the position of the top-left corner  of 
                this control. If argument 'pos' is set, 'x' 
                and 'y' are both ignored. If argument 'pos' 
                is None, 'x' and 'y' must be both set.
            width, height: int
                The width and the height of  this  control.
                May  be  None,  in  which case these values 
                will have to be known at  drawing  time  in 
                inheriting classes.
            visible: bool
                Set this to True to get this control  drawn
                at  its  drawing  time.  Set it to False to
                hide this control at drawing time. Defaults
                to True.
            enabled: bool
                Set this to True to  get  this  control  as 
                being enabled.  Set it to False to get this
                control  as  being  disabled.  May  get  no
                meaning  for  some  controls.  Defaults  to
                False.
            active: bool
                Set this to True to  get  this  control  as
                being active.  Set it to False to get it as
                being inactive. May get no meaning for some
                controls.  May  get  different meanings for
                different  other  controls.   Defaults   to 
                False.
            pos: Point
                If set, takes precedence over x and y. This 
                is  the  position of the top-left corner of 
                this control.  This argument must be  named 
                when passed at construction time. 
        
        Raises:
            AssertionError:  if 'pos' is not set,  both  'x' 
                and 'y' must be and are not both set.
        '''
        self.parent_view = parent_view

        if pos is None:
            assert x is not None and y is not None
            pos = Point( x, y )
        self.pos = pos
        
        self.width  = width
        self.height = height
        
        self.visible = visible
        self.enabled = enabled
        self.active  = active
        
        self.refresh()

    #-------------------------------------------------------------------------
    @property
    def x(self) -> int:
        return self.pos.x
    @x.setter
    def x(self, val: int) -> None:
        self.pos.x = round( val )

    @property
    def y(self) -> int:
        return self.pos.y
    @y.setter
    def y(self, val: int) -> None:
        self.pos.y = round( val )

    #-------------------------------------------------------------------------
    def activate(self) -> None:
        '''Makes this control active.
        
        See counterpart method 'deactivate()'.
        '''
        self.active = True
        self.refresh()

    #-------------------------------------------------------------------------
    def deactivate(self) -> None:
        '''Makes this control active.
        
        See counterpart method 'activate()'.
        '''
        self.active = False
        self.refresh()

    #-------------------------------------------------------------------------
    def disable(self) -> None:
        '''Makes this control disabled.
        
        See counterpart method 'enable()'.
        '''
        self.enable = False
        self.refresh()

    #-------------------------------------------------------------------------
    def draw(self, forced: bool = False) -> None:
        '''Draws this control in its parent view.
        
        This method internally calls protected method '_draw()'
        which must be implemented in inheriting classes.
        
        See related method '_draw()'.
        
        Args:
            forced: bool
                Set this to True to get this control  drawn
                whatever  its refresh status is.  Set it to
                False to get this control drawn only if its
                refresh status is True. Defaults to False.
        '''
        if forced or self.b_refresh:
            self._draw( self.parent_view )
            self.b_refresh = False
            
    #-------------------------------------------------------------------------
    def _draw(self, view: View) -> None:
        '''Draws this control in its parent view - specific part for inheriting classes.
        
        This is the protected part of the drawing method. It
        is  automatically  called  by  method  'draw()'.  It
        relates to the specific part of  drawing  inheriting
        controls.  In  this  base class,  this method is not
        implemented.
        
        This method MUST BE implemented in inheriting classes
        which  should check attribute '.visible' to decide to
        draw or not this control.  Attributes '.enabled'  and
        '.active'  should  be checked also to decide the type
        of drawing for the controls.
        
        Args:
            view: View
                This is a reference to  the  parent  view  of 
                this control. It is provided as a convenience 
                to this protected method.
        
        Raises:
            ImplementationError:  this method is not  implem-
                ented in inheriting class
        '''
        raise NotImplementedError( f"protected method _draw() is not implemented in class {self.__class__.__name__}" )

    #-------------------------------------------------------------------------
    def enable(self) -> None:
        '''Makes this control enabled.
        
        See counterpart method 'disable()'.
        '''
        self.enable = True
        self.refresh()

    #-------------------------------------------------------------------------
    def hide(self) -> None:
        '''Makes this control hidden at drawing time.
        
        See counterpart method 'show()'.
        '''
        self.visible = False
        self.refresh()
        
    #-------------------------------------------------------------------------
    def refresh(self) -> None:
        '''Sets the refresh status of this control.
        
        When status refresh is set, this control will be drawn
        at drawing time.
        '''
        self.b_refresh = True
        
    #-------------------------------------------------------------------------
    def show(self) -> None:
        '''Makes this control shown at drawing time.
        
        See counterpart method '.hide()'.
        '''
        self.visible = True
        self.refresh()

#=====   end of   src.GUIItems.gui_control_base   =====#
