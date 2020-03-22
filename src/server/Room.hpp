#ifndef __TRIVIA_GAME_SERVER_ROOM__
#define __TRIVIA_GAME_SERVER_ROOM__

#include <iostream>
#include <vector>

#include <boost/shared_ptr.hpp>

#include "User.hpp"

using namespace std;

class Room {
public:
    Room() : mActive(false) {}

    void addUser(User& user) {
        if (!active()) {
            mUsers.push_back(user);
        }
    }

    size_t size() {
        return mUsers.size();
    }

    bool active() {
        return mActive;
    }

    void activate() {
        mActive = true;
    }

    void deactivate() {
        mActive = false;
    }

private:
    bool mActive;

    vector<User> mUsers;
};

#endif // __TRIVIA_GAME_SERVER_ROOM__
