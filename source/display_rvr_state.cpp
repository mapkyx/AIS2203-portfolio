#include "sphero/display_rvr_state.hpp"
#include <opencv2/opencv.hpp>

using namespace cv;

void displayRvrState(const std::string& battery, const std::string& lastCommand, const std::string& mode) {
    Mat displayImg(200, 400, CV_8UC3, Scalar(255, 255, 255));

    putText(displayImg, "Battery: " + battery, Point(10, 80), FONT_HERSHEY_SIMPLEX, 0.7, Scalar(0, 0, 0), 2);
    putText(displayImg, "Last Command: " + lastCommand, Point(10, 130), FONT_HERSHEY_SIMPLEX, 0.7, Scalar(0, 0, 0), 2);
    putText(displayImg, "Mode: " + mode, Point(10, 180), FONT_HERSHEY_SIMPLEX, 0.7, Scalar(0, 0, 0), 2);

    imshow("RVR State", displayImg);
    waitKey(1);
}
