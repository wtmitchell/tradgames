//===------------------------------------------------------------*- C++ -*-===//
///
/// \file
/// \brief Creates a basic moderator for a game
///
//===----------------------------------------------------------------------===//
#ifndef COMMON_MODERATOR_H_INCLUDED
#define COMMON_MODERATOR_H_INCLUDED

#include <array>
#include <fstream>
#include <set>
#include <stdexcept>
#include <string>
#include <vector>

#include "Common/Client.h"
#include "Common/String.h"
#include "Common/Timer.h"

namespace Common {
template <typename GameState, typename GameClient>
class Moderator {
public:
  Moderator();
  ~Moderator();

  // Don't allow copies for simplicity (the functions below are for the rule of 5)
  // copy ctor
  Moderator(const Moderator &) = delete;
  // move ctor
  Moderator(const Moderator &&) = delete;
  // copy assignment
  Moderator &operator=(const Moderator &) = delete;
  // move assignment
  Moderator &operator=(const Moderator &&) = delete;

  void playGame(bool printBoard, bool quiet, double turnTimeLimit,
                 bool logGame);

private:
  void waitForStart() noexcept(false);

  // Open files for logging
  void setupLogging();

  void broadcast(const std::string &msg);
  void diagnostic(const std::string &msg);
  void final(unsigned winner, unsigned loser);

  GameState gs;
  std::set<std::string> echo;
  std::vector<std::string> names;
  std::array<std::string, 2> playerNames;
  std::array<unsigned, 2> playerIds;
  std::ofstream log;
  bool logging;
  int turnCount;
};
} // namespace Common

// Implementation
//------------------------------------------------------------------------------

