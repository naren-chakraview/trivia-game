#ifndef __TRIVIA_GAME_SERVER_GAME_MANAGER__
#define __TRIVIA_GAME_SERVER_GAME_MANAGER__

#include <boost/smart_ptr/shared_ptr.hpp>
#include <vector>

#include <boost/shared_ptr.hpp>

#include "TriviaGameConfig.h"
#include "User.hpp"

using namespace std;

class Room; //forward decl

class GameManager {
public:
    static boost::shared_ptr<GameManager> create() {
        return boost::shared_ptr<GameManager>(new GameManager());
    }

    void startGame(vector<boost::shared_ptr<User>>& users, boost::shared_ptr<Room> room);

private:
    GameManager() :
        mMaxRounds(MAX_ROUNDS),
        mMaxQuestionsPerRound(MAX_QUESTIONS),
        mMaxTimePerQuestion(MAX_TIME_PER_QUESTION) {}

    int mMaxRounds;
    int mMaxQuestionsPerRound;
    int mMaxTimePerQuestion; //seconds
};

#endif // __TRIVIA_GAME_SERVER_GAME_MANAGER__
