#include <iostream>
#include <thread>
#include <boost/asio.hpp>
#include <opencv2/opencv.hpp>
#include <SDL2/SDL.h>
#include <atomic>
#include <vector>
#include <mutex>

using namespace cv;

std::atomic<bool> isRunning(true);  // Global flag

std::mutex sharedDataMutex;

struct SharedData {
    std::string battery;
    std::string direction;
    std::string mode;
} sharedData;


class UDPClient {
public:
    UDPClient(boost::asio::io_service& io_service, const std::string& host, short port)
            : io_service_(io_service), socket_(io_service_), endpoint_(boost::asio::ip::address::from_string(host), port) {
        socket_.open(boost::asio::ip::udp::v4()); // Open the socket here
    }

    void send_command(const std::string& command) {
        boost::system::error_code ec;
        socket_.send_to(boost::asio::buffer(command), endpoint_, 0, ec);
        if (ec) {
            std::cerr << "Failed to send command: " << ec.message() << std::endl;
        }
    }

private:
    boost::asio::io_service& io_service_;
    boost::asio::ip::udp::socket socket_;
    boost::asio::ip::udp::endpoint endpoint_;
};

void camera_feed_receiver(boost::asio::io_service& io_service, const std::string& host, short port) {
    boost::asio::ip::udp::socket socket(io_service, boost::asio::ip::udp::endpoint(boost::asio::ip::udp::v4(), 1236));
    boost::asio::ip::udp::endpoint sender_endpoint;

    while (true) {
        // Receive the size of the frame
        std::array<char, 4> size_buf;
        size_t len = socket.receive_from(boost::asio::buffer(size_buf), sender_endpoint);

        if (len < 4) {
            std::cerr << "Incomplete size data received" << std::endl;
            continue;
        }

        uint32_t size = *reinterpret_cast<uint32_t*>(size_buf.data());
        std::vector<unsigned char> frame_buf(size);

        // Receive the frame in chunks
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

void displayRvrState(const std::string& battery, const std::string& lastCommand, const std::string& mode) {
    cv::Mat displayImg(200, 400, CV_8UC3, cv::Scalar(255, 255, 255)); // White background

    cv::putText(displayImg, "Battery: " + battery, cv::Point(10, 80), cv::FONT_HERSHEY_SIMPLEX, 0.7, cv::Scalar(0, 0, 0), 2);
    cv::putText(displayImg, "Last Command: " + lastCommand, cv::Point(10, 130), cv::FONT_HERSHEY_SIMPLEX, 0.7, cv::Scalar(0, 0, 0), 2);
    cv::putText(displayImg, "Mode: " + mode, cv::Point(10, 180), cv::FONT_HERSHEY_SIMPLEX, 0.7, cv::Scalar(0, 0, 0), 2);

    cv::imshow("RVR State", displayImg);
    cv::waitKey(1); // Refresh the display window
}




void udp_listener() {
    boost::asio::io_service io_service;
    boost::asio::ip::udp::socket socket(io_service, boost::asio::ip::udp::endpoint(boost::asio::ip::udp::v4(), 1235));
    std::array<char, 1024> recv_buf{};
    std::cout << "Listening for data on port 1235..." << std::endl;
    while (isRunning.load()) {
        boost::asio::ip::udp::endpoint remote_endpoint;
        boost::system::error_code error;
        size_t len = socket.receive_from(boost::asio::buffer(recv_buf), remote_endpoint, 0, error);


        std::string stateInfo(recv_buf.data(), len);

        // Parsing the stateInfo into battery, last command, and mode
        auto separator1 = stateInfo.find(',');
        auto separator2 = stateInfo.find(',', separator1 + 1);
        std::string batteryParsed = stateInfo.substr(0, separator1);
        std::string lastCommandParsed = stateInfo.substr(separator1 + 1, separator2 - separator1 - 1);
        std::string modeParsed = stateInfo.substr(separator2 + 1);

        // Lock the mutex to safely update the shared data
        {
            std::lock_guard<std::mutex> guard(sharedDataMutex);
            sharedData.battery = batteryParsed;
            sharedData.direction = lastCommandParsed;
            sharedData.mode = modeParsed;
        }

        if (!error || error == boost::asio::error::message_size) {

            std::string stateInfo(recv_buf.data(), len);

            //std::cout << "Received stateInfo: " << stateInfo << std::endl;  // Debug print

        } else {
            std::cerr << "Receive error: " << error.message() << std::endl;
        }
    }
}


void keyboard_listener(UDPClient& client) {
    SDL_Init(SDL_INIT_VIDEO); // Initialize SDL2

    SDL_Window* window = SDL_CreateWindow(
            "SDL2 Keyboard Events",
            SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
            640, 480,
            SDL_WINDOW_SHOWN
    );

    bool running = true;
    SDL_Event e;

    while (running && isRunning.load()) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                running = false;
            } else if (e.type == SDL_KEYDOWN) {
                switch (e.key.keysym.sym) {
                    case SDLK_w: client.send_command("start_forward"); break;
                    case SDLK_a: client.send_command("start_left"); break;
                    case SDLK_s: client.send_command("start_backward"); break;
                    case SDLK_d: client.send_command("start_right"); break;
                    case SDLK_1: client.send_command("autonomously-control"); break;
                    case SDLK_0: client.send_command("manual-control"); break;
                    case SDLK_c:
                        isRunning.store(false);  // Set the flag to false when 'C' is pressed
                        break;
                }
            } else if (e.type == SDL_KEYUP) {
                client.send_command("stop-right");
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
        }}

    SDL_DestroyWindow(window);
    SDL_Quit();
}


int main(int argc, char* argv[]) {
    try {
        boost::asio::io_service io_service;
        UDPClient client(io_service, "192.168.129.134", 1234);  // IP address of Raspberry Pi

        std::cout << "Connected to Raspberry Pi via UDP." << std::endl;
        std::cout << "Press W, A, S, D to control. Press Q to quit." << std::endl;

        // Start the keyboard listener in a separate thread
        std::thread keyboard_thread(keyboard_listener, std::ref(client));

        std::thread listener_thread(udp_listener);

        // Start the camera feed receiver in a separate thread
        std::thread camera_thread(camera_feed_receiver, std::ref(io_service), "192.168.3.138", 1234);  // Use a different port for video


        while (isRunning.load()) {
            std::string battery, lastCommand, mode;
            {
                std::lock_guard<std::mutex> guard(sharedDataMutex);
                battery = sharedData.battery;
                lastCommand = sharedData.direction;
                mode = sharedData.mode;
            }

            displayRvrState(battery, lastCommand, mode);

            if (cv::waitKey(30) >= 0) break;
        }


        keyboard_thread.join();
        camera_thread.join();
        listener_thread.join();

    } catch (std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }

    return 0;
}