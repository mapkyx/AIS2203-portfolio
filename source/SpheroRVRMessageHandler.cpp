#include "SpheroRVRMessageHandler.hpp"
#include "SpheroRVRController.hpp"
#include "message_handler.hpp"

// Implement the constructor
SpheroRVRMessageHandler::SpheroRVRMessageHandler(SpheroRVRController& controller)
    : controller_(controller) {}

// Implement the handle_message function
std::string SpheroRVRMessageHandler::handle_message(const std::string& message) {

    controller_.process_message(message);

    return "Command received: " + message;
}