#include <QApplication>
#include <opencv2/highgui/highgui.h>
#include "RVR-Control/rvr_controller.h"
#include "GUI/gui.h"
#include "Camera/camera_control.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);


    // Initialize RVR Controller
    RVRController rvrController;
    rvrController.initialize();

    // Initialize GUI Controller
    GUIController guiController;
    guiController.show();  // Display the GUI

    // Initialize Camera Controller
    CameraController cameraController;

    // Main loop
    while (true) {
        // Get data from RealSense camera
        cv::Mat frame = cameraController.getCameraData();

        // Process frame using OpenCV
        // Detect obstacles, apply filters, etc.

        // Display frame using OpenCV
        cv::imshow("Camera Feed", frame);
        cv::waitKey(1);  // Refresh display

        // Update GUI with robot state
        // For example:
        int currentSpeed = 10;  // This is just a placeholder. You'd get the actual speed from the RVR.
        guiController.updateSpeed(currentSpeed);

        // Check for user input and control RVR accordingly
        // ...

        // Process Qt events
        app.processEvents();
    }

    return app.exec();
}
