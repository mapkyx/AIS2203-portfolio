#pragma once
#include <boost/asio.hpp>
#include <string>

class UDPClient {
    public:
        UDPClient(boost::asio::io_service &io_service, const std::string &host, short port);

        void send_command(const std::string &command);

    private:
        boost::asio::io_service &io_service_;
        boost::asio::ip::udp::socket socket_;
        boost::asio::ip::udp::endpoint endpoint_;
    };