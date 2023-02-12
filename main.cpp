
#include <glog/logging.h>
#include <opencv2/opencv.hpp>

#include <iostream>

struct CameraPara
{
    cv::Mat camera_matrix;
    cv::Mat dist_coeffs;
    cv::Mat scale_xy;
    cv::Mat shift_xy;

    friend std::ostream& operator<<(std::ostream &os, const CameraPara &para);
};

std::ostream& operator<<(std::ostream &os,const CameraPara &para){
    os << "intrinsic is :" << para.camera_matrix << std::endl;
    os << "distort is :" << para.dist_coeffs<< std::endl;
    os << "scale xy is :" << para.scale_xy<< std::endl;
    os << "shift xy is :" << para.shift_xy<< std::endl;
    return os;
}


void readCameraParams(const std::string &filename, CameraPara &para){

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

int main(int argc, char **argv){

    cv::Mat image = cv::imread("../data/front.png");
    cv::imshow("image", image);
    cv::waitKey(0);

    if(image.empty()){
        LOG(INFO) << "image is empty";
        return -1;
    }

    //step0 : read camera params
    std::string params_path = "../data/front.yaml";
    CameraPara camera_params;
    readCameraParams(params_path, camera_params);
    LOG(INFO) << "camera para is ::::::::::::::::::::::::::::::::::::::::";
    LOG(INFO) << camera_params;

    //step1 : undistort
    cv::Mat new_intrinsic = camera_params.camera_matrix.clone();
    new_intrinsic.at<double>(0,0) *= camera_params.scale_xy.at<float>(0,0);
    new_intrinsic.at<double>(1,1) *= camera_params.scale_xy.at<float>(1,0);
    new_intrinsic.at<double>(0,2) += camera_params.shift_xy.at<float>(0,0);
    new_intrinsic.at<double>(1,2) += camera_params.shift_xy.at<float>(1,0);
    LOG(INFO) << "new intrinsic is :";
    LOG(INFO) << new_intrinsic;

    cv::Mat undistort_image;
    cv::fisheye::undistortImage(image, undistort_image, camera_params.camera_matrix, camera_params.dist_coeffs, new_intrinsic);

    cv::imwrite("../data/front_undistort.png", undistort_image);
    cv::namedWindow("undistort", cv::WINDOW_NORMAL);
    cv::imshow("undistort", undistort_image);
    cv::waitKey(0);

    //step2 : find correspondences
    std::vector<cv::Point2f> image_points;
    std::vector<cv::Point2f> object_points;



    //step3: compute perspective matrix



    //step4: convert to birdview



    //step5 : stitching



    LOG(INFO) << "hello world";

    return 0;
}