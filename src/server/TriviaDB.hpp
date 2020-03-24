#ifndef __TRIVIA_GAME_SERVER_DB__
#define __TRIVIA_GAME_SERVER_DB__

#include <string>
#include <vector>

#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/make_shared.hpp>

#include "TriviaGameConfig.h"

using namespace std;

class TriviaDB : public boost::enable_shared_from_this<TriviaDB> {
public:
    struct Question {
        string question;
        string answer;
    };

    static boost::shared_ptr<TriviaDB> get() {
        if (!db) {
            db = boost::make_shared<TriviaDB>(TriviaDB());
        }
        return db;
    }

private:
    TriviaDB(string location = TRIVIA_DB_LOCATION) :
        mLocation(location) {}

    void loadDB();

    string mLocation;
    vector<Question> mQuestions;

    static boost::shared_ptr<TriviaDB> db;
};

#endif // __TRIVIA_GAME_SERVER_DB__
