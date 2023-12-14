#pragma once

#include <boost/asio.hpp>
#include <string>

void camera_feed_receiver(boost::asio::io_service& io_service, const std::string& host, short port);