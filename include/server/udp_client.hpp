#ifndef PORTFOLIO_UDPCLIENT_HPP
#define PORTFOLIO_UDPCLIENT_HPP

#include <boost/asio.hpp>
#include <boost/asio/ip/udp.hpp>
#include <boost/asio/buffer.hpp>
#include <string>

class UDPClient {
    public:
        UDPClient(boost::asio::io_service &io_service, const std::string &host, short port);

        void send_command(const std::string &command);
        std::string getLastSentCommand() const;

    private:
       boost::asio::io_service& io_service_;
       boost::asio::ip::udp::socket socket_;
       boost::asio::ip::udp::endpoint endpoint_;
       std::string lastSentCommand_;
    };

#endif //PORTFOLIO_UDPCLIENT_HPP