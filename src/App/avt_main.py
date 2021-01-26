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
    
    cv2.waitKey( 0 )
    
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
    #     
    #===========================================================================
            
    #===========================================================================
    # 
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
    #     
    #===========================================================================
    
    #===========================================================================
    # cam0 = cv2.VideoCapture( 0 )
    # fps = cam0.get( cv2.CAP_PROP_FPS )
    # print( fps,
    #        cam0.get(cv2.CAP_PROP_MODE),
    #        cam0.get(cv2.CAP_PROP_FORMAT),
    #        cam0.get(cv2.CAP_PROP_FOURCC),
    #        cam0.get(cv2.CAP_PROP_FRAME_WIDTH),
    #        cam0.get(cv2.CAP_PROP_FRAME_HEIGHT), sep='\n' )
    # wait_ms = round( 1000 // fps if fps > 0 else 25 )
    # keep_on, frame = cam0.read()
    # n = 0
    # while keep_on:
    #     cv2.imshow( "cam 0", frame )
    #     cv2.waitKey( wait_ms )
    #     keep_on, frame = cam0.read()
    #     n += 1
    #     if keep_on:
    #         keep_on = n < 100
    # cam0.release()
    # cv2.destroyAllWindows()
    #===========================================================================
    
    print( "\n-- done!" )
    
#=====   end of   src.App.avt_main   =====#