namespace Common {
template <typename GameState, typename GameClient>
Moderator<GameState, GameClient>::Moderator()
    : logging(false) {}

template <typename GameState, typename GameClient>
Moderator<GameState, GameClient>::~Moderator() {
  if (log.is_open())
    log.close();
}

template <typename GameState, typename GameClient>
void Moderator<GameState, GameClient>::playGame(bool printBoard, bool quiet,
                                                double turnTimeLimit,
                                                bool logGame) {
  // Identify myself
  std::cout << "#name moderator\n"
            << "#master" << std::endl;

  try {
    waitForStart();
  } catch (std::logic_error e) {
    // Duplicate names can't be recovered from
    return;
  }

  // Set up logging
  if (logGame)
    setupLogging();

  // Setup the timer
  Common::Timer moveTimer;

  // Start game
  broadcast(GameClient::startGameMessage(playerNames[0], playerNames[1]));

  // start timer
  moveTimer.start();

  // Player 1 has turn 0
  unsigned turn = 0;
  turnCount = 0;

  // Main game loop
  for (;;) {
    // Read message
    std::string msg = Common::readMsg();
    std::vector<std::string> tokens = Common::split(msg);

    // Stop the timer
    moveTimer.stop();

    // Look for echoes
    std::set<std::string>::iterator it = echo.find(msg);
    if (it != echo.end()) {
      // We received an expected echo
      echo.erase(it);
      continue;
    }

    if (GameClient::isValidMoveMessage(playerIds[turn], tokens)) {
      ++turnCount;
      // Print out turn and time information
      std::stringstream timeMsg;
      timeMsg << "Turn " << turnCount << " by " << playerIds[turn] << ":"
              << playerNames[turn] << " took " << moveTimer;
      diagnostic(timeMsg.str());

      // Took too long
      if (moveTimer.seconds_elapsed() > turnTimeLimit) {
        std::stringstream forfeitMsg;
        forfeitMsg << "Too long. Exceeds time limit of " << turnTimeLimit
                   << " seconds. "
                   << "Took " << moveTimer.seconds_elapsed() << " seconds. "
                   << playerIds[turn] << ":" << playerNames[turn]
                   << " forfeits.";
        diagnostic(forfeitMsg.str());

        if (printBoard)
          std::cerr << gs.dumpState() << std::endl;

        final((turn + 1) % 2, turn);
        broadcast("#quit");
        continue;
      }

      // Received move from current player
      auto m = gs.translateToLocal(tokens);

      // Validate move
      if (!gs.isValidMove(m)) {
        // Display board if requested
        if (printBoard)
          std::cerr << gs.dumpState() << std::endl;
        std::stringstream invalidMsg;
        invalidMsg << "Invalid move: " << msg;
        diagnostic(invalidMsg.str());

        final((turn + 1) % 2, turn);
        broadcast("#quit");
        continue;
      }

      // Apply move and echo it
      gs.applyMove(m);
      broadcast(GameClient::moveMessage(m));

      // Start timer for next player's move
      moveTimer.start();

      // Display board if requested
      if (printBoard)
        std::cerr << gs.dumpState() << std::endl;

      // Check if game is over
      if (gs.gameOver()) {
        // Game was just won by last played move
        final(turn, (turn + 1) % 2);
        broadcast("#quit");
        continue;
      }

      // Alternate whose turn
      turn = (turn + 1) % 2;
    } else {
      if (!quiet)
        std::cerr << "Received unknown or out of turn message: " << msg << std::endl;
    }
  }
}


template <typename GameState, typename GameClient>
void Moderator<GameState, GameClient>::waitForStart() noexcept(false) {
  // Wait for the game to begin
  for (;;) {
    std::cout << "#players" << std::endl;
    std::string response = Common::readMsg();
    std::vector<std::string> tokens = Common::split(response);

    int players = std::stoi(tokens[1]);

    if (tokens.size() == 2 && tokens[0] == "#players" && players >= 3) {
      // Enough have joined. Get player names
      for (int i = 0; i < players; ++i) {
        std::cout << "#getname " << i << std::endl;
        std::string name = Common::readMsg();
        std::vector<std::string> nameTokens  = Common::split(name);
        if (nameTokens.size() == 3 && nameTokens[0] == "#getname" &&
            std::stoi(nameTokens[1]) == i) {
          names.push_back(nameTokens[2]);
        } else {
          std::cerr << "Did not received expected response '#getname " << i
               << " name'."
                    << " Received message '" << name << "'" << std::endl;
        }
      }
      break;
    }
  }

  // Determine player names
  unsigned i = 0;
  for (unsigned j = 0; j < names.size(); ++j) {
    if (names[j] != "moderator" && names[j] != "observer") {
      if (i > 1)
        std::cerr << "Too many clients connected, using only "
                  << playerNames[0] << " and " << playerNames[1] << std::endl;

      playerNames[i] = names[j];
      playerIds[i] = j;
      ++i;
    }
  }

  // Verify a game can begin
  if (playerNames[0] == playerNames[1]) {
    std::cerr << "Both players have duplicate names: " << playerNames[0]
              << std::endl;
    std::cout << "FINAL " << playerNames[0] << " beats " << playerNames[1]
              << std::endl;
    throw(std::logic_error("Duplicate names"));
  }
}


template <typename GameState, typename GameClient>
void Moderator<GameState, GameClient>::setupLogging() {
  std::string filename = playerNames[0] + "-vs-" + playerNames[1] + ".txt";
  log.open(filename.c_str());
  logging = true;
}

template <typename GameState, typename GameClient>
void Moderator<GameState, GameClient>::broadcast(const std::string &msg) {
  echo.insert(msg);
  if (logging)
    log << msg << std::endl;
  std::cout << msg << std::endl;
}

template <typename GameState, typename GameClient>
void Moderator<GameState, GameClient>::diagnostic(const std::string &msg) {
  if (logging)
    log << "# " << msg << std::endl;
  std::cerr << msg << std::endl;
}

template <typename GameState, typename GameClient>
void Moderator<GameState, GameClient>::final(unsigned winner, unsigned loser) {
  std::stringstream finalMsg;
  finalMsg << "FINAL " << playerNames[winner] << " BEATS "
           << playerNames[loser];
  broadcast(finalMsg.str());
  std::stringstream movecountMsg;
  movecountMsg << turnCount << " moves were played in total";
  diagnostic(movecountMsg.str());
}
} //namespace Common

#endif
