# AR-Vision Project
AR-Vision is a computer vision project that combines augmented reality (AR) with computer vision techniques to create interactive experiences. It detects and tracks real-world objects, overlaying virtual content on them in real-time. The project uses OpenCV and OpenGL libraries for image processing, feature detection, camera calibration, and projection mapping.

### Links
https://drive.google.com/file/d/1vFo2pobLJxbGbjaWqy2wvHZ48b390Ek6/view?usp=sharing

## Features
- Camera calibration module
- Feature detection and tracking module
- Rendering engine
- Interface to control and manipulate virtual objects
- Sample applications: virtual object placement, object recognition, and face tracking

## Getting Started
To run the project, execute the main function. The code will search for either the chessboard or circle-grid image through the camera, depending on the selected mode.

### Chessboard Mode Controls
x: Show 3D axes
d: Display virtual objects
r: Show Harris corners
s: Save the current image frame for calibration (if more than five frames are saved, continuous calibration starts automatically)
c: Save the current calibration as a CSV file
k: Capture a screenshot of the current video frame

### Circle-Grid Mode Controls
x: Show 3D axes
d: Display virtual objects
p: Turn the target into an art canvas
s: Save the current image frame for calibration (if more than five frames are saved, continuous calibration begins automatically)
c: Save the current calibration as a CSV file
k: Capture a screenshot of the current video frame

## Contributing
Contributions are welcome! Please feel free to submit a pull request or open an issue to discuss improvements, bug fixes, or new features.
