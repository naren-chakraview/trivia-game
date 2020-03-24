#include <vector>
#include <set>
#include <string>
#include <sstream>
#include <algorithm>
#include <functional>
#include <chrono>
#include <thread>

#include <boost/shared_ptr.hpp>

#include "TriviaGameConfig.h"
#include "GameManager.hpp"
#include "Room.hpp"
#include "TriviaDB.hpp"

using namespace std;

void GameManager::wait(int secs) {
	std::this_thread::sleep_for(std::chrono::seconds(secs));
}

void GameManager::broadcastMessage(vector<boost::shared_ptr<User>>& users, string message) {
    for (auto user : users) {
        user->sendMessage(message);
    }
}

string GameManager::getIntroduction() {
    stringstream message;
    message << "The game is about to begin. There are a maximum of "
        << MAX_ROUNDS << ", each with " << MAX_QUESTIONS
        << ". You have " << MAX_TIME_PER_QUESTION << " seconds to answer."
        << endl;
    for (int i = 0; i < MAX_ROUNDS; i++) {
        message << "At the end of round " << ROUND_NUMBERS[i]
            << " there might be a maximum of " << ROUND_WINNERS[i]
            << "." << endl;
    }

    message << "Additionally, if there is a clear winner at the end of any round, "
        << "the game will end immediately." << endl << endl
        << "May the force be with you!" << endl;

    return message.str();
}

void GameManager::harvestAnswersAndUpdateStatistics(vector<boost::shared_ptr<User>>& users,
        TriviaDB::Question& question, int questionNumber) {
    int correct = 0;
    for (auto user : users) {
        string answer = user->readMessage();

        if (answer == question.answer) {
            mUserScores[user->name()]++;
            correct++;
        }
    }
    mCorrectAnswerPerQuestion[questionNumber] = correct;
}

void GameManager::executeRound(vector<boost::shared_ptr<User>>& users, int round) {
    if (mUsersPerRound.size() == 0) return; // we have 0 players; stop

    stringstream message;
    message << "Round number " << ROUND_NUMBERS[round] << " begins..." << endl;
    broadcastMessage(users, message.str());
    wait(3);

    for (int i = 0; i < MAX_QUESTIONS; i++) {
        TriviaDB::Question question = TriviaDB::get()->getNextQuestion();
        broadcastMessage(users, question.question);
        wait(MAX_TIME_PER_QUESTION + 1);

        harvestAnswersAndUpdateStatistics(users, question, i);
    }
}

void GameManager::broadcastStatistics(vector<boost::shared_ptr<User>>& users, int round) {
    if (mUsersPerRound.size() == 0) return; // we have 0 players; stop

    stringstream stats;

    stats << "Statistics for the round " << ROUND_NUMBERS[round] << ": " << endl;
    stats << "Number of correct answers per question: " << endl;
    for (int i = 0; i < MAX_QUESTIONS; i++) {
        stats << "[ Q " << i+1 << ": " << mCorrectAnswerPerQuestion[i] << "] ";
    }
    stats << endl << "Number of correct answers for you: ";
    for (auto user : mUsersPerRound) {
        stringstream statsMessage;
        statsMessage << stats.str() << mUserScores[user.first] << endl;
        user.second->sendMessage(statsMessage.str());
    }
}

void GameManager::deactivateUsersWhoLost(vector<boost::shared_ptr<User>>& users, int round) {
    if (mUsersPerRound.size() == 0) return; // we have 0 players; stop

    // function object for comparing by value
	typedef function<bool(pair<string, int>, pair<string, int>)> Comparator;

	// Defining a lambda function to compare two pairs.
    // It will compare two pairs using second field, which is the count in our case
	Comparator compare =
			[](pair<string, int> left, pair<string, int> right) {
				return left.second > right.second; // descending
			};

	set<pair<string, int>, Comparator> sortedScores(
            mUserScores.begin(), mUserScores.end(), compare);

    int idx = 0;
    for (auto iter = sortedScores.begin(); iter != sortedScores.end(); iter++, idx++) {
        string userName = iter->first;
        int score = iter->second;
        if (idx < ROUND_WINNERS[round] && score != 0) {
            mUsersPerRound[userName]->sendMessage("Congrats, you are a winner!");
        }
        else {
            mUsersPerRound[userName]->sendMessage("Sorry, you lost!\n");
            mUsersPerRound[userName]->sendMessage("Goodbye");
            mUsersPerRound[userName]->deactivate();
        }
    }
}

void GameManager::resetStats(vector<boost::shared_ptr<User>>& users) {
    // initialize per round stats
    for (auto user : users) {
        if (user->active()) {
            mUsersPerRound[user->name()] = user;
            mUserScores[user->name()] = 0;
        }
    }
    fill(mCorrectAnswerPerQuestion, mCorrectAnswerPerQuestion + MAX_QUESTIONS, 0);
}

void GameManager::startGame(vector<boost::shared_ptr<User>>& users, boost::shared_ptr<Room> room) {
    cout << "Starting game for " << users.size() << " players" << endl;

    broadcastMessage(users, getIntroduction());
    wait(3);

    resetStats(users);
    for (int round = 0; round < MAX_ROUNDS; round++) {
        executeRound(users, round);
        broadcastStatistics(users, round);
        deactivateUsersWhoLost(users, round);

        // reset per round statistics
        resetStats(users);
        if (mUsersPerRound.size() <= 1) break; // we either have 0 or 1 winner; done
    }

    room->deactivate(); // done with the room
}

