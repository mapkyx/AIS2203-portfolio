
#ifndef TESTABLE_NETWORKING_SOCKET_HANDLER_HPP
#define TESTABLE_NETWORKING_SOCKET_HANDLER_HPP

#include <atomic>
#include <iostream>
#include <memory>
#include <string>
#include <thread>

#include "network_helper.hpp"
#include <boost/asio.hpp>

using namespace boost::asio;
using namespace boost::asio::ip;

struct message_handler {

    virtual std::string handle_message(const std::string &message) = 0;
};


class connection {

public:
    explicit connection(std::unique_ptr<udp::socket> socket, message_handler *handler)
            : socket_(std::move(socket)), listener_(handler) {}

    connection(const connection &) = delete;
    connection(const connection &&) = delete;

    ~connection() {
        if (t_.joinable()) {
            t_.join();
        }
    }

private:
    std::thread t_;
    message_handler *listener_;
    std::unique_ptr<udp::socket> socket_;

    int recvSize() {
        std::array<unsigned char, 4> buf{};
        boost::asio::read(*socket_, boost::asio::buffer(buf), boost::asio::transfer_exactly(4));
        return bytes_to_int(buf);
    }

    std::string recvMsg() {

        int len = recvSize();

        boost::asio::streambuf buf;
        boost::system::error_code err;
        boost::asio::read(*socket_, buf, boost::asio::transfer_exactly(len), err);

        if (err) {
            throw boost::system::system_error(err);
        }

        std::string data(boost::asio::buffer_cast<const char *>(buf.data()), len);
        return data;
    }

    void write(const std::string &msg) {
        int msgSize = static_cast<int>(msg.size());

        socket_->send(boost::asio::buffer(int_to_bytes(msgSize), 4));
        socket_->send(boost::asio::buffer(msg));
    }

    void run_handler() {
        t_ = std::thread([this] {
            try {
                while (true) {

                    auto msg = recvMsg();
                    auto response = listener_->handle_message(msg);
                    write(response);
                }
            } catch (const std::exception &ex) {
                std::cerr << "[socket_handler] " << ex.what() << std::endl;
            }
        });
    }

    friend class udp_server;
};

#endif//TESTABLE_NETWORKING_SOCKET_HANDLER_HPP