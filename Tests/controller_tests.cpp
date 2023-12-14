#include <catch2/catch_test_macros.hpp>
<<<<<<< Updated upstream:Tests/controller_tests.cpp
#include <Src/Functions.cpp>
=======
#include "SpheroRVRController.hpp"
#include "SpheroRVRMessageHandler.hpp"
>>>>>>> Stashed changes:tests/controller_tests.cpp
#include <iostream>
#include <random>

TEST_CASE("Test Sphero RVR Controller forward") {
    SpheroRVRController controller;
    SpheroRVRMessageHandler handler(controller);

    std::string testCommand = "w";

    auto expected = "Command received: " + testCommand;
    auto actual = handler.handle_message(testCommand);

    CHECK(expected == actual);
}

TEST_CASE("Test Sphero RVR Controller backwards") {
    SpheroRVRController controller;
    SpheroRVRMessageHandler handler(controller);

    std::string testCommand = "s";

    auto expected = "Command received: " + testCommand;
    auto actual = handler.handle_message(testCommand);

    CHECK(expected == actual);
}

TEST_CASE("Test Sphero RVR Controller left") {
    SpheroRVRController controller;
    SpheroRVRMessageHandler handler(controller);

    std::string testCommand = "a";

    auto expected = "Command received: " + testCommand;
    auto actual = handler.handle_message(testCommand);

    CHECK(expected == actual);
}

TEST_CASE("Test Sphero RVR Controller right") {
    SpheroRVRController controller;
    SpheroRVRMessageHandler handler(controller);

    std::string testCommand = "d";

    auto expected = "Command received: " + testCommand;
    auto actual = handler.handle_message(testCommand);

    CHECK(expected == actual);
}

<<<<<<< Updated upstream:Tests/controller_tests.cpp

=======
TEST_CASE("Test Sphero RVR Controller fail") {
    SpheroRVRController controller;
    SpheroRVRMessageHandler handler(controller);

    std::string testCommand = "t";

    auto expected = "Command received: " + testCommand;
    auto actual = handler.handle_message(testCommand);

    CHECK(expected == actual);
}
>>>>>>> Stashed changes:tests/controller_tests.cpp
