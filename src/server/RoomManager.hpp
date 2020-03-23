#ifndef __TRIVIA_GAME_SERVER_ROOM_MANAGER__
#define __TRIVIA_GAME_SERVER_ROOM_MANAGER__

#include <iostream>
#include <list>

#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/asio.hpp>
#include <boost/asio/deadline_timer.hpp>
#include <boost/bind.hpp>

#include "Room.hpp"
#include "User.hpp"

using namespace std;

class RoomManager : public boost::enable_shared_from_this<RoomManager> {
public:
    static boost::shared_ptr<RoomManager> create(int maxCapacity, int maxWait) {
        if (!manager) {
            manager = boost::make_shared<RoomManager>(RoomManager(maxCapacity, maxWait));
        }
        return manager;
    }

    static boost::shared_ptr<RoomManager> get() {
        return manager;
    }

    int capacity() { return mCapacity; }

    int waitTime() { return mWait; }

    void cleanupRooms();

    void createNewRoomIfNeeded();

    void activateRoom();

    void addUser(User& user);

    int roomCount() {
        return mActiveRooms.size() + (mRoom == nullptr ? 0 : 1);
    }

    int activeRoomCount() {
        int count = 0;
        for (auto room : mActiveRooms) {
            count += (room->active() == true) ? 1 : 0;
        }
        return count;
    }

private:
    RoomManager(int maxCapacity, int maxWait) : 
        mCapacity(maxCapacity),
        mWait(maxWait),
        mTimer(RoomManager::service) {}

    int mCapacity;
    int mWait;
    
    boost::shared_ptr<Room> mRoom;
    list<boost::shared_ptr<Room>> mActiveRooms;
    boost::asio::deadline_timer mTimer;

    static boost::shared_ptr<RoomManager> manager;
    static boost::asio::io_service service;
};

#endif // __TRIVIA_GAME_SERVER_ROOM_MANAGER__
