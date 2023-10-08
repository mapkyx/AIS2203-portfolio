//
// Created by Admin on 08/10/2023.
//

#ifndef AIS2203_PORTFOLIO_CAMERA_CONTROL_H
#define AIS2203_PORTFOLIO_CAMERA_CONTROL_H

#include <librealsense2/rs.hpp>
#include <opencv2/opencv.hpp>

class CameraController {
public:
    CameraController();
    ~CameraController();

    cv::Mat getCameraData();
    // ... other member functions ...

private:
    rs2::pipeline pipe;
    // ... other private member variables for camera handling ...
};

#endif //AIS2203_PORTFOLIO_CAMERA_CONTROL_H
