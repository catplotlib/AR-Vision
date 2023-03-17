/*
Puja Chaudhury
CPP functions for mapping 3D points in world coordinates to corresponding 2D pixel coordinates in an image.
*/

#include "3D_projection.h"
#include "helper_csv.h"

/*
This function extracts the calibrated camera matrix and distortion coefficients from a CSV file containing calibration data.
 */
int loadCalibration(std::string csv_filename, cv::Mat &camera_matrix, cv::Mat &dist_coeff)
{
    char *fname_char = new char[csv_filename.length() + 1];
    strcpy(fname_char, csv_filename.c_str());

    std::cout << "Loading the saved calibration" << std::endl;
    std::vector<char *> featureName;
    std::vector<std::vector<float>> data;
    read_image_data_csv(fname_char, featureName, data);

    camera_matrix.at<double>(0, 0) = (double)data[0][0];
    camera_matrix.at<double>(0, 1) = (double)data[0][1];
    camera_matrix.at<double>(0, 2) = (double)data[0][2];
    camera_matrix.at<double>(1, 0) = (double)data[0][3];
    camera_matrix.at<double>(1, 1) = (double)data[0][4];
    camera_matrix.at<double>(1, 2) = (double)data[0][5];
    camera_matrix.at<double>(2, 0) = (double)data[0][6];
    camera_matrix.at<double>(2, 1) = (double)data[0][7];
    camera_matrix.at<double>(2, 2) = (double)data[0][8];

    dist_coeff = cv::Mat(1, 5, CV_32F);

    for (int i = 0; i < 5; i++)
    {
        dist_coeff.at<float>(0, i) = data[1][i];
    }

    return (0);
}

/*
This function estimates the camera's position relative to the target based on the given vector containing the current point set and corner set,
calibrated camera matrix, and distortion coefficients. The resulting rotation and translation data are stored in the specified arrays.
 */
int calculateCameraPosition(std::vector<cv::Vec3f> &points, std::vector<cv::Point2f> &corners, cv::Mat &camera_matrix, cv::Mat &dist_coeff, cv::Mat &rot, cv::Mat &trans)
{
    cv::solvePnP(points, corners, camera_matrix, dist_coeff, rot, trans);

    return (0);
}

/*
This function takes in the following parameters: an image frame in the form of a cv::Mat,
a calibrated camera matrix, distortion coefficients, and rotation and translation data representing the current estimated camera position.

The function then calculates the 3D world coordinates of the axes and projects them to image pixel coordinates on the input image frame. Finally,
it draws lines between these points to create the 3D axes at the origin.
 */
int draw3dAxes(cv::Mat &src, cv::Mat &camera_matrix, cv::Mat &dist_coeff, cv::Mat &rot, cv::Mat &trans)
{
    std::vector<cv::Vec3f> points;
    points.push_back(cv::Vec3f({0, 0, 0}));
    points.push_back(cv::Vec3f({2, 0, 0}));
    points.push_back(cv::Vec3f({0, -2, 0}));
    points.push_back(cv::Vec3f({0, 0, 2}));

    std::vector<cv::Point2f> corners;

    cv::projectPoints(points, rot, trans, camera_matrix, dist_coeff, corners);

    cv::arrowedLine(src, corners[0], corners[1], cv::Scalar(0, 0, 255), 5);
    cv::arrowedLine(src, corners[0], corners[2], cv::Scalar(0, 255, 0), 5);
    cv::arrowedLine(src, corners[0], corners[3], cv::Scalar(255, 0, 0), 5);

    return (0);
}

