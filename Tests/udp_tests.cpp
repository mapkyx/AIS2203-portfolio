#define CATCH_CONFIG_MAIN
#include <catch2/catch_test_macros.hpp>
<<<<<<< Updated upstream
#include <Include/Server/udp_server.h>
=======
>>>>>>> Stashed changes

#include <thread>
#include <boost/asio.hpp>
#include <boost/asio/ip/udp.hpp>
#include <boost/asio/buffer.hpp>
#include "/Users/johanneslervag/CLionProjects/Sphero_rvr_v2/Include/Server/udp_server.hpp"

<<<<<<< Updated upstream
const int port = 22;

struct my_message_handler : message_handler {

  my_message_handler(const std::function<std::string(const std::string &)> &f) : f_(f) {}

        std::string handle_message(const std::string &message) override {
            return f_(message);
        }

    private:
            std::function<std::string(const std::string&)> f_;
    };

}

    TEST_CASE("Test UDP server start/stop") {
    for (int i = 0; i < 5; i++) {
            UDPServer server(port, nullptr);
            server.start();
            std::this_thread::sleep_for(std::chrono::milliseconds(50));

            server.stop();
         }

    }

    TEST_CASE("Test handler") {
        auto msgGenerator = [](auto msg) {
            return "Good morning, " + msg + "!"
        };

        my_message_handler handler(msgGenerator);

        std::string msg("Per")

        auto expected = msgGenerator(msg);
        auto actual = handler.handle_message(msg);

        CHECK(expected == actual);
    }

    TEST_CASE("Test socket communication") {

        auto msgGenerator = auto [](auto msg) {
            return "hello " + msg + "!";
        };

        my_message_handler handler (msgGenerator);

        udp_server server(port, &handler);
        server.start();

        udp_client client ("127.0.0.1", port);
        std::string msg("Nils");
        client.send(msg);

        auto expected = msgGenerator(msg);
        auto actual = client.recv();

        CHECK (actual == expected);
    }

=======
class UDPClient {
public:
    UDPClient(boost::asio::io_service& io_service, short port)
            : socket_(io_service), server_endpoint_(boost::asio::ip::address::from_string("127.0.0.1"), port) {
        socket_.open(boost::asio::ip::udp::v4());
    }

    void send(const std::string& message) {
        socket_.send_to(boost::asio::buffer(message), server_endpoint_);
    }

private:
    boost::asio::ip::udp::socket socket_;
    boost::asio::ip::udp::endpoint server_endpoint_;
};

TEST_CASE("Test UDP server communication") {
    constexpr short port = 9090;

    boost::asio::io_service io_service;
    udp_server server(port, nullptr);

    // Create a separate thread to run the io_service
    std::thread io_thread([&io_service]() {
        io_service.run();
    });

    // Wait for the server to start
    std::this_thread::sleep_for(std::chrono::milliseconds(50));

    SECTION("Test sending a message to the server") {
        UDPClient client(io_service, port);
        client.send("Hello, UDP Server!");

        // Add appropriate checks for the received message in the UDPServer handle_receive method
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }

    // Stop the server and join the thread
    io_service.stop();
    io_thread.join();
}
>>>>>>> Stashed changes
