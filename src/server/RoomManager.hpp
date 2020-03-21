#include <boost/smart_ptr/shared_ptr.hpp>
#include <iostream>

#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>
#include <boost/enable_shared_from_this.hpp>

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

private:
    RoomManager(int maxCapacity, int maxWait) : 
        mCapacity(maxCapacity), mWait(maxWait) {}

    int mCapacity;
    int mWait;

    static boost::shared_ptr<RoomManager> manager;
};
