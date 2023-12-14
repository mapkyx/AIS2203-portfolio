#include "server/udp_listener.hpp"
#include "sphero/shared_data.hpp"
#include <boost/asio.hpp>
#include <iostream>
#include <array>
#include <string>

extern std::atomic<bool> isRunning;

void udp_listener() {
    boost::asio::io_service io_service;
    boost::asio::ip::udp::socket socket(io_service, boost::asio::ip::udp::endpoint(boost::asio::ip::udp::v4(), 1236));
    std::array<char, 1024> recv_buf{};
    std::cout << "Listening for data on port 1236..." << std::endl;

    while (isRunning.load()) {
        boost::asio::ip::udp::endpoint remote_endpoint;
        boost::system::error_code error;
        size_t len = socket.receive_from(boost::asio::buffer(recv_buf), remote_endpoint, 0, error);

        std::string stateInfo(recv_buf.data(), len);

        auto separator1 = stateInfo.find(',');
        auto separator2 = stateInfo.find(',', separator1 + 1);
        std::string batteryParsed = stateInfo.substr(0, separator1);
        std::string lastCommandParsed = stateInfo.substr(separator1 + 1, separator2 - separator1 - 1);
        std::string modeParsed = stateInfo.substr(separator2 + 1);

        {
            std::lock_guard<std::mutex> guard(sharedDataMutex);
            sharedData.battery = batteryParsed;
            sharedData.direction = lastCommandParsed;
            sharedData.mode = modeParsed;
        }

        if (error && error != boost::asio::error::message_size) {
            std::cerr << "Receive error: " << error.message() << std::endl;
        }
    }
}
