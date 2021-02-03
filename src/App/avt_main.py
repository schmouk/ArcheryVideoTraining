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
import time

from src.Display.main_window     import MainWindow


#=============================================================================
def avt_main():
    """This is the main function of the Archery Video Training application.
    """
    
    #-- creates the main window
    main_window = MainWindow()
     
    #-- shows the main window
    main_window.draw()
    
    #-- starts the cameras acquisition
    main_window.run_views()
    
    #-- interactions w. mouse and keyboard
    while True:
        if cv2.waitKey( 20 ) == 27:
            break
    
    #-- stops cameras acquisition
    main_window.stop_views()
    
    #===========================================================================
    # with main_window.cameras_pool:
    #     # interactions with mouse & keyboard
    #     while True:
    #         if cv2.waitKey( 20 ) == 27:
    #             break
    #===========================================================================
     
    #-- releases all allocated resources
    cv2.destroyAllWindows()

    print( "\n-- done!" )
    
#=====   end of   src.App.avt_main   =====#
