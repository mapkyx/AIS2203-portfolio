#ifndef AIS2203_PORTFOLIO_SPHERORVRCONTROLLER_HPP
#define AIS2203_PORTFOLIO_SPHERORVRCONTROLLER_HPP

#pragma once

#include <iostream>

class SpheroRVRController {
public:
    // Constructor
    SpheroRVRController();

    // Function to process messages
    void process_message(const std::string& message);

private:
    // Add any private members or helper functions as needed
};

#endif //AIS2203_PORTFOLIO_SPHERORVRCONTROLLER_HPP
