//
// Created by Admin on 08/10/2023.
//

#include "camera_control.h"

CameraController::CameraController() {
    pipe.start();
}

CameraController::~CameraController() {
    // Destructor implementation, if needed
}

cv::Mat CameraController::getCameraData() {
    rs2::frameset frames = pipe.wait_for_frames();
    rs2::frame colorFrame = frames.get_color_frame();

    // Convert RealSense frame to OpenCV Mat
    cv::Mat colorMat(cv::Size(640, 480), CV_8UC3, (void*)colorFrame.get_data(), cv::Mat::AUTO_STEP);
    return colorMat;
    // ... other member function implementations ...
}