#ifndef RVR_STRUCTURED_PROJECT_UDPCLIENT_HPP
#define RVR_STRUCTURED_PROJECT_UDPCLIENT_HPP

#include <boost/asio.hpp>
#include <boost/asio/ip/udp.hpp>
#include <boost/asio/buffer.hpp>
#include <string>

// Forward declaration of udp_server
class udp_server;

class UDPClient {
public:
    UDPClient(boost::asio::io_service& io_service, const std::string& host, short port);
    void send_command(const std::string& command);
    std::string getLastSentCommand() const;

private:
    boost::asio::io_service& io_service_;
    boost::asio::ip::udp::socket socket_;
    boost::asio::ip::udp::endpoint endpoint_;
    std::string lastSentCommand_;
};

#endif //RVR_STRUCTURED_PROJECT_UDPCLIENT_HPP
