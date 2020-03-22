#include <boost/shared_ptr.hpp>
#include <boost/smart_ptr/make_shared_array.hpp>
#include <boost/smart_ptr/shared_ptr.hpp>

#include "RoomManager.hpp"

boost::shared_ptr<RoomManager> RoomManager::manager;

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
        mRoom = boost::make_shared<Room>(Room());

        // start timer on room
    }
}

void RoomManager::addUser(User& user) {
    createNewRoomIfNeeded();

    mRoom->addUser(user);

    if (mRoom->size() >= capacity()) {
        mRoom->activate();
        mActiveRooms.push_back(mRoom);
        mRoom = nullptr;
        cleanupRooms();
    }
}

