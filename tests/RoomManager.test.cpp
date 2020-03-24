#include <chrono>
#include <thread>

#include <boost/shared_ptr.hpp>
#include <boost/smart_ptr/shared_ptr.hpp>

#include "catch.hpp"

#include "RoomManager.hpp"

using namespace std::chrono;

SCENARIO("Room manager is initialized") {
    GIVEN("A Room Manager with some capacity settings") {
        boost::shared_ptr<RoomManager> rm = RoomManager::get(2, 3);

        REQUIRE(rm->capacity() == 2);
        REQUIRE(rm->waitTime() == 3);
        REQUIRE(rm->roomCount() == 0);
        REQUIRE(rm->activeRoomCount() == 0);

        WHEN("Try to create a new room manager with other settings") {
            auto newRM = RoomManager::get(300, 45);

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
        boost::shared_ptr<RoomManager> rm = RoomManager::get(2, 30);

        REQUIRE(rm->capacity() == 2);

        WHEN("Adding users within capacity") {
            boost::shared_ptr<User> u1 = User::create("u1", nullptr);
            rm->addUser(u1);

            THEN("Room count goes up, but active rooms remains same") {
                REQUIRE(rm->roomCount() == 1);
                REQUIRE(rm->activeRoomCount() == 0);
            }
        }

        WHEN("Adding users at capacity") {
            boost::shared_ptr<User> u2 = User::create("u2", nullptr);
            rm->addUser(u2);

            THEN("Room count goes up, and active room count goes up") {
                REQUIRE(rm->roomCount() == 1);
                REQUIRE(rm->activeRoomCount() == 1);
            }
        }

        WHEN("Adding users more than capacity") {
            boost::shared_ptr<User> u3 = User::create("u3", nullptr);
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
        boost::shared_ptr<RoomManager> rm = RoomManager::get(2, 3);

        REQUIRE(rm->capacity() == 2);
        REQUIRE(rm->waitTime() == 3);

        WHEN("Adding users within capacity and without exceeding max wait") {
            boost::shared_ptr<User> u1 = User::create("u1", nullptr);
            rm->addUser(u1);

            THEN("Room count goes up, but active rooms remains same") {
                REQUIRE(rm->roomCount() == 2);
                REQUIRE(rm->activeRoomCount() == 2);
            }
        }

        WHEN("Adding users after than capacity") {
            boost::shared_ptr<User> u2 = User::create("u2", nullptr);
            boost::shared_ptr<User> u3 = User::create("u3", nullptr);
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

SCENARIO("Room manager cleaning up") {
    GIVEN("A few rooms in various states of activity, clean up") {
    }
}
