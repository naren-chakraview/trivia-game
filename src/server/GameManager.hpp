#ifndef __TRIVIA_GAME_SERVER_GAME_MANAGER__
#define __TRIVIA_GAME_SERVER_GAME_MANAGER__

#include <vector>
#include <map>

#include <boost/shared_ptr.hpp>

#include "TriviaGameConfig.h"
#include "User.hpp"
#include "TriviaDB.hpp"

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

    void wait(int seconds);
    std::string getIntroduction();
    void broadcastMessage(std::vector<boost::shared_ptr<User>>& users, std::string message);
    void harvestAnswersAndUpdateStatistics(std::vector<boost::shared_ptr<User>>& users,
            TriviaDB::Question& question, int questionNumber);
    void executeRound(std::vector<boost::shared_ptr<User>>& users, int round);
    void broadcastStatistics(std::vector<boost::shared_ptr<User>>& users, int round);
    void deactivateUsersWhoLost(std::vector<boost::shared_ptr<User>>& users, int round);
    void resetStats(std::vector<boost::shared_ptr<User>>& users);

    int mMaxRounds;
    int mMaxQuestionsPerRound;
    int mMaxTimePerQuestion; //seconds

    // per round statistics; gets reset every round
    std::map<std::string, boost::shared_ptr<User>> mUsersPerRound;
    int mCorrectAnswerPerQuestion[MAX_QUESTIONS];
    std::map<std::string, int> mUserScores;
};

#endif // __TRIVIA_GAME_SERVER_GAME_MANAGER__
