// message_handler.cpp
#include "message_handler.h"
#include <iostream>

std::string message_handler::handle_message(const std::string& message) {
    // Implementation here
    std::cout << "Received message: " << message << std::endl;

    return "Response: " + message;
}


