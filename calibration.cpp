/*
Puja Chaudhury
Calibrate.cpp
Function implementations for various steps used during the calibration of camera and AR system.
*/

#include "calibration.h"
#include "helper_csv.h"

/*
This function takes three parameters:
a cv::Mat object representing the image frame,
another cv::Mat object for the output,
and a vector of points.
The function first identifies the corners present in the checkerboard grid in the input image, then draws them on the output image.
Additionally, the function populates the input vector with the pixel coordinates of the detected corners in the image.
 */
bool GetChessboardCorners(cv::Mat &inputImage, cv::Mat &outputImage, std::vector<cv::Point2f> &corners, bool shouldDrawCorners)
{
    outputImage = inputImage.clone();
    bool found = cv::findChessboardCorners(inputImage, cv::Size(9, 6), corners);
    cv::Mat gray;
    cv::cvtColor(inputImage, gray, cv::COLOR_BGR2GRAY);
    if (found == true)
    {
        cv::cornerSubPix(gray, corners, cv::Size(5, 5), cv::Size(-1, -1), cv::TermCriteria(cv::TermCriteria::COUNT | cv::TermCriteria::EPS, 30, 0.1));
    }
    if (shouldDrawCorners)
    {
        cv::drawChessboardCorners(outputImage, cv::Size(6, 9), corners, found);
    }
    return (found);
}

/*
This function takes a vector of points that represent the pixel coordinates of detected corners in an image.
It then calculates the corresponding world coordinates for these points on a checkerboard target,
and populates a vector of points with these world coordinates.
Additionally, the function populates vectors for storing multiple sets of pixel and world coordinates,
which can be used for calibration purposes.
 */
int specifyCalibration(std::vector<cv::Point2f> &corners, std::vector<std::vector<cv::Point2f>> &corners_list, std::vector<cv::Vec3f> &points, std::vector<std::vector<cv::Vec3f>> &points_list)

{
    int cols = 9;
    for (int k = 0; k < corners.size(); k++)
    {
        float i = (float)(k % cols);
        float j = (float)(-1 * k / cols);
        cv::Vec3f point(i, j, 0);
        points.push_back(point);
    }
    corners_list.push_back(corners);
    points_list.push_back(points);
    return (0);
}

/*
This function takes as input a vector of point sets and a vector of corner sets,
as well as an initial camera matrix.
Using this data, the function generates a calibration and computes the calibrated camera matrix and distortion coefficients.
 */
float computeCameraParameters(std::vector<std::vector<cv::Vec3f>> &points_list, std::vector<std::vector<cv::Point2f>> &corners_list, cv::Mat &camera_matrix, cv::Mat &dist_coeff)
{
    std::vector<cv::Mat> rot, trans;

    float error = cv::calibrateCamera(points_list,
                                      corners_list,
                                      cv::Size(1280, 720),
                                      camera_matrix,
                                      dist_coeff,
                                      rot,
                                      trans,
                                      cv::CALIB_FIX_ASPECT_RATIO,
                                      cv::TermCriteria(cv::TermCriteria::MAX_ITER + cv::TermCriteria::EPS, 30, DBL_EPSILON));

    return (error);
}

/*
This function saves the current calibration data,
 including the camera matrix and distance coefficients,
 to a CSV file. This file can be later retrieved to calculate the camera pose.
 */
int storeCalibrationData(cv::Mat &camera_matrix, cv::Mat &dist_coeff)
{
    std::string fileName = "intrinsics.csv";
    char *fileName_char = new char[fileName.length() + 1];
    strcpy(fileName_char, fileName.c_str());

    std::string columnName = "camera_matrix";
    char *labelName_char = new char[columnName.length() + 1];
    strcpy(labelName_char, columnName.c_str());

    std::vector<float> camVector;
    for (int i = 0; i < camera_matrix.rows; i++)
    {
        for (int j = 0; j < camera_matrix.cols; j++)
        {
            float f_val = (float)camera_matrix.at<double>(i, j);
            camVector.push_back(f_val);
        }
    }
    append_image_data_csv(fileName_char, labelName_char, camVector);

    columnName = "distortion_coeff";
    char *label_char = new char[columnName.length() + 1];
    strcpy(label_char, columnName.c_str());

    std::vector<float> distVector;
    for (int i = 0; i < dist_coeff.rows; i++)
    {
        for (int j = 0; j < dist_coeff.cols; j++)
        {
            float f_val = (float)dist_coeff.at<double>(i, j);
            distVector.push_back(f_val);
        }
    }
    append_image_data_csv(fileName_char, label_char, distVector);

    return (0);
}
