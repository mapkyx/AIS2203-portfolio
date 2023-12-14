<<<<<<< Updated upstream:Include/Server/udp_server.hpp
#ifndef UDP_SERVER_HPP
#define UDP_SERVER_HPP
=======
// udp_server.hpp
#pragma once
>>>>>>> Stashed changes:include/server/udp_server.hpp

#include <functional>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <iostream>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include "message_handler.hpp"

class udp_server {
public:
    udp_server(int port, message_handler* handler)
            : port_(port), message_handler_(handler), running_(false) {}

    ~udp_server() {
        if (running_) {
            stop();
        }
    }

    void start() {
        if (!running_) {
            running_ = true;
            server_thread_ = std::thread(&udp_server::run, this);
        }
    }

    void stop() {
        if (running_) {
            running_ = false;
            if (server_thread_.joinable()) {
                server_thread_.join();
            }
        }
    }

private:
    void run() {
        int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
        if (sockfd == -1) {
            perror("socket");
            return;
        }

        sockaddr_in server_addr{};
        server_addr.sin_family = AF_INET;
        server_addr.sin_addr.s_addr = INADDR_ANY;
        server_addr.sin_port = htons(port_);

        if (bind(sockfd, reinterpret_cast<struct sockaddr*>(&server_addr), sizeof(server_addr)) == -1) {
            perror("bind");
            close(sockfd);
            return;
        }

        std::cout << "UDP Server listening on port " << port_ << std::endl;

        char buffer[1024];
        while (running_) {
            sockaddr_in client_addr{};
            socklen_t addr_len = sizeof(client_addr);

            ssize_t bytes_received = recvfrom(sockfd, buffer, sizeof(buffer), 0,
                                              reinterpret_cast<struct sockaddr*>(&client_addr), &addr_len);

            if (bytes_received == -1) {
                perror("recvfrom");
                continue;
            }

            buffer[bytes_received] = '\0';
            std::string received_msg(buffer);

            std::string response = message_handler_->handle_message(received_msg);

            sendto(sockfd, response.c_str(), response.length(), 0,
                   reinterpret_cast<struct sockaddr*>(&client_addr), addr_len);
        }

        close(sockfd);
    }

private:
    int port_;
    message_handler* message_handler_;
    std::thread server_thread_;
    bool running_;
};

<<<<<<< Updated upstream:Include/Server/udp_server.hpp
    std::thread t_;

    bool stopped{false};
    std::atomic<bool> stop_{false};
};

#endif//UDP_SERVER_HPP
=======
>>>>>>> Stashed changes:include/server/udp_server.hpp
