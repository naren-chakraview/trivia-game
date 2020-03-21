#include <iostream>

#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/system/error_code.hpp>

#include "Listener.hpp"

using namespace std;
using namespace boost::asio;

void Listener::Connection::processClient() {
    async_write(mSocket, buffer("Hello Client"),
        boost::bind(&Listener::Connection::sayHello, shared_from_this(),
          boost::asio::placeholders::error,
          boost::asio::placeholders::bytes_transferred));
}

void Listener::listen() {
    mIoContext = make_unique<io_context>();
    mService = make_unique<io_service>();
    mEndpoint = make_unique<ip::tcp::endpoint>(ip::tcp::v4(), mPort);
    mAcceptor = make_unique<ip::tcp::acceptor>(*mService, *mEndpoint);
    cout << "Listening on port " << mPort << "..." << endl;

    startAccept();

    mService->run();
}

void Listener::startAccept() {
    boost::shared_ptr<Listener::Connection> connection =
        Listener::Connection::create(*mIoContext);
    mAcceptor->async_accept(connection->socket(),
        boost::bind(&Listener::acceptClient, this, connection,
          boost::asio::placeholders::error));

}

void Listener::acceptClient(boost::shared_ptr<Listener::Connection> connection,
        boost::system::error_code error) {
    cout << "Got a client... " << endl;
    if (!error) {
        connection->processClient();
        startAccept();
    }
}
