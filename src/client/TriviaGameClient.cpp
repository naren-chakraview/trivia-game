#include <iostream>

#include <boost/asio.hpp>
#include <boost/array.hpp>

#include "TriviaGameConfig.h"

using namespace boost::asio;
using namespace std;

int main(int argc, char* argv[])
{
    try
    {
        if (argc != 3) {
          std::cerr << "Usage: TriviaGameClient <host> <port>" << std::endl;
          return 1;
        }

        io_context ioContext;

        ip::tcp::resolver resolver(ioContext);
        ip::tcp::resolver::results_type endpoints = resolver.resolve(argv[1], argv[2]);

        ip::tcp::socket socket(ioContext);
        connect(socket, endpoints);

        while (true) {
            boost::array<char, 128> buf;
            boost::system::error_code error;

            size_t len = socket.read_some(boost::asio::buffer(buf), error);

            if (error == boost::asio::error::eof)
            break; // Connection closed cleanly by peer.
            else if (error)
            throw boost::system::system_error(error); // Some other error.

            std::cout.write(buf.data(), len);
        }
    }
    catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}
