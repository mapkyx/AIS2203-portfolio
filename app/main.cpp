
#include <thread>
#include <iostream>
#include <atomic>

std::atomic<bool> isRunning(true);
SharedData sharedData;
std::mutex sharedDataMutex;

int main(int argc, char* argv[]) {
    try {
        boost::asio::io_service io_service;
        UDPClient client(io_service, "192.168.129.134", 1234);  // IP address of Raspberry Pi

        std::cout << "Connected to Raspberry Pi via UDP." << std::endl;
        std::cout << "Press W, A, S, D to control. Press Q to quit." << std::endl;

        std::thread keyboard_thread(keyboard_listener, std::ref(client));
        std::thread listener_thread(udp_listener);
        std::thread camera_thread(camera_feed_receiver, std::ref(io_service), "192.168.3.138", 1234);  // Different port for video

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