/*
This function takes in the following parameters:
cv::Mat for the image frame,
the calibrated camera matrix,
the distortion coefficients,
rotation and translation data for the current estimated camera position.

The function projects 3D world vertices of virtual shapes to image pixel coordinates on the image frame using the camera matrix and distortion coefficients.
It then draws lines between these points to generate 3D virtual objects on the target.
*/
int draw3dObject(cv::Mat &src, cv::Mat &camera_matrix, cv::Mat &dist_coeff, cv::Mat &rot, cv::Mat &trans)
{
    std::vector<cv::Vec3f> points;
    std::vector<cv::Point2f> corners;

    // Draw a triangle
    points.push_back(cv::Vec3f({2, -2, 3})); // center
    points.push_back(cv::Vec3f({3, -1, 0})); // tr
    points.push_back(cv::Vec3f({3, -3, 0})); // br
    points.push_back(cv::Vec3f({1, -3, 0})); // bl
    points.push_back(cv::Vec3f({1, -1, 0})); // tl
    cv::projectPoints(points, rot, trans, camera_matrix, dist_coeff, corners);
    cv::line(src, corners[0], corners[1], cv::Scalar(0, 255, 255), 3);
    cv::line(src, corners[0], corners[2], cv::Scalar(0, 255, 255), 3);
    cv::line(src, corners[0], corners[3], cv::Scalar(0, 255, 255), 3);
    cv::line(src, corners[0], corners[4], cv::Scalar(0, 255, 255), 3);
    cv::line(src, corners[1], corners[2], cv::Scalar(0, 255, 255), 3);
    cv::line(src, corners[2], corners[3], cv::Scalar(0, 255, 255), 3);
    cv::line(src, corners[3], corners[4], cv::Scalar(0, 255, 255), 3);
    cv::line(src, corners[4], corners[1], cv::Scalar(0, 255, 255), 3);

    points.clear();
    corners.clear();

    // Draw a cylinder on the chessboard
    float height = 4;
    float radius = 1;
    cv::Point3f center(5, -5, height / 2);
    int num_segments = 20;
    for (int i = 0; i < num_segments; i++)
    {
        float theta1 = ((float)i / (float)num_segments) * 2 * CV_PI;
        float theta2 = ((float)(i + 1) / (float)num_segments) * 2 * CV_PI;
        std::vector<cv::Vec3f> circlePoints1, circlePoints2;
        for (int j = 0; j <= num_segments; j++)
        {
            float x = radius * cos((float)j / (float)num_segments * 2 * CV_PI);
            float y = radius * sin((float)j / (float)num_segments * 2 * CV_PI);
            circlePoints1.push_back(cv::Vec3f(x + center.x, y + center.y, center.z - height / 2));
            circlePoints2.push_back(cv::Vec3f(x + center.x, y + center.y, center.z + height / 2));
        }
        std::vector<cv::Point2f> corners1, corners2;
        cv::projectPoints(circlePoints1, rot, trans, camera_matrix, dist_coeff, corners1);
        cv::projectPoints(circlePoints2, rot, trans, camera_matrix, dist_coeff, corners2);
        for (size_t j = 0; j < corners1.size() - 1; ++j)
        {
            cv::line(src, corners1[j], corners1[j + 1], cv::Scalar(255, 0, 0), 3);
            cv::line(src, corners2[j], corners2[j + 1], cv::Scalar(255, 0, 0), 3);
            cv::line(src, corners1[j], corners2[j], cv::Scalar(255, 0, 0), 3);
        }
    }

    points.clear();
    corners.clear();

    // Draw a sphere on the chessboard
    float radCircle = 1.5;
    cv::Point3f centerCircle(5, 0, radCircle);
    for (int i = 0; i < num_segments; i++)
    {
        float theta1 = ((float)i / (float)num_segments) * CV_PI;
        float theta2 = ((float)(i + 1) / (float)num_segments) * CV_PI;
        std::vector<cv::Vec3f> circlePoints1, circlePoints2;
        for (int j = 0; j <= num_segments; j++)
        {
            float phi = ((float)j / (float)num_segments) * 2 * CV_PI;
            float x = radCircle * sin(theta1) * cos(phi) + centerCircle.x;
            float y = radCircle * sin(theta1) * sin(phi) + centerCircle.y;
            float z = radCircle * cos(theta1) + centerCircle.z;
            circlePoints1.push_back(cv::Vec3f(x, y, z));

            x = radCircle * sin(theta2) * cos(phi) + centerCircle.x;
            y = radCircle * sin(theta2) * sin(phi) + centerCircle.y;
            z = radCircle * cos(theta2) + centerCircle.z;
            circlePoints2.push_back(cv::Vec3f(x, y, z));
        }
        std::vector<cv::Point2f> corners1, corners2;
        cv::projectPoints(circlePoints1, rot, trans, camera_matrix, dist_coeff, corners1);
        cv::projectPoints(circlePoints2, rot, trans, camera_matrix, dist_coeff, corners2);
        for (size_t j = 0; j < corners1.size() - 1; ++j)
        {
            cv::line(src, corners1[j], corners1[j + 1], cv::Scalar(0, 0, 255), 3);
            cv::line(src, corners2[j], corners2[j + 1], cv::Scalar(0, 0, 255), 3);
            cv::line(src, corners1[j], corners2[j], cv::Scalar(0, 0, 255), 3);
        }
    }
    points.clear();
    corners.clear();
}

/*
This function detects corners in an image frame using the Harris corners detection method and draws them on the output frame.

Parameters:

image: cv::Mat of the input image frame
output: cv::Mat of the output image frame
 */
int detectHarrisCorners(cv::Mat &src, cv::Mat &dst)
{
    int thresh = 150;
    int blockSize = 2;
    int apertureSize = 3;
    double k = 0.04;

    dst = src.clone();
    cv::Mat gray, tmp, tmp_norm;
    cv::cvtColor(src, gray, cv::COLOR_BGR2GRAY);
    tmp = cv::Mat::zeros(src.size(), CV_32FC1);

    cornerHarris(gray, tmp, blockSize, apertureSize, k);

    cv::normalize(tmp, tmp_norm, 0, 255, cv::NORM_MINMAX, CV_32FC1, cv::Mat());

    for (int j = 0; j < tmp_norm.rows; j++)
    {
        for (int i = 0; i < tmp_norm.cols; i++)
        {
            if ((int)tmp_norm.at<float>(j, i) > thresh)
            {
                cv::circle(dst, cv::Point(i, j), 2, cv::Scalar(0, 0, 255), 2, 8, 0);
            }
        }
    }

    return (0);
}
