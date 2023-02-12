
#include <vector>
#include <opencv2/opencv.hpp>

class BoardConfig{
public:

    BoardConfig() = default;

    ~BoardConfig() = default;

    float shift_w = 300.0;
    float shift_h = 300.0;

    void getFrontBoard(std::vector<cv::Point2f> &points){
        points.clear();
        points.emplace_back(shift_w + 120.0, shift_h);
        points.emplace_back(shift_w + 480.0, shift_h);
        points.emplace_back(shift_w + 120.0, shift_h + 160.0);
        points.emplace_back(shift_w + 480.0, shift_h + 160.0);
    }

    void getLeftBoard(std::vector<cv::Point2f> &points){
        points.clear();
        points.emplace_back(shift_h + 280.0, shift_w);
        points.emplace_back(shift_h + 840.0, shift_w);
        points.emplace_back(shift_h + 280.0, shift_w + 160.0);
        points.emplace_back(shift_h + 840.0, shift_w + 160.0);
    }

};