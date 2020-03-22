#include <boost/shared_ptr.hpp>
#include <boost/smart_ptr/shared_ptr.hpp>

#include "catch.hpp"

#include "RoomManager.hpp"


SCENARIO("Room manager is initialized") {
    GIVEN("A Room Manager with some capacity settings") {
        boost::shared_ptr<RoomManager> rm = RoomManager::create(2, 30);

        REQUIRE(rm->capacity() == 2);
        REQUIRE(rm->waitTime() == 30);
        REQUIRE(rm->roomCount() == 0);
        REQUIRE(rm->activeRoomCount() == 0);

        WHEN("Try to create a new room manager with other settings") {
            auto newRM = RoomManager::create(300, 45);

            THEN("The old settings still hold") {
                REQUIRE(newRM->capacity() == 2);
                REQUIRE(newRM->waitTime() == 30);
                REQUIRE(newRM == rm);
            }
        }
    }
}

SCENARIO("Room manager room management") {
    GIVEN("A Room with fixed capacity") {
        boost::shared_ptr<RoomManager> rm = RoomManager::create(2, 30);

        REQUIRE(rm->capacity() == 2);

        WHEN("Adding users within capacity") {
            User u1;
            rm->addUser(u1);

            THEN("Room count goes up, but active rooms remains same") {
                REQUIRE(rm->roomCount() == 1);
                REQUIRE(rm->activeRoomCount() == 0);
            }
        }

        WHEN("Adding users at capacity") {
            User u2;
            rm->addUser(u2);

            THEN("Room count goes up, and active room count goes up") {
                REQUIRE(rm->roomCount() == 1);
                REQUIRE(rm->activeRoomCount() == 1);
            }
        }

        WHEN("Adding users more than capacity") {
            User u3;
            rm->addUser(u3);

            THEN("Room count goes up, but active rooms remains same") {
                REQUIRE(rm->roomCount() == 2);
                REQUIRE(rm->activeRoomCount() == 1);
            }
        }
    }
};
