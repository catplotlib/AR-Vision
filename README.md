<!-- Author: Puja Chaudhury -->
### Links
https://drive.google.com/file/d/1vFo2pobLJxbGbjaWqy2wvHZ48b390Ek6/view?usp=sharing

### Specification

OS: MacBook Air (M1, 2020)
IDE: VS Code

### Running Executables (Building from Scratch)

Step 1: Extract the folder from the zip file.
Step 2: Navigate to the root directory in your terminal.
Step 3: Create the CMake files by running the command:

```
cmake ./
```

Step 4: Create the executable files by running the following command.

```
make
```

### Running the program:

To run the program we simply need to run the following:

```
./main
```
After running the main function, the code will look for the chessboard image throught the camera: 
-To show 3D axes, hit the 'x' key.
-To exhibit virtual objects, use the 'd' key.
-To present Harris corners, push the 'r' key.
-To keep the current image frame for calibration, hit the 's' key. If you save more than five frames, continuous calibration will start automatically.
-To save the current calibration in a CSV file, use the 'c' key.
-To capture a screenshot of the current displayed video frame, press the 'k' key.


##### Extensions
I have implemented two extensions:
-For the first extension I used a different type of target called the asymmetric circle grid and calibrated my system again in a similar manner as done for the checkerboard target.
-For the second extension, I used the perspective transform method to project two of my favorite artworks ‘The Great Wave off kanagawa’ and ‘Fine Wind, Clear Morning’ paintings onto the target, as can be seen in the background of my room. 

To run the extensions:
Navigate to the 'Extensions' folder from the root directory. To run the program we simply need to run the following:

```
./main_extend
``` 
After running the main function, the code will look for the circle-grid image throught the camera: 
- To show the 3D axes, hit the 'x' key.
- Display virtual objects by pressing 'd'.
- To turn the target into an art canvas, press 'p'.
- Save the current image frame for calibration by pressing 's'. If you save more than five frames, continuous calibration will begin automatically.
- Save the current calibration as a CSV file by pressing 'c'.
- Capture a screenshot of the current video frame by pressing 'k'.

Time travel days used:
0# AR-Vision
