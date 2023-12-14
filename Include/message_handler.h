#ifndef AIS2203_PORTFOLIO_MESSAGE_HANDLER_H
#define AIS2203_PORTFOLIO_MESSAGE_HANDLER_H

#pragma once

#include <string>

class message_handler {
public:
    virtual ~message_handler() = default;
    virtual std::string handle_message(const std::string& message) = 0;
};


#endif //AIS2203_PORTFOLIO_MESSAGE_HANDLER_H
