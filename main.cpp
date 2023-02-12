
#include <glog/logging.h>
#include <opencv2/opencv.hpp>
#include <iostream>

#include "board_config.h"

struct CameraPara
{
    cv::Mat camera_matrix;
    cv::Mat dist_coeffs;
    cv::Mat scale_xy;
    cv::Mat shift_xy;

    friend std::ostream &operator<<(std::ostream &os, const CameraPara &para);
};

std::ostream &operator<<(std::ostream &os, const CameraPara &para)
{
    os << "intrinsic is :" << para.camera_matrix << std::endl;
    os << "distort is :" << para.dist_coeffs << std::endl;
    os << "scale xy is :" << para.scale_xy << std::endl;
    os << "shift xy is :" << para.shift_xy << std::endl;
    return os;
}

void readCameraParams(const std::string &filename, CameraPara &para)
{

    cv::FileStorage file_storage(filename, cv::FileStorage::Mode::READ);

    cv::Mat camera_matrix;
    file_storage["camera_matrix"] >> camera_matrix;

    cv::Mat dist_coeffs;
    file_storage["dist_coeffs"] >> dist_coeffs;

    cv::Mat scale_xy;
    file_storage["scale_xy"] >> scale_xy;

    cv::Mat shift_xy;
    file_storage["shift_xy"] >> shift_xy;

    para.camera_matrix = camera_matrix.clone();
    para.dist_coeffs = dist_coeffs.clone();
    para.scale_xy = scale_xy.clone();
    para.shift_xy = shift_xy.clone();

    file_storage.release();
}

int main(int argc, char **argv)
{

    std::string path = "../data/";
    std::string camera = "left";
    std::string image_name = path + camera + ".png";
    cv::Mat image = cv::imread(image_name, 0);
    cv::imshow("image", image);
    cv::waitKey(0);

    if (image.empty())
    {
        LOG(INFO) << "image is empty";
        return -1;
    }

    // step0 : read camera params
    std::string params_path = path + camera + ".yaml";
    CameraPara camera_params;
    readCameraParams(params_path, camera_params);
    LOG(INFO) << "camera para is ::::::::::::::::::::::::::::::::::::::::";
    LOG(INFO) << camera_params;

    // step1 : undistort
    cv::Mat new_intrinsic = camera_params.camera_matrix.clone();
    new_intrinsic.at<double>(0, 0) *= camera_params.scale_xy.at<float>(0, 0);
    new_intrinsic.at<double>(1, 1) *= camera_params.scale_xy.at<float>(1, 0);
    new_intrinsic.at<double>(0, 2) += camera_params.shift_xy.at<float>(0, 0);
    new_intrinsic.at<double>(1, 2) += camera_params.shift_xy.at<float>(1, 0);
    LOG(INFO) << "new intrinsic is :";
    LOG(INFO) << new_intrinsic;

    cv::Mat undistort_image;
    cv::fisheye::undistortImage(image, undistort_image, camera_params.camera_matrix, camera_params.dist_coeffs, new_intrinsic);

    std::string undistort_name = path + camera + "_undistort.png";
    cv::imwrite(undistort_name, undistort_image);
    cv::namedWindow("undistort", cv::WINDOW_NORMAL);
    cv::imshow("undistort", undistort_image);
    cv::waitKey(0);

    // step2 : find correspondences
    std::vector<cv::Point2f> image_points;
    std::vector<cv::Point2f> object_points;

    BoardConfig board_config;

    if (camera == "front")
    {
        image_points.emplace_back(231, 264);
        image_points.emplace_back(535, 233);
        image_points.emplace_back(53, 387);
        image_points.emplace_back(862, 324);

        board_config.getFrontBoard(object_points);
    }
    else if (camera == "left")
    {
        // image_points.emplace_back(777, 218);
        // image_points.emplace_back(437, 239);
        // image_points.emplace_back(913, 398);
        // image_points.emplace_back(200, 487);

        image_points.emplace_back(409, 238);
        image_points.emplace_back(753, 220);
        image_points.emplace_back(117, 498);
        image_points.emplace_back(873, 404);
        board_config.getLeftBoard(object_points);
    }

    // step3: compute perspective matrix
    cv::Mat transform = cv::getPerspectiveTransform(image_points, object_points);

    // step4: convert to birdview
    cv::Mat bird_image = cv::Mat::zeros(image.size(), image.type());
    cv::warpPerspective(undistort_image, bird_image, transform, bird_image.size());

    cv::imshow("bird image", bird_image);
    cv::waitKey(0);

    // step5 : stitching

    LOG(INFO) << "hello world";

    return 0;
}