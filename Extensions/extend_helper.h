/*
Puja Chaudhury
Functions for various steps used during the calibration of camera and projecting 3D points in world coordinates to 2D image pixel coordinates.
*/

#ifndef calibrate_hpp
#define calibrate_hpp

#include <stdio.h>
#include <iostream>

#include <opencv2/core.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/calib3d.hpp>

bool extractCircleCenters(cv::Mat &src, cv::Mat &dst, std::vector<cv::Point2f> &centers, bool drawCenters);

int specifyCalibration(std::vector<cv::Point2f> &centers, std::vector<std::vector<cv::Point2f>> &centers_list, std::vector<cv::Vec3f> &points, std::vector<std::vector<cv::Vec3f>> &points_list);

float computeCameraParameters(std::vector<std::vector<cv::Vec3f>> &points_list, std::vector<std::vector<cv::Point2f>> &centers_list, cv::Mat &camera_matrix, cv::Mat &dist_coeff);
int storeCalibrationData(cv::Mat &camera_matrix, cv::Mat &dist_coeff);

int loadCalibration(std::string csv_filename, cv::Mat &camera_matrix, cv::Mat &dist_coeff);

int calculateCameraPosition(std::vector<cv::Vec3f> &points, std::vector<cv::Point2f> &centers, cv::Mat &camera_matrix, cv::Mat &dist_coeff, cv::Mat &rot, cv::Mat &trans);

int draw3dAxes(cv::Mat &src, cv::Mat &camera_matrix, cv::Mat &dist_coeff, cv::Mat &rot, cv::Mat &trans);

int draw3dObject(cv::Mat &src, cv::Mat &camera_matrix, cv::Mat &dist_coeff, cv::Mat &rot, cv::Mat &trans);

int drawOnTarget(cv::Mat &src, cv::Mat &dst, cv::Mat &camera_matrix, cv::Mat &dist_coeff, cv::Mat &rot, cv::Mat &trans, std::string img_filename);

#endif /* calibrate_hpp */
