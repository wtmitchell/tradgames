//===------------------------------------------------------------*- C++ -*-===//
///
/// \file
/// \brief Creates a basic agent that plays moves randomly
///
//===----------------------------------------------------------------------===//
#ifndef COMMON_RANDOMPLAYER_H_INCLUDED
#define COMMON_RANDOMPLAYER_H_INCLUDED

#include <iostream>
#include <random>
#include <string>
#include <vector>

#include "Common/Client.h"
#include "Common/String.h"

namespace Common {
template <typename GameState, typename GameClient>
class Random {
public:
  Random(std::string &name);
  ~Random() = default;
  // Don't allow copies for simplicity (the functions below are for the rule of 5)
  // copy ctor
  Random(const Random &) = delete;
  // move ctor
  Random(const Random &&) = delete;
  // copy assignment
  Random &operator=(const Random &) = delete;
  // move assignment
  Random &operator=(const Random &&) = delete;

  void playGame();

private:
  typedef typename GameClient::Move Move;
  void waitForStart();
  void switchCurrentPlayer();
  Move nextMove();
  void printAndRecvEcho(std::string msg) const;

  enum Players { player1, player2 };

  std::string myName;
  std::string oppName;
  Players currentPlayer;
  Players me;
  GameState gs;
  std::random_device rd;
  std::mt19937 mt;
};
} // namespace Common

// Implementation
//------------------------------------------------------------------------------
namespace Common {
template <typename GameState, typename GameClient>
Random<GameState, GameClient>::Random(std::string &name) : myName(name), rd(), mt(rd()) {}

template <typename GameState, typename GameClient>
void Random<GameState, GameClient>::playGame() {
  // Identify myself
  std::cout << "#name " << myName << std::endl;

  // Wait for start of game
  waitForStart();

  // Main game loop
  for (;;) {
    if (currentPlayer == me) {
      // My turn
      if (gs.gameOver()) {
        std::cerr << "By looking at the board, I know that I, " << myName
                  << ", have lost." << std::endl;
        switchCurrentPlayer();
        continue;
      }
      // Determine next move
      auto m = nextMove();

      // Double check it is valid
      if (!gs.isValidMove(m)) {
        std::cerr << "I was about to play an invalid move: "
                  << m << std::endl;
        std::cout << "#quit" << std::endl;
      }

      // Apply it
      gs.applyMove(m);

      if (m.isNull()) {
        // Concede to the server so we know what is going on
        std::cout << "# I, " << myName << ", have no moves to play." << std::endl;

        switchCurrentPlayer();
        // End game locally, server should detect and send #quit
        continue;
      }

      // Tell the world
      printAndRecvEcho(GameClient::moveMessage(m));

      // It is the opponents turn
      switchCurrentPlayer();
    } else {
      // Wait for move from other player
      // Get server's next instruction
      std::string serverMsg = Common::readMsg();
      std::vector<std::string> tokens = Common::split(serverMsg);

      if (GameClient::isValidMoveMessage(tokens)) {
        // Translate to local coordinates
        auto m = gs.translateToLocal(tokens);

        // Double check it is valid
        if (!gs.isValidMove(m)) {
          std::cerr << "Received move from opponent I think is invalid: " << m << std::endl;
          std::cout << "#quit" << std::endl;
        }

        // Apply the move and continue
        gs.applyMove(m);

        // It is now my turn
        switchCurrentPlayer();
      } else if (tokens.size() == 4 && tokens[0] == "FINAL" &&
                 tokens[2] == "BEATS") {
        // Game over
        if (tokens[1] == myName && tokens[3] == oppName)
          std::cerr << "I, " << myName << ", have won!" << std::endl;
        else if (tokens[3] == myName && tokens[1] == oppName)
          std::cerr << "I, " << myName << ", have lost." << std::endl;
        else
          std::cerr << "Did not find expected players in FINAL command.\n"
                    << "Found '" << tokens[1] << "' and '" << tokens[3] << "'. "
                    << "Expected '" << myName << "' and '" << oppName << "'.\n"
                    << "Received message '" << serverMsg << "'" << std::endl;
      } else {
        // Unknown command
        std::cerr << "Unknown command of '" << serverMsg << "' from the server";
      }
    }
  }
  std::cerr << "Quiting" << std::endl;
}

template <typename GameState, typename GameClient>
void Random<GameState, GameClient>::waitForStart() {
  for (;;) {
    std::string response = Common::readMsg();
    std::vector<std::string> tokens = Common::split(response);

    if (GameClient::isValidStartGameMessage(tokens)) {
      // Found BEGIN GAME message, determine if we play first
      if (tokens[2] == myName) {
        // We go first!
        oppName = tokens[3];
        me = player1;
        break;
      } else if (tokens[3] == myName) {
        // They go first
        oppName = tokens[2];
        me = player2;
        break;
      } else {
        std::cerr << "Did not find '" << myName
                  << "', my name, in the BEGIN command.\n"
                  << "# Found '" << tokens[2] << "' and '" << tokens[3] << "'"
                  << " as player names. Received message '" << response << "'";
        std::cout << "#quit";
        std::exit(EXIT_FAILURE);
      }
    } else if (response == "DUMPSTATE") {
      std::cout << gs.dumpState() << std::endl;
    } else if (tokens[0] == "LOADSTATE") {
      std::string newState = response.substr(10);
      if (!gs.loadState(newState))
        std::cerr << "Failed to load '" << newState << "'\n";
    } else if (response == "LISTMOVES") {
      std::vector<typename GameClient::Move> moves;
      gs.getMoves(moves);
      for (const auto i : moves)
        std::cout << i.from << ", " << i.to << "; ";
      std::cout << std::endl;
    } else if (tokens[0] == "MOVE") {
      // Just apply the move
      Move m;
      m.from = static_cast<unsigned>(std::stoi(tokens[2]));
      m.to = static_cast<unsigned>(std::stoi(tokens[5]));
      gs.applyMove(m);
    } else {
      std::cerr << "Unexpected message " << response << "\n";
    }
  }

  // Game is about to begin, restore to start state in case DUMPSTATE/LOADSTATE/LISTMOVES
  // were used
  gs.reset();
  
  // Player 1 goes first
  currentPlayer = player1;
}

template <typename GameState, typename GameClient>
void Random<GameState, GameClient>::switchCurrentPlayer() {
  currentPlayer = (currentPlayer == player1) ? player2 : player1;
}

template <typename GameState, typename GameClient>
typename GameClient::Move Random<GameState, GameClient>::nextMove() {
  std::vector<typename GameClient::Move> moves;
  gs.getMoves(moves);

  std::uniform_int_distribution<> dis(0, int(moves.size() - 1));

  return moves[size_t(dis(mt))];
}

template <typename GameState, typename GameClient>
void Random<GameState, GameClient>::printAndRecvEcho(std::string msg) const {
   // Note the endl flushes the stream, which is necessary
  std::cout << msg << std::endl;
  const std::string echo = Common::readMsg();
  if (msg != echo)
    std::cerr << "Expected echo of '" << msg << "'. Received '" << echo << "'"
              << std::endl;
}

} // namespace Common

#endif
