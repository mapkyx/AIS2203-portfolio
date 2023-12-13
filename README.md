# AIS2203-portfolio
AIS2203 Real-Time Cyber-Physical Systems Portfolio

## Description

This project consists of a C++ application designed to remotely control a Sphero RVR using UDP communication. The application interfaces with a Raspberry Pi Zero to send control commands and receive video and state information from the RVR. It leverages Boost for networking, OpenCV for video processing, and SDL2 for keyboard event handling.

## Features

1.Real-time video streaming from the RVR.

2.Keyboard-based control for the RVR's movements.

3.Display of RVR's current state, including battery level, last command/direction, and mode (manual/autonomous).

4.UDP-based communication for robust and fast data exchange.

## Installation

Prerequisites

-Boost: Used for UDP networking.

-OpenCV: For video processing and displaying.

-SDL2: For handling keyboard inputs.

-CMake (Optional): For building the project.

## Installing and Running

Clone the repository and navigate into the project directory:

```bash
git clone [repository-url]
cd [repository-name]
```

Compile the project (example using g++):

```bash
g++ -o rvr-project main.cpp UDPClient.cpp CameraFeed.cpp DisplayRvrState.cpp UDPListener.cpp KeyboardListener.cpp -lboost_system -lopencv_core -lopencv_highgui -lopencv_imgproc -lSDL2
```

Run the application:

```bash
./rvr-project
```

## Usage
After launching the application, use the following keyboard keys to control the rover:

-W, A, S, D for movement.

-1 for autonomous control.

-0 for manual control.

-C to stop and exit the application.

## Authors

Myklebust, Åsmund

Lyngstad, Markus

Lervåg, Johannes





