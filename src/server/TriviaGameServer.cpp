#include <iostream>
#include "TriviaGameConfig.h"
#include "Listener.hpp"

using namespace std;

int main() {
  cout << "Starting Trivia Game server..." << endl;

  try {
      boost::asio::io_service ioService;

      Listener listener(ioService, GAME_SERVER_PORT);
      listener.listen();

      ioService.run();
  }
  catch (exception& e) {
      cerr << e.what() << endl;
  }

  return 0;
}
