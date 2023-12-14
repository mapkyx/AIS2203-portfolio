#include "camera_feed.hpp"
#include <opencv2/opencv.hpp>
#include <iostream>
#include <array>
#include <vector>

using namespace cv;

void camera_feed_receiver(boost::asio::io_service& io_service, const std::string& host, short port) {
    boost::asio::ip::udp::socket socket(io_service, boost::asio::ip::udp::endpoint(boost::asio::ip::udp::v4(), 1236));
    boost::asio::ip::udp::endpoint sender_endpoint;

    while (true) {
        std::array<char, 4> size_buf;
        size_t len = socket.receive_from(boost::asio::buffer(size_buf), sender_endpoint);

        if (len < 4) {
            std::cerr << "Incomplete size data received" << std::endl;
            continue;
        }

        uint32_t size = *reinterpret_cast<uint32_t*>(size_buf.data());
        std::vector<unsigned char> frame_buf(size);

        size_t total_received = 0;
        while (total_received < size) {
            size_t remaining = size - total_received;
            size_t chunk_size = std::min(remaining, static_cast<size_t>(65536));

            total_received += socket.receive_from(boost::asio::buffer(&frame_buf[total_received], chunk_size), sender_endpoint);
        }

        Mat frame = imdecode(Mat(frame_buf), IMREAD_COLOR);
        if (!frame.empty()) {
            imshow("Video Stream", frame);
            if (waitKey(30) >= 0) break;
        }
    }
}
