/*
Puja Chaudhury

This is a CPP function that analyzes a video stream by detecting feature points on a target and placing virtual 3D objects on the target.
*/

#include <iostream>

#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include "calibration.h"
#include "3D_projection.h"

int main(int argc, char *argv[])
{
    cv::VideoCapture *cap;

    // Initialize video capture object
    cap = new cv::VideoCapture(0);
    if (!cap->isOpened())
    {
        printf("Failed to open video device");
        return (-1);
    }

    // Set video capture properties
    cap->set(cv::CAP_PROP_FRAME_WIDTH, 960);
    cap->set(cv::CAP_PROP_FRAME_HEIGHT, 540);
    cv::Size refS((int)cap->get(cv::CAP_PROP_FRAME_WIDTH),
                  (int)cap->get(cv::CAP_PROP_FRAME_HEIGHT));
    printf("Expected size: %d %d\n", refS.width, refS.height);

    // Create a window to display the video
    cv::namedWindow("Video", 1); // identifies a window

    // Create a cv::Mat object to hold the current video videoFrame
    cv::Mat videoFrame;

    // Initialize variables for videoFrame numbers
    int frameNumber = 1;
    int savedFrameNumber = 1;

    // Create a cv::Mat object to hold the output videoFrame
    cv::Mat outputFrame;

    // Initialize variables for different tasks
    bool cornersDrawn = true;
    std::vector<std::vector<cv::Vec3f>> points_list;
    std::vector<std::vector<cv::Point2f>> corners_list;

    // Create an array for the camera matrix
    double camMat[] = {1, 0, (double)videoFrame.cols / 2, 0, 1, (double)videoFrame.rows / 2, 0, 0, 1};

    // Create a cv::Mat object to hold the camera matrix
    cv::Mat cameraMat(cv::Size(3, 3), CV_64FC1, &camMat);

    // Create a cv::Mat object to hold the distortion coefficients
    cv::Mat distCoeff;

    // Create cv::Mat objects to hold the rotation and translation matrices
    cv::Mat rot, trans;

    // Initialize variables for displaying axes and objects
    bool showAxes = false;
    bool showObject = false;

    // Initialize variable for robust feature detection
    bool isRobust = false;

    while (true)
    {
        *cap >> videoFrame; // get a new videoFrame from the camera, treat as a stream
        if (videoFrame.empty())
        {
            printf("EmptyFrameError\n");
            break;
        }

        std::vector<cv::Point2f> corners;
        std::vector<cv::Vec3f> points;

        // Task 1 - Detect and Extract Chessboard Corners
        bool found = GetChessboardCorners(videoFrame, outputFrame, corners, cornersDrawn);
        if (showAxes && found)
        {
            specifyCalibration(corners, corners_list, points, points_list);

            // Task 4 - Calculate Current Position of the Camera
            calculateCameraPosition(points, corners, cameraMat, distCoeff, rot, trans);
            std::cout << std::endl
                      << "rotation matrix: " << rot << std::endl;
            std::cout << std::endl
                      << "translation matrix: " << trans << std::endl;

            // Task 5 - Project Outside Corners or 3D Axes
            draw3dAxes(outputFrame, cameraMat, distCoeff, rot, trans);
        }

        // Display virtual object
        if (showObject && found)
        {
            specifyCalibration(corners, corners_list, points, points_list);

            // Task 4 - Calculate Current Position of the Camera
            calculateCameraPosition(points, corners, cameraMat, distCoeff, rot, trans);
            std::cout << std::endl
                      << "rotation matrix: " << rot << std::endl;
            std::cout << std::endl
                      << "translation matrix: " << trans << std::endl;

            // Task 6 - Create a virtual object
            draw3dObject(outputFrame, cameraMat, distCoeff, rot, trans);
        }

        if (isRobust)
        {
            // Task 7 - detect Robust features
            detectHarrisCorners(videoFrame, outputFrame);
        }

        // display the current videoFrame
        cv::imshow("Video", outputFrame);

        // see if there is a waiting keystroke
        char key = cv::waitKey(10);

        // press 'q' to quit
        if (key == 'q')
        {
            break;
        }
        // press 's' to save current calibration videoFrame and perform calibration if frames >= 5
        else if (key == 's' && found && !showAxes && !showObject && cornersDrawn)
        {
            // Task 2 - Select calibration images
            specifyCalibration(corners, corners_list, points, points_list);

            printf("Calibration image is saved...\n");
            std::string fname = "calibrated-videoFrame-";
            fname += std::to_string(savedFrameNumber) + ".jpg";
            imwrite(fname, outputFrame);

            // print the corner points in world coordinates with corresponding image coordinates
            std::cout << "---------------------------------------------------------------------------" << std::endl;
            std::cout << "Calibration image " << savedFrameNumber << " saved" << std::endl;
            std::cout << "Point set " << savedFrameNumber << " \t Corners set " << savedFrameNumber << std::endl;
            for (int i = 0; i < points.size(); i++)
            {
                cv::Vec3s point = points[i];
                cv::Point2f corner = corners[i];
                printf("[%d %d %d] \t\t", point[0], point[1], point[2]);
                printf("[%f, %f] \n", corner.x, corner.y);
            }
            std::cout << "---------------------------------------------------------------------------" << std::endl;

            //  At least 5 frames are required for calibration
            if (savedFrameNumber >= 5)
            {
                // Task 3 - Calibrate the camera
                std::cout << "Calibrating with " << savedFrameNumber << " frames..." << std::endl;
                std::cout << "Camera matrix before calibration:" << std::endl;
                std::cout << cameraMat << std::endl;

                float reprojErr = computeCameraParameters(points_list, corners_list, cameraMat, distCoeff);

                // print the calibration stats for the user
                std::cout << "Calibrated camera matrix:" << std::endl;
                std::cout << cameraMat << std::endl;
                std::cout << "Re-projection error: " << reprojErr << std::endl;
                std::cout << "Distortion coefficients: " << distCoeff << std::endl;
            }

            savedFrameNumber++;
        }
        //  Press 'c' to save current calibration data in a CSV file for future retrieval.
        else if (key == 'c' && found && !showAxes && !showObject && cornersDrawn)
        {
            // saving current calibration in a csv file
            std::cout << std::endl
                      << "Saving current calibration data." << std::endl;
            storeCalibrationData(cameraMat, distCoeff);
        }
        // Press 'x' to toggle display of 3D axes at the origin of world coordinates
        else if (key == 'x' && found)
        {
            // Toggle the showAxes flag and set showObject flag to false
            showAxes = !showAxes;
            if (showObject)
            {
                showObject = !showObject;
            }

            // Toggle the cornersDrawn flag based on showAxes and showObject flags
            cornersDrawn = !(showAxes || showObject);

            // Read calibration data from "intrinsic_data_chessboard.csv" file and print the camera matrix and distortion coefficients
            std::string fileName = "chessboard_intrinsics.csv";
            loadCalibration(fileName, cameraMat, distCoeff);
            std::cout << std::endl
                      << "retrieved calibrated camera matrix:" << std::endl;
            std::cout << cameraMat << std::endl;
            std::cout << "distortion coefficients: " << distCoeff << std::endl;
        }
        // Press 'd' to visualize 3D objects.
        else if (key == 'd' && found)
        {
            if (showAxes)
            {
                showAxes = !showAxes;
            }
            showObject = !showObject;
            cornersDrawn = !(showObject || showAxes);

            // Load the calibration data to display the virtual object.
            std::string fileName = "chessboard_intrinsics.csv";
            loadCalibration(fileName, cameraMat, distCoeff);
            std::cout << std::endl
                      << "Calibrated camera matrix is retrieved:" << std::endl;
            std::cout << cameraMat << std::endl;
            std::cout << "The distortion coefficients are: " << distCoeff << std::endl;
        }

        // Press the 'r' key for Robust features
        else if (key == 'r')
        {
            cornersDrawn = !cornersDrawn;
            showAxes = false;
            showObject = false;
            isRobust = !isRobust;
        }
        // Press the 'k' key to capture a snapshot of the current video videoFrame.
        else if (key == 'k')
        {
            printf("Saving the Images\n");
            std::string fname = "videoFrame-";
            fname += std::to_string(frameNumber) + ".jpg";
            imwrite(fname, outputFrame);
            frameNumber++;
        }
    }

    delete cap;

    return (0);
}
