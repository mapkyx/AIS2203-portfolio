// message_handler.cpp
#include "message_handler.hpp"
#include <iostream>

std::string message_handler::handle_message(const std::string& message) {
    // Your implementation here
    std::cout << "Received message: " << message << std::endl;

    // Add custom processing logic as needed
    // For now, just return the received message as a response
    return "Response: " + message;
}
