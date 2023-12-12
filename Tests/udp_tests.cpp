#include <catch2/catch_test_macros.hpp>
#include <Include/Server/udp_server.h>

namespace {

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

