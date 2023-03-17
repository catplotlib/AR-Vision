/*
Puja Chaudhury
Implementation of functions used for camera calibration and projecting 3D points to 2D image coordinates.
*/

#include "extend_helper.h"
#include "helper_csv_extend.h"

/*
The function takes in an image frame as a cv::Mat,
an output frame as another cv::Mat, and a vector of points.
It detects the centers of the circles present in the circle grid using a specific method and draws them on the output frame.
Additionally, this function fills the given vector with the pixel coordinates of the detected circle centers in the image.
 */
bool extractCircleCenters(cv::Mat &src, cv::Mat &dst, std::vector<cv::Point2f> &centers, bool drawCenters)
{
    dst = src.clone();

    bool found = cv::findCirclesGrid(src, cv::Size(4, 11), centers, cv::CALIB_CB_ASYMMETRIC_GRID + cv::CALIB_CB_CLUSTERING);
    if (drawCenters)
    {
        cv::drawChessboardCorners(dst, cv::Size(4, 11), centers, found);
    }

    return (found);
}

/*
This function takes a vector of points that represents the pixel coordinates of detected circle centers in an image.
It then computes the corresponding world coordinates for the checkerboard target and populates another vector of points with these values.
Additionally, it stores the multiple point sets and center sets that will be used in calibration.
 */
int specifyCalibration(std::vector<cv::Point2f> &corners, std::vector<std::vector<cv::Point2f>> &corners_list, std::vector<cv::Vec3f> &points, std::vector<std::vector<cv::Vec3f>> &points_list)
{
    points.push_back(cv::Vec3f(10, 7, 0));
    points.push_back(cv::Vec3f(10, 5, 0));
    points.push_back(cv::Vec3f(10, 3, 0));
    points.push_back(cv::Vec3f(10, 1, 0));
    points.push_back(cv::Vec3f(9, 6, 0));
    points.push_back(cv::Vec3f(9, 4, 0));
    points.push_back(cv::Vec3f(9, 2, 0));
    points.push_back(cv::Vec3f(9, 0, 0));
    points.push_back(cv::Vec3f(8, 7, 0));
    points.push_back(cv::Vec3f(8, 5, 0));
    points.push_back(cv::Vec3f(8, 3, 0));
    points.push_back(cv::Vec3f(8, 1, 0));
    points.push_back(cv::Vec3f(7, 6, 0));
    points.push_back(cv::Vec3f(7, 4, 0));
    points.push_back(cv::Vec3f(7, 2, 0));
    points.push_back(cv::Vec3f(7, 0, 0));
    points.push_back(cv::Vec3f(6, 7, 0));
    points.push_back(cv::Vec3f(6, 5, 0));
    points.push_back(cv::Vec3f(6, 3, 0));
    points.push_back(cv::Vec3f(6, 1, 0));
    points.push_back(cv::Vec3f(5, 6, 0));
    points.push_back(cv::Vec3f(5, 4, 0));
    points.push_back(cv::Vec3f(5, 2, 0));
    points.push_back(cv::Vec3f(5, 0, 0));
    points.push_back(cv::Vec3f(4, 7, 0));
    points.push_back(cv::Vec3f(4, 5, 0));
    points.push_back(cv::Vec3f(4, 3, 0));
    points.push_back(cv::Vec3f(4, 1, 0));
    points.push_back(cv::Vec3f(3, 6, 0));
    points.push_back(cv::Vec3f(3, 4, 0));
    points.push_back(cv::Vec3f(3, 2, 0));
    points.push_back(cv::Vec3f(3, 0, 0));
    points.push_back(cv::Vec3f(2, 7, 0));
    points.push_back(cv::Vec3f(2, 5, 0));
    points.push_back(cv::Vec3f(2, 3, 0));
    points.push_back(cv::Vec3f(2, 1, 0));
    points.push_back(cv::Vec3f(1, 6, 0));
    points.push_back(cv::Vec3f(1, 4, 0));
    points.push_back(cv::Vec3f(1, 2, 0));
    points.push_back(cv::Vec3f(1, 0, 0));
    points.push_back(cv::Vec3f(0, 7, 0));
    points.push_back(cv::Vec3f(0, 5, 0));
    points.push_back(cv::Vec3f(0, 3, 0));
    points.push_back(cv::Vec3f(0, 1, 0));

    corners_list.push_back(corners);
    points_list.push_back(points);

    return (0);
}

/*
This function takes in vectors containing lists of point sets and center sets,
as well as an initial camera matrix.
It performs calibration and calculates the calibrated camera matrix and distortion coefficients.
Additionally, it returns the reprojection error.
 */
