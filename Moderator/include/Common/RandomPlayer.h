//===------------------------------------------------------------*- C++ -*-===//
///
/// \file
/// \brief Creates a basic agent that plays moves randomly
///
//===----------------------------------------------------------------------===//
#ifndef COMMON_RANDOMPLAYER_H_INCLUDED
#define COMMON_RANDOMPLAYER_H_INCLUDED

#include <iostream>
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
  void waitForStart();

  std::string name;
  GameState gs;
};
} // namespace Common

// Implementation
//------------------------------------------------------------------------------
namespace Common {
template <typename GameState, typename GameClient>
Random<GameState, GameClient>::Random(std::string &name_) : name(name_) {}

template <typename GameState, typename GameClient>
void Random<GameState, GameClient>::playGame() {
  /*
  // Identify myself
  std::cout << "#name " << name << std::endl;

  // Wait for start of game
  waitForStart();

  // Main game loop
  for (;;) {
    if (current_player == my_player) {
      // My turn
      if (gs.gameOver()) {
        cerr << "By looking at the board, I know that I, " << name
             << ", have lost." << endl;
        current_player = (current_player == player1) ? player2 : player1;
        continue;
      }
      // Determine next move
      Move m = next_move();

      // Double check it is valid
      if (!gs->valid_move(m)) {
        cerr << "I was about to play an invalid move: "
             << gs->pretty_print_move(m) << endl;
        cout << "#quit" << endl;
      }

      // Apply it
      gs->apply_move(m);

      if (m.isNull()) {
        // Concede to the server so we know what is going on
        cout << "# I, " << name << ", have no moves to play." << endl;

        current_player = (current_player == player1) ? player2 : player1;
        // End game locally, server should detect and send #quit
        continue;
      }

      // Tell the world
      print_and_recv_echo(gs->pretty_print_move(m));

      // It is the opponents turn
      current_player = (current_player == player1) ? player2 : player1;
    } else {
      // Wait for move from other player
      // Get server's next instruction
      string server_msg;
      vector<string> tokens = Client::read_msg_and_tokenize(&server_msg);

      if (tokens.size() == 6 && tokens[0] == "MOVE") {
        // Translate to local coordinates
        Move m = gs->translate_to_local(tokens);

        // Apply the move and continue
        gs->apply_move(m);

        // It is now my turn
        current_player = (current_player == player1) ? player2 : player1;
      } else if (tokens.size() == 4 && tokens[0] == "FINAL" &&
                 tokens[2] == "BEATS") {
        // Game over
        if (tokens[1] == name && tokens[3] == opp_name)
          cerr << "I, " << name << ", have won!" << endl;
        else if (tokens[3] == name && tokens[1] == opp_name)
          cerr << "I, " << name << ", have lost." << endl;
        else
          cerr << "Did not find expected players in FINAL command.\n"
               << "Found '" << tokens[1] << "' and '" << tokens[3] << "'. "
               << "Expected '" << name << "' and '" << opp_name << "'.\n"
               << "Received message '" << server_msg << "'" << endl;
      } else {
        // Unknown command
        cerr << "Unknown command of '" << server_msg << "' from the server";
      }
    }
  }
  cerr << "Quiting" << endl;
  */
}

template <typename GameState, typename GameClient>
void Random<GameState, GameClient>::waitForStart() {
  /*
  for (;;) {
    std::string response = Common::readMsg();
    std::vector<std::string> tokens = Common::split(response);

    if (GameClient::isValidStartGameMessage(tokens)) {
      // Found BEGIN GAME message, determine if we play first
      if (tokens[2] == name) {
        // We go first!
        opp_name = tokens[3];
        my_player = player1;
        break;
      } else if (tokens[3] == name) {
        // They go first
        opp_name = tokens[2];
        my_player = player2;
        break;
      } else {
        std::cerr << "Did not find '" << name
             << "', my name, in the BEGIN BREAKTHROUGH command.\n"
             << "# Found '" << tokens[2] << "' and '" << tokens[3] << "'"
             << " as player names. Received message '" << response << "'";
        std::cout << "#quit";
        std::exit(EXIT_FAILURE);
      }
    } // else if dumpstate
    // else if loadstate
  }

  // Player 1 goes first
  current_player = player1;
  */
}
} // namespace Common

#endif
