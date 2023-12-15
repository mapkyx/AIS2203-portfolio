#include "server/udp_client.hpp"
#include <iostream>

UDPClient::UDPClient(boost::asio::io_service& io_service, const std::string& host, short port)
        : io_service_(io_service), socket_(io_service_), endpoint_(boost::asio::ip::address::from_string(host), port) {
    socket_.open(boost::asio::ip::udp::v4());
}

void UDPClient::send_command(const std::string& command) {
    boost::system::error_code ec;
    socket_.send_to(boost::asio::buffer(command), endpoint_, 0, ec);
    if (ec) {
        std::cerr << "Failed to send command: " << ec.message() << std::endl;
    }
    else {
            lastSentCommand_ = command;  // Update the last sent command
        }

    std::string UDPClient::getLastSentCommand() const {
        return lastSentCommand_;
    }



