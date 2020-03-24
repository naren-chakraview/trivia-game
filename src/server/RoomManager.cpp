#include <boost/asio/deadline_timer.hpp>
#include <boost/date_time/posix_time/posix_time_duration.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>

#include "RoomManager.hpp"

boost::shared_ptr<RoomManager> RoomManager::manager;

void RoomManager::cleanupRooms() {
    int count = 0;
    for (auto room = mActiveRooms.begin(); room != mActiveRooms.end();) {
        if (!(*room)->active()) {
            room = mActiveRooms.erase(room);
            count++;
        }
        else {
            ++room;
        }
    }
    cout << "Cleaned up " << count << " rooms." << endl;
}

void RoomManager::createNewRoomIfNeeded(io_service& service) {
    if (!mRoom) {
        cout << "Creating new game room and waiting for " << mWait << " seconds." << endl;
        mRoom = Room::create();

        // start timer on room
        if (!mTimer) {
            mTimer = boost::shared_ptr<boost::asio::deadline_timer>(new boost::asio::deadline_timer(service));
        }
        mTimer->expires_from_now(boost::posix_time::seconds(mWait));
        mTimer->async_wait(boost::bind(&RoomManager::activateRoom, shared_from_this()));
        cout << "Started timer..." << endl;
    }
}

void RoomManager::activateRoom() {
    if (mRoom->size() == 0) {
        mTimer->expires_from_now(boost::posix_time::seconds(mWait));
        mTimer->async_wait(boost::bind(&RoomManager::activateRoom, shared_from_this()));
        cout << "No users yet. Restarted " << mWait << " second timer..." << endl;

        return;
    }
    cout << "Activating game room" << endl;
    mRoom->activate();
    mActiveRooms.push_back(mRoom);
    mRoom = nullptr;
    cleanupRooms();
}

void RoomManager::addUser(boost::shared_ptr<User> user) {
    createNewRoomIfNeeded(user->connection()->socket().get_io_service());

    mRoom->addUser(user);

    if (mRoom->size() >= capacity()) {
        mRoom->activate();
        mActiveRooms.push_back(mRoom);
        mRoom = nullptr;
        cleanupRooms();
    }
}

