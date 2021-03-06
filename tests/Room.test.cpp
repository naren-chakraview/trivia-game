#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>
#include <boost/smart_ptr/shared_ptr.hpp>

#include "catch.hpp"

#include "User.hpp"
#include "Room.hpp"

SCENARIO("Basic Room activities") {
    GIVEN("A new and empty Room") {
        boost::shared_ptr<Room> rm = Room::create();

        REQUIRE(rm->active() == false);
        REQUIRE(rm->size() == 0);

        WHEN("A new user is added") {
            boost::shared_ptr<User> user = User::create("user", nullptr);
            rm->addUser(user);

            THEN("Size changes but not activeness") {
                REQUIRE(rm->size() == 1);
                REQUIRE(rm->active() == false);
            }
        }

        WHEN("Multiple users are added") {
            boost::shared_ptr<User> u1 = User::create("u1", nullptr);
            boost::shared_ptr<User> u2 = User::create("u3", nullptr);
            boost::shared_ptr<User> u3 = User::create("u3", nullptr);
            rm->addUser(u1);
            rm->addUser(u2);
            rm->addUser(u3);

            THEN("Size changes by number of users, but remains inactive") {
                REQUIRE(rm->size() == 3);
                REQUIRE(rm->active() == false);
            }
        }

        WHEN("Room is activated") {
            rm->activate();

            THEN("Room should be active") {
                REQUIRE(rm->active() == true);
            }
        }

        WHEN("Room is activated and deactivated") {
            rm->activate();
            rm->deactivate();

            THEN("Room should be inactive") {
                REQUIRE(rm->active() == false);
            }
        }

        WHEN("Room is active") {
            rm->activate();
            boost::shared_ptr<User> u4 = User::create("u4", nullptr);
            rm->addUser(u4);

            THEN("Room size should not change") {
                REQUIRE(rm->size() == 0);
            }
        }
    }
}
