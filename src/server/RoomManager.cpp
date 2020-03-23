#include <boost/date_time/posix_time/posix_time_duration.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>

#include "RoomManager.hpp"

boost::shared_ptr<RoomManager> RoomManager::manager;
boost::asio::io_service RoomManager::service;

void RoomManager::cleanupRooms() {
    for (auto room = mActiveRooms.begin(); room != mActiveRooms.end();) {
        if (!(*room)->active()) {
            room = mActiveRooms.erase(room);
        }
        else {
            ++room;
        }
    }
}

void RoomManager::createNewRoomIfNeeded() {
    if (!mRoom) {
        mRoom = Room::create();

        // start timer on room
        mTimer.expires_from_now(boost::posix_time::seconds(mWait));
        mTimer.async_wait(boost::bind(&RoomManager::activateRoom, this));
    }
}

void RoomManager::activateRoom() {
    mRoom->activate();
    mActiveRooms.push_back(mRoom);
    mRoom = nullptr;
    cleanupRooms();
}

void RoomManager::addUser(boost::shared_ptr<User> user) {
    createNewRoomIfNeeded();

    mRoom->addUser(user);

    if (mRoom->size() >= capacity()) {
        mRoom->activate();
        mActiveRooms.push_back(mRoom);
        mRoom = nullptr;
        cleanupRooms();
    }
}

