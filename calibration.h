/*
Puja Chaudhury
Calibration.h
The calibration process for the camera and AR system is split into several functions, each responsible for a specific step.
*/

#ifndef calibration_hpp
#define calibration_hpp

#include <stdio.h>
#include <iostream>

#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/calib3d.hpp>

bool GetChessboardCorners(cv::Mat &src, cv::Mat &dst, std::vector<cv::Point2f> &corners, bool drawCorners);
int specifyCalibration(std::vector<cv::Point2f> &corners, std::vector<std::vector<cv::Point2f>> &corners_list, std::vector<cv::Vec3f> &points, std::vector<std::vector<cv::Vec3f>> &points_list);
float computeCameraParameters(std::vector<std::vector<cv::Vec3f>> &points_list, std::vector<std::vector<cv::Point2f>> &corners_list, cv::Mat &camera_matrix, cv::Mat &dist_coeff);
int storeCalibrationData(cv::Mat &camera_matrix, cv::Mat &dist_coeff);

#endif