float computeCameraParameters(std::vector<std::vector<cv::Vec3f>> &points_list, std::vector<std::vector<cv::Point2f>> &centers_list, cv::Mat &camera_matrix, cv::Mat &dist_coeff)
{
    std::vector<cv::Mat> rot, trans;

    float error = cv::calibrateCamera(points_list,
                                      centers_list,
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
This function stores the current camera matrix and distortion coefficients as calibration data into a CSV file.
The data can be later retrieved to calculate the camera pose.
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

/*
Given a CSV file containing calibration data,
this function reads the file and retrieves the calibrated camera matrix and distortion coefficients.
 */
int loadCalibration(std::string csv_filename, cv::Mat &camera_matrix, cv::Mat &dist_coeff)
{
    char *fname_char = new char[csv_filename.length() + 1];
    strcpy(fname_char, csv_filename.c_str());

    std::cout << "Retrieving saved calibration..." << std::endl;
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

This function takes in a vector of current points and center set,
as well as the calibrated camera matrix and distortion coefficients.
It then calculates the position of the camera with respect to the target and stores the rotation and translation data in output arrays.
 */
int calculateCameraPosition(std::vector<cv::Vec3f> &points, std::vector<cv::Point2f> &centers, cv::Mat &camera_matrix, cv::Mat &dist_coeff, cv::Mat &rot, cv::Mat &trans)
{
    cv::solvePnP(points, centers, camera_matrix, dist_coeff, rot, trans);

    return (0);
}

/*
 Given a cv::Mat of the image frame, calibrated camera matrix, distortion coefficients, rotation and translation data
 from the current estimated camera position, this function projects 3D world coordinates of axes to image pixel
 coordinates on the image frame and draws lines between these points to generate the 3D axes at origin.
 */
int draw3dAxes(cv::Mat &src, cv::Mat &camera_matrix, cv::Mat &dist_coeff, cv::Mat &rot, cv::Mat &trans)
{
    std::vector<cv::Vec3f> points;
    points.push_back(cv::Vec3f({0, 0, 0}));
    points.push_back(cv::Vec3f({2, 0, 0}));
    points.push_back(cv::Vec3f({0, 2, 0}));
    points.push_back(cv::Vec3f({0, 0, 2}));

    std::vector<cv::Point2f> centers;

    cv::projectPoints(points, rot, trans, camera_matrix, dist_coeff, centers);

    cv::arrowedLine(src, centers[0], centers[1], cv::Scalar(0, 0, 255), 5);
    cv::arrowedLine(src, centers[0], centers[2], cv::Scalar(0, 255, 0), 5);
    cv::arrowedLine(src, centers[0], centers[3], cv::Scalar(255, 0, 0), 5);

    return (0);
}

/*
Given a cv::Mat image frame, calibrated camera matrix, distortion coefficients,
rotation and translation data of the current estimated camera position,
this function projects 3D world coordinates of axes onto the image plane and draws
lines between these points to create a 3D axes visualization at the origin in image pixel coordinates.
 */
int draw3dObject(cv::Mat &src, cv::Mat &camera_matrix, cv::Mat &dist_coeff, cv::Mat &rot, cv::Mat &trans)
{
    std::vector<cv::Vec3f> points;
    std::vector<cv::Point2f> corners;

    
    // Draw a cylinder on the chessboard
    float height = 4;
    float radius = 1;
    cv::Point3f center(5, 5, height / 2);
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
    cv::Point3f centerCircle(3, 3, radCircle);
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
Given a cv::Mat of the input frame, a cv::Mat of the output frame, calibrated camera matrix,
distortion coefficients, rotation & translation data and filename for artwork image,
this function applies a perspective transformation to the artwork image and overlays
it onto the target in the output frame.
 */
int drawOnTarget(cv::Mat &src, cv::Mat &dst, cv::Mat &camera_matrix, cv::Mat &dist_coeff, cv::Mat &rot, cv::Mat &trans, std::string img_filename)
{
    cv::Point2f inputQuad[4];
    cv::Point2f outputQuad[4];

    cv::Mat lambda(2, 4, CV_32FC1);
    cv::Mat canvas;

    canvas = cv::imread(img_filename, 1);

    lambda = cv::Mat::zeros(canvas.rows, canvas.cols, canvas.type());

    inputQuad[0] = cv::Point2f(0, 0);
    inputQuad[1] = cv::Point2f(canvas.cols, 0);
    inputQuad[2] = cv::Point2f(canvas.cols - 1, canvas.rows - 1);
    inputQuad[3] = cv::Point2f(0, canvas.rows - 1);

    std::vector<cv::Vec3f> points;
    points.push_back(cv::Vec3f({-3, 9, 0}));
    points.push_back(cv::Vec3f({13, 9, 0}));
    points.push_back(cv::Vec3f({13, -2, 0}));
    points.push_back(cv::Vec3f({-3, -2, 0}));

    std::vector<cv::Point2f> centers;

    cv::projectPoints(points, rot, trans, camera_matrix, dist_coeff, centers);

    outputQuad[0] = centers[0];
    outputQuad[1] = centers[1];
    outputQuad[2] = centers[2];
    outputQuad[3] = centers[3];

    std::vector<cv::Point> vertices{outputQuad[0], outputQuad[1], outputQuad[2], outputQuad[3]};
    std::vector<std::vector<cv::Point>> pts{vertices};
    cv::fillPoly(src, pts, cv::Scalar(0, 0, 0));

    lambda = getPerspectiveTransform(inputQuad, outputQuad);
    warpPerspective(canvas, dst, lambda, dst.size());

    src.copyTo(dst, src);

    return (0);
}
