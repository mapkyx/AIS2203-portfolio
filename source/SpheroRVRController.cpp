// SpheroRVRController.cpp
#include "sphero/SpheroRVRController.hpp"

// Implement the constructor
SpheroRVRController::SpheroRVRController() {

}
void SpheroRVRController::process_message(const std::string& message) {
    // Print the received message to the console
    std::cout << "Received message: " << message << std::endl;

}


