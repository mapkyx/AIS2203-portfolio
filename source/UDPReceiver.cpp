#include <iostream>
#include <thread>
#include <boost/asio.hpp>
#include <array>

class UDPServer {
public:
    UDPServer(boost::asio::io_service& io_service, short port)
            : socket_(io_service, boost::asio::ip::udp::endpoint(boost::asio::ip::udp::v4(), port)) {
        start_receive();
    }

private:
    void start_receive() {
        socket_.async_receive_from(
                boost::asio::buffer(recv_buffer_), remote_endpoint_,
                [this](boost::system::error_code ec, std::size_t bytes_transferred) {
                    if (!ec) {
                        handle_receive(bytes_transferred);
                        start_receive();
                    }
                });
    }

    void handle_receive(std::size_t bytes_transferred) {
        std::string received_data(recv_buffer_.data(), bytes_transferred);
        std::cout << "Received: " << received_data << std::endl;
    }

    boost::asio::ip::udp::socket socket_;  // Removed the default initialization here
    boost::asio::ip::udp::endpoint remote_endpoint_;
    std::array<char, 1024> recv_buffer_;
};

int main() {
    try {
        boost::asio::io_service io_service;
        UDPServer server(io_service, 22);

        std::cout << "Succeeded with connecting through UDP." << std::endl;

        // Create a separate thread to run the io_service
        std::thread io_thread([&io_service]() {
            io_service.run();
        });

        // Here you can add more threads for other tasks if needed

        io_thread.join();
    } catch (std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }

    return 0;
}