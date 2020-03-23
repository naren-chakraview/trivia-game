#include <chrono>

#include <boost/shared_ptr.hpp>
#include <boost/smart_ptr/shared_ptr.hpp>

#include "catch.hpp"

#include "RoomManager.hpp"

using namespace std::chrono;

SCENARIO("Room manager is initialized") {
    GIVEN("A Room Manager with some capacity settings") {
        boost::shared_ptr<RoomManager> rm = RoomManager::create(2, 3);

        REQUIRE(rm->capacity() == 2);
        REQUIRE(rm->waitTime() == 3);
        REQUIRE(rm->roomCount() == 0);
        REQUIRE(rm->activeRoomCount() == 0);

        WHEN("Try to create a new room manager with other settings") {
            auto newRM = RoomManager::create(300, 45);

            THEN("The old settings still hold") {
                REQUIRE(newRM->capacity() == 2);
                REQUIRE(newRM->waitTime() == 3);
                REQUIRE(newRM == rm);
            }
        }
    }
}

SCENARIO("Room manager room management w.r.t. capacity") {
    GIVEN("A Room with fixed small capacity") {
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

SCENARIO("Room manager room management w.r.t. wait time") {
    GIVEN("A Room with ample capacity and slow incoming clients") {
        boost::shared_ptr<RoomManager> rm = RoomManager::create(50, 3);

        REQUIRE(rm->capacity() == 2);
        REQUIRE(rm->waitTime() == 3);

        WHEN("Adding users within capacity and without exceeding max wait") {
            User u1;
            rm->addUser(u1);

            THEN("Room count goes up, but active rooms remains same") {
                REQUIRE(rm->roomCount() == 2);
                REQUIRE(rm->activeRoomCount() == 2);
            }
        }

        WHEN("Adding users after than capacity") {
            User u2, u3;
            rm->addUser(u2);

            std::this_thread::sleep_for(seconds(3));

            rm->addUser(u3);

            THEN("Room count goes up, and active rooms goes up") {
                REQUIRE(rm->roomCount() == 3);
                REQUIRE(rm->activeRoomCount() == 3);
            }
        }
    }
};
