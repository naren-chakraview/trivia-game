#ifndef __TRIVIA_GAME_SERVER_LISTENER__
#define __TRIVIA_GAME_SERVER_LISTENER__

#include <iostream>
#include <memory>

#include <boost/asio.hpp>
#include <boost/array.hpp>
#include <boost/system/error_code.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>

using namespace std;
using namespace boost::asio;

class Listener {
public:
    class Connection : public boost::enable_shared_from_this<Connection> {
    public:
        static boost::shared_ptr<Connection> create(io_service& ioService) {
            return boost::shared_ptr<Connection>(new Connection(ioService));
        }

        ip::tcp::socket& socket() {
            return mSocket;
        }
        
        string readMessage();

        void sendMessage(string message);

        void processClient();

    private:
        Connection(io_service& ioService) : mSocket(ioService) {
        }

        void handleWriteError(const boost::system::error_code&, size_t) {
        }

        void handleRead(const boost::system::error_code& error, size_t len) {
            mAnswer = "";
            if (!error) {
                mAnswer = string(mBuf.data(), len);
                cout << "got answer " << mAnswer << endl;
            }
        }

        ip::tcp::socket mSocket;
        boost::array<char, 1> mBuf; // 1 character answers expected!!
        std::string mAnswer;
    };

    void listen();

    Listener(boost::asio::io_service& service, int port)
        : mService(service), mPort(port) {};

private:
    void startAccept();
    void acceptClient(boost::shared_ptr<Connection> connection, boost::system::error_code error);

    unique_ptr<ip::tcp::acceptor> mAcceptor; 
    unique_ptr<ip::tcp::endpoint> mEndpoint;
    boost::asio::io_service& mService;
    int mPort;
};

#endif // __TRIVIA_GAME_SERVER_LISTENER__
