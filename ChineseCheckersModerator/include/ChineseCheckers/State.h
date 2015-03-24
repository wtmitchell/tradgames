//===------------------------------------------------------------*- C++ -*-===//
///
/// \file
/// \brief Defines the Chinese Checkers game state
///
//===----------------------------------------------------------------------===//
#ifndef CHINESECHECKERS_STATE_H_INCLUDED
#define CHINESECHECKERS_STATE_H_INCLUDED

#include <array>
#include <vector>

namespace ChineseCheckers {
class State {
public:
  // Initialize with the starting state for a 2 player game
  State();

  // dtor - default since we have nothing to clean up
  ~State() = default;

  // Don't allow copies for simplicity (the functions below are for the rule of 5)
  // copy ctor
  State(const State&) = delete;
  // move ctor
  State(const State&&) = delete;
  // copy assignment
  State &operator=(const State&) = delete;
  // move assignment
  State &operator=(const State&&) = delete;

  // Put all valid moves into the vector of moves passed in by reference
  void getMoves(std::vector<Move> &moves) const;

  // Apply the move m, returning true if m is a valid move, false if not
  bool applyMove(Move m);

  // Undo the move m, returning true if m is a move that can be undone, false if not
  bool undoMove(Move m);

  // Returns true iff the game is over
  bool gameOver() const;

  // Loads the state stored in the string, returning true if it is a valid state, false if not
  bool loadState(const std::string &newState);

  // Dump out the current state, usable with loadState
  std::string dumpState() const;
private:
  std::array<int, 81> board;
  bool currentPlayer;
};
} // namespace ChineseCheckers
