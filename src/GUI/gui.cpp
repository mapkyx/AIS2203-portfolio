//
// Created by Admin on 08/10/2023.
//

#include "gui.h"

GUIController::GUIController(QWidget *parent) : QWidget(parent) {
    this->setWindowTitle("RVR Controller");

    speedLabel = new QLabel("Speed: 0", this);
    speedLabel->move(10, 10);
    // ... initialize other GUI components ...
}

GUIController::~GUIController() {
    // Destructor implementation, if needed
}

void GUIController::updateSpeed(int speed) {
    speedLabel->setText(QString("Speed: %1").arg(speed));
    // ... other member function implementations ...
}