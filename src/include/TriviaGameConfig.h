#ifndef __TRIVIA_GAME_SERVER_CONFIG__
#define __TRIVIA_GAME_SERVER_CONFIG__

// server config
const int GAME_SERVER_PORT = 12345;

// game room config
const int MAX_ROOM_CAPACITY = 50;
const int MAX_ROOM_WAIT = 30; // seconds

// game config
const int MAX_ROUNDS = 3;
const int MAX_QUESTIONS = 10;
const int MAX_TIME_PER_QUESTION = 10; // seconds

// trivia db config
const char TRIVIA_DB_LOCATION[] = "../db/triviadb.json";

#endif // __TRIVIA_GAME_SERVER_CONFIG__
