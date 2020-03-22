#include "catch.hpp"

#include "User.hpp"
#include "Room.hpp"

SCENARIO("Basic Room activities") {
    GIVEN("A new and empty Room") {
        Room rm;

        REQUIRE(rm.active() == false);
        REQUIRE(rm.size() == 0);

        WHEN("A new user is added") {
            User user;
            rm.addUser(user);

            THEN("Size changes but not activeness") {
                REQUIRE(rm.size() == 1);
                REQUIRE(rm.active() == false);
            }
        }

        WHEN("Multiple users are added") {
            User u1, u2, u3;
            rm.addUser(u1);
            rm.addUser(u2);
            rm.addUser(u3);

            THEN("Size changes by number of users, but remains inactive") {
                REQUIRE(rm.size() == 3);
                REQUIRE(rm.active() == false);
            }
        }

        WHEN("Room is activated") {
            rm.activate();

            THEN("Room should be active") {
                REQUIRE(rm.active() == true);
            }
        }

        WHEN("Room is activated and deactivated") {
            rm.activate();
            rm.deactivate();

            THEN("Room should be inactive") {
                REQUIRE(rm.active() == false);
            }
        }

        WHEN("Room is active") {
            rm.activate();
            User u4;
            rm.addUser(u4);

            THEN("Room size should not change") {
                REQUIRE(rm.size() == 0);
            }
        }
    }
}
