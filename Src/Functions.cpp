#include <iostream>
#include <sphero_sdk/sphero.hpp>
#include <game_controller_library/game_controller.hpp>

int main() {
    // Initialize RVR
    auto rvr = std::make_shared<sphero_sdk::Sphero>();
    rvr->connect(); // Implement your own connection logic

    // Initialize Game Controller
    auto controller = std::make_shared<game_controller_library::GameController>();
    controller->connect(); // Implement your own connection logic

    while (true) {
        // Read controller input
        float leftStickX = controller->getLeftStickX();
        float leftStickY = controller->getLeftStickY();

        // Map controller input to RVR commands
        float speed = leftStickY; // Use the Y-axis for speed
        float heading = leftStickX * 360.0; // Use the X-axis for heading

        // Send commands to RVR
        rvr->drive(speed, heading);

        // Add a delay to control the loop frequency
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }

    // Disconnect when done
    rvr->disconnect();
    controller->disconnect();

    return 0;
}




