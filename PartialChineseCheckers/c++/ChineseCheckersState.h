//===------------------------------------------------------------*- C++ -*-===//
///
/// \file
/// \brief Defines the Chinese Checkers game state
///
/// Note: Many aspects of this State are inefficient to make the code clearer
///
//===----------------------------------------------------------------------===//
#ifndef CHINESECHECKERS_STATE_H_INCLUDED
#define CHINESECHECKERS_STATE_H_INCLUDED

#include <array>
#include <ostream>
#include <set>
#include <string>
#include <vector>

struct Move {
  unsigned from;
  unsigned to;

  operator std::string() const; //conversion operator
};
bool operator==(const Move &lhs, const Move &rhs);
bool operator<(const Move &lhs, const Move &rhs);
std::ostream &operator<<(std::ostream &out, const Move &m);


class ChineseCheckersState {
public:
  // Initialize with the starting state for a 2 player game
  ChineseCheckersState();

  // dtor - default since we have nothing to clean up
  ~ChineseCheckersState() = default;

  // Don't allow copies for simplicity (the functions below are for the rule of 5)
  // copy ctor
  ChineseCheckersState(const ChineseCheckersState&) = delete;
  // move ctor
  ChineseCheckersState(const ChineseCheckersState&&) = delete;
  // copy assignment
  ChineseCheckersState &operator=(const ChineseCheckersState&) = delete;
  // move assignment
  ChineseCheckersState &operator=(const ChineseCheckersState&&) = delete;

  // Put all valid moves into the vector of moves passed in by reference
  void getMoves(std::vector<Move> &moves) const;

  // Apply the move m, returning true if m is a valid move, false if not
  bool applyMove(Move m);

  // Undo the move m, returning true if m is a move that can be undone, false if not
  bool undoMove(Move m);

  // Returns true iff the move m is valid
  bool isValidMove(const Move &m) const;

  // Returns true iff the game is over
  bool gameOver() const;

  // Return the player who won, assuming the game is over
  int winner() const;

  // Reset the board to the initial state
  void reset();

  // Loads the state stored in the string, returning true if it is a valid state, false if not
  bool loadState(const std::string &newState);

  // Dump out the current state, usable with loadState
  std::string dumpState() const;

  // Translates a sequence of tokens from the move format used to the local move type
  Move translateToLocal(const std::vector<std::string> &tokens) const;

private:
  std::array<int, 81> board;
  bool currentPlayer;

  void getMovesSingleStep(std::vector<Move> &moves, unsigned from) const;

  void swapTurn();

  bool player1Wins() const;
  bool player2Wins() const;
};

#endif
