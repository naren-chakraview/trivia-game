# trivia-game
---
Simple implementation of a game similar to HQ trivia game. The game is similar
in rules to the once popular [HQ Trivia game](https://en.wikipedia.org/wiki/HQ_(game))

## Requirements along with game rules
---
The following points capture requirements both in terms of game mechanics as
well as relevant game rules.

- The server starts up and waits for users to join.
- When users join, server will start a game under 2 conditions
  1. Room has enough users
  2. Users have waited a max time limit
- During game,
  1. Users have 10 seconds to answer
  2. Correct answers are tallied
  3. Top K users go the next round
  4. Remaining users are waved goodbye
  5. After a fixed set of rounds, the user(s) with the most correct answers win
- Multiple games can be in progress
- Users can join at any time

## Design
---
The design for the Trivia Game is described in the [docs](docs/README.md)

## Build instructions
---
The prerequsites for this project are a cmake aware C++ tool chain and boost
STL library of version >= 1.65.

```
$ mkdir build
$ cd build
$ cmake ..
$ make TriviaGameServer TriviaGameClient TriviaTests
$ make test
```

The test results are captured in JUnit style report (test_report.xml) in the
build folder.

## State of project
---
The game server does a good job of managing users, but it has a few remaining
issues. The main issues are -

1. The game client does not time out after 10 seconds for the questions. It
   has a blocking terminal read method.
2. The game server for some reason is getting all the user responses after the
   results have been computed and everyone marked as lost because of 0 scores.
3. The game server has some blocking synchronous code despite efforts to design
   a clean asynchronous execution without using a multi-threaded approach.
