#include <iostream>
#include "TriviaGameConfig.h"
#include "Listener.hpp"

using namespace std;

int main() {
  cout << "Starting Trivia Game server..." << endl;

  try {
      Listener listener(GAME_SERVER_PORT);
      listener.listen();
  }
  catch (exception& e) {
      cerr << e.what() << endl;
  }

  return 0;
}
