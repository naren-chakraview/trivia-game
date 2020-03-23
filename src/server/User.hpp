#ifndef __TRIVIA_GAME_SERVER_USER__
#define __TRIVIA_GAME_SERVER_USER__

#include <iostream>

#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>

#include "Listener.hpp"

using namespace std;

class User : public enable_shared_from_this<User> {
public:
    static boost::shared_ptr<User> create(string name,
            boost::shared_ptr<Listener::Connection> connection) {
        return boost::shared_ptr<User>(new User(name, connection));
    }

private:
    User(string name, boost::shared_ptr<Listener::Connection> connection)
        : mName(name), mConnection(connection) {}

    string mName;
    boost::shared_ptr<Listener::Connection> mConnection;
};

#endif // __TRIVIA_GAME_SERVER_USER__
