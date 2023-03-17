/*
Puja Chaudhury
Modified main function to include the new functions for the extensions.
*/

#include <iostream>

#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include "extend_helper.h"

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

    // Create a cv::Mat object to hold the outputFrame videoFrame
    cv::Mat outputFrame;

    // Initialize variables for different tasks
    bool cornersDrawn = true;
    std::vector<std::vector<cv::Vec3f>> points_list;
    std::vector<std::vector<cv::Point2f>> centers_list;

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

    // Initialize variables for displaying the canvas
    bool canvas = false;

    while (true)
    {
        *cap >> videoFrame; // get a new videoFrame from the camera, treat as a stream
        if (videoFrame.empty())
        {
            printf("EmptyFrameError\n");
            break;
        }

        std::vector<cv::Point2f> centers;
        std::vector<cv::Vec3f> points;

        //  Detect and Extract Circle grid centers
        bool found = extractCircleCenters(videoFrame, outputFrame, centers, cornersDrawn);

        // display axes
        if (showAxes && found)
        {
            specifyCalibration(centers, centers_list, points, points_list); // select calibration images

            // Calculate current position of the camera
            calculateCameraPosition(points, centers, cameraMat, distCoeff, rot, trans);
            std::cout << std::endl
                      << "rotation matrix: " << rot << std::endl;
            std::cout << std::endl
                      << "translation matrix: " << trans << std::endl;

            // project 3D axes
            draw3dAxes(outputFrame, cameraMat, distCoeff, rot, trans);
        }

        // display virtual object
        if (showObject && found)
        {
            specifyCalibration(centers, centers_list, points, points_list);

            // calculate current position of the camera
            calculateCameraPosition(points, centers, cameraMat, distCoeff, rot, trans);
            std::cout << std::endl
                      << "rotation matrix: " << rot << std::endl;
            std::cout << std::endl
                      << "translation matrix: " << trans << std::endl;

            // Create a virtual object
            draw3dObject(outputFrame, cameraMat, distCoeff, rot, trans);
        }

        // Transform target into image canvas
        if (canvas && found)
        {
            specifyCalibration(centers, centers_list, points, points_list);

            // calculate current position of the camera
            calculateCameraPosition(points, centers, cameraMat, distCoeff, rot, trans);
            std::cout << std::endl
                      << "rotation matrix: " << rot << std::endl;
            std::cout << std::endl
                      << "translation matrix: " << trans << std::endl;

            std::string imageFilename = "fuji.jpeg";
            // draw image contents on the target
            drawOnTarget(videoFrame, outputFrame, cameraMat, distCoeff, rot, trans, imageFilename);
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
            // select calibration images
            specifyCalibration(centers, centers_list, points, points_list);

            printf("Saving calibration image...\n");
            std::string fname = "calibration-videoFrame-";
            fname += std::to_string(savedFrameNumber) + ".jpg";
            imwrite(fname, outputFrame);

            // print the corner points in world coordinates with corresponding image coordinates
            std::cout << "---------------------------------------------------------------------------" << std::endl;
            std::cout << "Calibration image " << savedFrameNumber << " saved" << std::endl;
            std::cout << "Point set " << savedFrameNumber << " \t Corners set " << savedFrameNumber << std::endl;
            for (int i = 0; i < points.size(); i++)
            {
                cv::Vec3s point = points[i];
                cv::Point2f corner = centers[i];
                printf("[%d %d %d] \t\t", point[0], point[1], point[2]);
                printf("[%f, %f] \n", corner.x, corner.y);
            }
            std::cout << "---------------------------------------------------------------------------" << std::endl;

            // require at least 5 frames for calibration
            if (savedFrameNumber >= 5)
            {
                // calibrate the camera
                std::cout << "Performing calibration with " << savedFrameNumber << " frames..." << std::endl;
                std::cout << "initial camera matrix:" << std::endl;
                std::cout << cameraMat << std::endl;

                float reprojErr = computeCameraParameters(points_list, centers_list, cameraMat, distCoeff);

                // print the calibration stats for the user
                std::cout << "calibrated camera matrix:" << std::endl;
                std::cout << cameraMat << std::endl;
                std::cout << "re-projection error: " << reprojErr << std::endl;
                std::cout << "distortion coefficients: " << distCoeff << std::endl;
            }

            savedFrameNumber++;
        }
        // press 'c' to save current calibration in a csv file to be read later
        else if (key == 'c' && found && !showAxes && !showObject && cornersDrawn)
        {
            // save current calibration in a csv file
            std::cout << std::endl
                      << "Saving performed calibration..." << std::endl;
            storeCalibrationData(cameraMat, distCoeff);
        }
        // press 'x' to display 3d axes at the origin of world coordinates
        else if (key == 'x' && found)
        {
            showAxes = !showAxes;
            if (showObject)
            {
                showObject = !showObject;
            }
            cornersDrawn = !(showAxes || showObject);
            if (canvas)
            {
                canvas = !canvas;
            }

            // read calibration to display axes
            std::string fileName = "circlegrid_intrinsics.csv";
            loadCalibration(fileName, cameraMat, distCoeff);
            std::cout << std::endl
                      << "retrieved calibrated camera matrix:" << std::endl;
            std::cout << cameraMat << std::endl;
            std::cout << "distortion coefficients: " << distCoeff << std::endl;
        }
        // press 'd' to display 3d objects
        else if (key == 'd' && found)
        {
            if (showAxes)
            {
                showAxes = !showAxes;
            }
            showObject = !showObject;
            cornersDrawn = !(showObject || showAxes);
            if (canvas)
            {
                canvas = !canvas;
            }

            // read calibration to display virtual object
            std::string fileName = "circlegrid_intrinsics.csv";
            loadCalibration(fileName, cameraMat, distCoeff);
            std::cout << std::endl
                      << "retrieved calibrated camera matrix:" << std::endl;
            std::cout << cameraMat << std::endl;
            std::cout << "distortion coefficients: " << distCoeff << std::endl;
        }
        // press 'p' to place image canvas on target
        else if (key == 'p' && found)
        {
            canvas = !canvas;
            if (showAxes)
            {
                showAxes = !showAxes;
            }
            if (showObject)
            {
                showObject = !showObject;
            }
            cornersDrawn = !(showObject || showAxes || canvas);

            // read calibration to transform target
            std::string fileName = "circlegrid_intrinsics.csv";
            loadCalibration(fileName, cameraMat, distCoeff);
            std::cout << std::endl
                      << "retrieved calibrated camera matrix:" << std::endl;
            std::cout << cameraMat << std::endl;
            std::cout << "distortion coefficients: " << distCoeff << std::endl;
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
