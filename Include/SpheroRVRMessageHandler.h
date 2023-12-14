#ifndef AIS2203_PORTFOLIO_SPHERORVRMESSAGEHANDLER_H
#define AIS2203_PORTFOLIO_SPHERORVRMESSAGEHANDLER_H

// SpheroRVRMessageHandler.h
#pragma once

#include <string>

class SpheroRVRController; // Forward declaration

class SpheroRVRMessageHandler {
public:
    // Constructor taking a reference to SpheroRVRController
    SpheroRVRMessageHandler(SpheroRVRController& controller);

    // Function to handle messages
    std::string handle_message(const std::string& message);

private:
    // Reference to SpheroRVRController
    SpheroRVRController& controller_;
};


#endif //AIS2203_PORTFOLIO_SPHERORVRMESSAGEHANDLER_H
