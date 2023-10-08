//
// Created by Admin on 08/10/2023.
//

#ifndef AIS2203_PORTFOLIO_GUI_H
#define AIS2203_PORTFOLIO_GUI_H

#include <librealsense2/rs.hpp>
#include <QWidget>
#include <QLabel>

class CameraController {
public:
    CameraController();
    ~CameraController();

    rs2::frameset getCameraData();
    // ... other member functions ...

private:
    rs2::pipeline pipe;
    // ... other private member variables for camera handling ...
};

#endif //AIS2203_PORTFOLIO_GUI_H
