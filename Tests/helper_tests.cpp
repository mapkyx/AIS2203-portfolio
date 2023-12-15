#include <catch2/catch_test_macros.hpp>
<<<<<<< Updated upstream:Tests/helper_tests.cpp
#include <Include/network_helper.hpp>
=======
>>>>>>> Stashed changes:tests/helper_tests.cpp

#include "network_helper.hpp"

TEST_CASE("Test network helper [Big endian]") {

    for (int i = -100; i < 100; i++) {
        auto bytes = int_to_bytes(i, byte_order::BIG);
        CHECK(i == bytes_to_int(bytes, byte_order::BIG));
    }
}

TEST_CASE("Test network helper [Little endian]") {

    for (int i = -100; i < 100; i++) {
        auto bytes = int_to_bytes(i, byte_order::LITTLE);
        CHECK(i == bytes_to_int(bytes, byte_order::LITTLE));
    }
}

