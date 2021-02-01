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

from src.Cameras.cameras_pool    import CamerasPool
from src.Display.main_window     import MainWindow


#=============================================================================
def avt_main():
    """This is the main function of the Archery Video Training application.
    """
    
    #-- creates the main window
    main_window = MainWindow()
     
    #-- shows the main window
    print( "draws main window content" )
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
     
    #===========================================================================
    # #-- let's create all cameras acquisition frameworks
    # with CamerasPool() as cameras_pool:
    #              
    #     #-- let's run now acquisitions, until application is stopped
    #     keep_on = True
    #     frames_count = 0
    #     start_time = time.perf_counter()
    #      
    #     first = True
    #     while keep_on:
    #          
    #         frames = cameras_pool.get_frames()
    #          
    #         ok = False
    #         for index, frame in enumerate(frames):
    #             if frame is not None:
    #                 ok = True
    #                 cv2.imshow( f"camera-{index}", frame.frame )
    #          
    #         if ok:
    #             frames_count += 1
    #             if first:
    #                 start_time = time.perf_counter()
    #                 first = False
    #              
    #         key = cv2.waitKey( 10 )
    #          
    #         keep_on = key != 27
    #      
    #     #-- finally, let's stop threads and release allocated resources
    #     end_time = time.perf_counter()
    #     print( f"frame rate = {(frames_count-1) / (end_time - start_time):.2f} fps" )
    #      
    #     cv2.destroyAllWindows()
    #===========================================================================
         
            
     
    #===========================================================================
    # #-- let's create all cameras acquisition frameworks
    # cameras_pool = CamerasPool()
    #          
    # #-- let's launch all the cameras acquisition frameworks
    # cameras_pool.start_acquisitions()
    #          
    # #-- let's run now on acquisitions until app is stopped
    # keep_on = True
    # frames_count = 0
    # start_time = time.perf_counter()
    #  
    # first = True
    # while keep_on:
    #      
    #     frames = cameras_pool.get_frames()
    #      
    #     ok = False
    #     for index, frame in enumerate(frames):
    #         if frame is not None:
    #             ok = True
    #             cv2.imshow( f"camera-{index}", frame.frame )
    #      
    #     if ok:
    #         frames_count += 1
    #         if first:
    #             start_time = time.perf_counter()
    #             first = False
    #          
    #     key = cv2.waitKey( 10 )
    #      
    #     keep_on = key != 27
    #  
    # #-- finally, let's stop threads and release allocated resources
    # end_time = time.perf_counter()
    #  
    # cv2.destroyAllWindows()
    # cameras_pool.stop()
    #  
    # print( f"frame rate = {(frames_count-1) / (end_time - start_time):.2f} fps" )
    #===========================================================================

    print( "\n-- done!" )
    
#=====   end of   src.App.avt_main   =====#
