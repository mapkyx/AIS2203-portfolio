#pragma once

#include <string>

class message_handler {
public:
    virtual ~message_handler() = default;
    virtual std::string handle_message(const std::string& message) = 0;
};