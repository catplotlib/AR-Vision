/*
Puja Chaudhury
*/

#ifndef project_hpp
#define project_hpp

#include <stdio.h>
#include <filesystem>
#include <iostream>

#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/calib3d.hpp>

int loadCalibration(std::string csv_filename, cv::Mat &camera_matrix, cv::Mat &dist_coeff);

int calculateCameraPosition(std::vector<cv::Vec3f> &points, std::vector<cv::Point2f> &corners, cv::Mat &camera_matrix, cv::Mat &dist_coeff, cv::Mat &rot, cv::Mat &trans);

int draw3dAxes(cv::Mat &src, cv::Mat &camera_matrix, cv::Mat &dist_coeff, cv::Mat &rot, cv::Mat &trans);

int draw3dObject(cv::Mat &src, cv::Mat &camera_matrix, cv::Mat &dist_coeff, cv::Mat &rot, cv::Mat &trans);

int detectHarrisCorners(cv::Mat &src, cv::Mat &dst);

#endif
