#include <string>

#include "User.hpp"

using namespace std;

string User::readMessage() {
    if (mConnection != nullptr) {
        return mConnection->readMessage();
    }
    return string("");
}

void User::sendMessage(string message) {
    if (mConnection) {
        mConnection->sendMessage(message);
    }
    // no op if no connection
}
