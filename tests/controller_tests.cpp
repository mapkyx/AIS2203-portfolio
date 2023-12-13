#include <catch2/catch_test_macros.hpp>
#include <source/Functions.cpp>
#include <iostream>

TEST_CASE("Test Sphero RVR Controller forward") {
    SpheroRVRController controller;
    SpheroRVRMessageHandler handler(controller);

    std::string testCommand = "w";  // Example command for testing

    auto expected = "Command received: " + testCommand;
    auto actual = handler.handle_message(testCommand);

    CHECK(expected == actual);
}

TEST_CASE("Test Sphero RVR Controller backwards") {
    SpheroRVRController controller;
    SpheroRVRMessageHandler handler(controller);

    std::string testCommand = "s";  // Example command for testing

    auto expected = "Command received: " + testCommand;
    auto actual = handler.handle_message(testCommand);

    CHECK(expected == actual);
}

TEST_CASE("Test Sphero RVR Controller left") {
    SpheroRVRController controller;
    SpheroRVRMessageHandler handler(controller);

    std::string testCommand = "a";  // Example command for testing

    auto expected = "Command received: " + testCommand;
    auto actual = handler.handle_message(testCommand);

    CHECK(expected == actual);
}

TEST_CASE("Test Sphero RVR Controller right") {
    SpheroRVRController controller;
    SpheroRVRMessageHandler handler(controller);

    std::string testCommand = "d";  // Example command for testing

    auto expected = "Command received: " + testCommand;
    auto actual = handler.handle_message(testCommand);

    CHECK(expected == actual);
}