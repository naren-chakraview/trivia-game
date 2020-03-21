#include <boost/shared_ptr.hpp>

#include "catch.hpp"

#include "RoomManager.hpp"


SCENARIO("Room manager is initialized") {
    GIVEN("A Room Manager with some capacity settings") {
        boost::shared_ptr<RoomManager> rm = RoomManager::create(10, 30);

        REQUIRE(rm->capacity() == 10);
        REQUIRE(rm->waitTime() == 30);

        WHEN("Try to create a new room manager with other settings") {
            auto newRM = RoomManager::create(300, 45);

            THEN("The old settings still hold") {
                REQUIRE(newRM->capacity() == 10);
                REQUIRE(newRM->waitTime() == 30);
                REQUIRE(newRM == rm);
            }
        }
    }
};
