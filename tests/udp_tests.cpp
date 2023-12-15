#define CATCH_CONFIG_MAIN
#include <catch2/catch_test_macros.hpp>
#include <thread>
#include <boost/asio.hpp>
#include <boost/asio/ip/udp.hpp>
#include <boost/asio/buffer.hpp>
#include "server/udp_client.hpp"

// Path to the Python script
const std::string pythonScriptPath = "include/Py/rvr.py";


TEST_CASE("Test UDP server communication") {
      std::thread pythonServerThread([]() {
            std::string command = "python " + pythonScriptPath;
            system(command.c_str());
        });

   // Wait for the server to start
       std::this_thread::sleep_for(std::chrono::milliseconds(500));

    SECTION("Test sending a message to the server") {
        // Create UDP client and send a message
                boost::asio::io_service io_service;
                UDPClient client(io_service, "127.0.0.1", 1234);

                // Modify this command based on the expected behavior in your server
                std::string testCommand = "start_forward";
                client.send_command(testCommand);

                // Add appropriate checks for the received message in the Python UDP server
                std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }

     // Stop the Python server thread
        pythonServerThread.detach();
}