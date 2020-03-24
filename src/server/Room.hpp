#ifndef __TRIVIA_GAME_SERVER_ROOM__
#define __TRIVIA_GAME_SERVER_ROOM__

#include <iostream>
#include <vector>

#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>

#include "User.hpp"
#include "GameManager.hpp"

class Room : public boost::enable_shared_from_this<Room> {
public:
    static boost::shared_ptr<Room> create() {
        return boost::shared_ptr<Room>(new Room());
    }

    void addUser(boost::shared_ptr<User> user) {
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
        mGameManager->startGame(mUsers, shared_from_this());
    }

    void deactivate() {
        mActive = false;
    }

private:
    Room() : mActive(false) {
        mGameManager = GameManager::create();
    }

    bool mActive;

    std::vector<boost::shared_ptr<User>> mUsers;
    boost::shared_ptr<GameManager> mGameManager;
    boost::shared_ptr<Room> mThis;
};

#endif // __TRIVIA_GAME_SERVER_ROOM__
