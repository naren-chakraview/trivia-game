#include <iostream>
#include <string>

#include <boost/array.hpp>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/system/error_code.hpp>

#include "Listener.hpp"
#include "RoomManager.hpp"
#include "User.hpp"

using namespace std;
using namespace boost::asio;

string Listener::Connection::readMessage() {
    return mAnswer;
}

void Listener::Connection::processClient() {
    boost::array<char, 255> buf;
    boost::system::error_code error;

    size_t len = mSocket.read_some(buffer(buf), error);
    if (error == error::eof) {
        return; // Connection closed by client; don't bother
    }
    else if (error) {
        throw boost::system::system_error(error); // Some other error.
    }

    string userName(buf.data());
    
    boost::shared_ptr<User> user = User::create(userName, shared_from_this());

    RoomManager::get()->addUser(user);

    cout << "Player " << userName << " joined. Added to waiting room."  << endl;

    string greeting = "Hello: Welcome, " + userName + "! The game will start shortly\n";

    sendMessage(greeting);
}

void Listener::Connection::sendMessage(string message) {
    async_write(mSocket, buffer(message),
        boost::bind(&Listener::Connection::handleWriteError, shared_from_this(),
          boost::asio::placeholders::error,
          boost::asio::placeholders::bytes_transferred));

    if (message.rfind("question:") == 0) {
        // sent out a question, so wait for a response
        async_read(mSocket, buffer(mBuf),
        boost::bind(&Listener::Connection::handleRead, shared_from_this(),
          boost::asio::placeholders::error,
          boost::asio::placeholders::bytes_transferred));
    }
}

void Listener::listen() {
    mEndpoint = make_unique<ip::tcp::endpoint>(ip::tcp::v4(), mPort);
    mAcceptor = make_unique<ip::tcp::acceptor>(mService, *mEndpoint);
    cout << "Listening on port " << mPort << "..." << endl;

    startAccept();
}

void Listener::startAccept() {
    boost::shared_ptr<Listener::Connection> connection =
        Listener::Connection::create(mService);

    mAcceptor->async_accept(connection->socket(),
        boost::bind(&Listener::acceptClient, this, connection,
          boost::asio::placeholders::error));
}

void Listener::acceptClient(boost::shared_ptr<Listener::Connection> connection,
        boost::system::error_code error) {
    if (!error) {
        connection->processClient();
        startAccept();
    }
}
