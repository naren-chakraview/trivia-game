#ifndef __TRIVIA_GAME_SERVER_USER__
#define __TRIVIA_GAME_SERVER_USER__

#include <boost/asio/io_service.hpp>
#include <iostream>
#include <string>

#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>

#include "Listener.hpp"

class User : public enable_shared_from_this<User> {
public:
    static boost::shared_ptr<User> create(std::string name,
            boost::shared_ptr<Listener::Connection> connection) {
        return boost::shared_ptr<User>(new User(name, connection));
    }

    std::string name() { return mName; }

    std::string readMessage();

    void sendMessage(std::string message);

    void deactivate() {
        // release connection to terminate user
        mConnection = nullptr;
    }

    bool active() {
        return (mConnection != nullptr);
    }

    boost::shared_ptr<Listener::Connection> connection() {
        return mConnection;
    }

private:
    User(string name, boost::shared_ptr<Listener::Connection> connection)
        : mName(name), mConnection(connection) {}

    std::string mName;
    boost::shared_ptr<Listener::Connection> mConnection;
};

#endif // __TRIVIA_GAME_SERVER_USER__
