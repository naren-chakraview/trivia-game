#include <iostream>
#include <memory>

#include <boost/asio.hpp>
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

        void processClient();

    private:
        Connection(io_service& ioService) : mSocket(ioService) {
        }

        void sayHello(const boost::system::error_code&, size_t) {
        }

        ip::tcp::socket mSocket;
    };

    void listen();

    Listener(int port) :mPort(port) {};

private:
    void startAccept();
    void acceptClient(boost::shared_ptr<Connection> connection, boost::system::error_code error);

    unique_ptr<ip::tcp::acceptor> mAcceptor; 
    unique_ptr<ip::tcp::endpoint> mEndpoint;
    unique_ptr<io_service> mService;
    int mPort;
};
