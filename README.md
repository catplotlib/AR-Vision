<!-- Author: Puja Chaudhury -->
### Links
https://drive.google.com/file/d/1vFo2pobLJxbGbjaWqy2wvHZ48b390Ek6/view?usp=sharing

The AR-Vision project is a computer vision project that utilizes augmented reality (AR) technology to create interactive experiences.  The main objective of the project is to implement computer vision techniques to detect and track real-world objects and overlay virtual content on top of them in real-time.The project uses various computer vision libraries such as OpenCV and OpenGL to perform image processing, feature detection, camera calibration, and projection mapping. The output is rendered as a 3D virtual object on top of a live camera feed, which can be viewed on a computer screen or a mobile device.

The project includes several components such as a camera calibration module, a feature detection and tracking module, a rendering engine, and an interface to control and manipulate virtual objects. The project also includes several sample applications such as virtual object placement, object recognition, and face tracking. The project can be used as a starting point for developing more complex AR applications, or as a learning resource for computer vision and augmented reality technologies.

After running the main function, the code will look for the chessboard image throught the camera: 
-To show 3D axes, hit the 'x' key.
-To exhibit virtual objects, use the 'd' key.
-To present Harris corners, push the 'r' key.
-To keep the current image frame for calibration, hit the 's' key. If you save more than five frames, continuous calibration will start automatically.
-To save the current calibration in a CSV file, use the 'c' key.
-To capture a screenshot of the current displayed video frame, press the 'k' key.

I have implemented two extensions:
-For the first extension I used a different type of target called the asymmetric circle grid and calibrated my system again in a similar manner as done for the checkerboard target.
-For the second extension, I used the perspective transform method to project two of my favorite artworks ‘The Great Wave off kanagawa’ and ‘Fine Wind, Clear Morning’ paintings onto the target, as can be seen in the background of my room. 

After running the main function, the code will look for the circle-grid image throught the camera: 
- To show the 3D axes, hit the 'x' key.
- Display virtual objects by pressing 'd'.
- To turn the target into an art canvas, press 'p'.
- Save the current image frame for calibration by pressing 's'. If you save more than five frames, continuous calibration will begin automatically.
- Save the current calibration as a CSV file by pressing 'c'.
- Capture a screenshot of the current video frame by pressing 'k'.

