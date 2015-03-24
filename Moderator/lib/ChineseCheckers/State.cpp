//===------------------------------------------------------------*- C++ -*-===//
#include "ChineseCheckers/State.h"

#include <array>
#include <sstream>
#include <string>
#include <vector>

namespace ChineseCheckers {
bool operator==(const Move &lhs, const Move &rhs) {
  return lhs.from == rhs.from && lhs.to == rhs.to;
}

std::ostream &operator<<(std::ostream &out, const Move &m) {
  out << "{" << m.from << ", " << m.to << "}";
  return out;
}


State::State()
    : board{{1, 1, 1, 1, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0,
             0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
             0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 2, 2, 0, 0, 0,
             0, 0, 0, 2, 2, 2, 0, 0, 0, 0, 0, 2, 2, 2, 2}},
      currentPlayer(1) {}

void State::getMoves(std::vector<Move> &moves) const {
  moves.clear();

  for (unsigned i = 0; i < 81; ++i) {
    if (board[i] == currentPlayer) {
      getMovesSingleStep(moves, i);
      getMovesJumps(moves, i, i);
    }
  }
}

bool State::applyMove(Move m) {
  return false; // TODO
}

bool State::undoMove(Move m) {
  std::swap(m.from, m.to);
  return applyMove(m);
}

bool State::gameOver() const {
  return false; // TODO
}

int State::winner() const {
  return -1; // TODO
}

std::string State::dumpState() const {
  std::stringstream out;
  out << currentPlayer;
  for (const auto i : board)
    out << " " << i;

  return out.str();
}

void State::getMovesSingleStep(std::vector<Move> &moves, unsigned from) const {
  unsigned row = from / 9;
  unsigned col = from % 9;

  // Up Left
  if (col > 0 && board[from - 1] == 0)
    moves.push_back({from, from - 1});

  // Up Right
  if (row > 0 && board[from - 9] == 0)
    moves.push_back({from, from - 9});

  // Left
  if (col > 0 && row < 8 && board[from + 8] == 0)
    moves.push_back({from, from + 8});

  // Right
  if (col < 8 && row > 0 && board[from - 8] == 0)
    moves.push_back({from, from - 8});

  // Down Left
  if (row < 8 && board[from + 9] == 0)
    moves.push_back({from, from + 9});

  // Down Right
  if (col < 8 && board[from + 1] == 0)
    moves.push_back({from, from + 1});
}

void State::getMovesJumps(std::vector<Move> &moves, unsigned from,
                          unsigned current) const {
  // Mark the current state as visited
  int originalCurrent = board[current];
  board[current] = -1;

  // If from != current we have a valid move to get here
  if (from != current)
    moves.push_back({from, current});

  unsigned row = current / 9;
  unsigned col = current % 9;

  // Up Left
  if (col > 1 && board[current - 2] == 0) {
    int jumped = board[current - 1];
    if (jumped == 1 || jumped == 2)
      getMovesJumps(moves, from, current - 2);
  }

  // Up Right
  if (row > 1 && board[current - 18] == 0) {
    int jumped = board[current - 9];
    if (jumped == 1 || jumped == 2)
      getMovesJumps(moves, from, current - 18);
  }

  // Left
  if (col > 1 && row < 7 && board[current + 16] == 0) {
    int jumped = board[current + 8];
    if (jumped == 1 || jumped == 2)
      getMovesJumps(moves, from, current + 16);
  }

  // Right
  if (col < 7 && row > 1 && board[current - 16] == 0) {
    int jumped = board[current - 8];
    if (jumped == 1 || jumped == 2)
      getMovesJumps(moves, from, current - 16);
  }

  // Down Left
  if (row < 7 && board[current + 18] == 0) {
    int jumped = board[current + 9];
    if (jumped == 1 || jumped == 2)
      getMovesJumps(moves, from, current + 18);
  }

  // Down Right
  if (col < 7 && board[current + 2] == 0) {
    int jumped = board[current + 1];
    if (jumped == 1 || jumped == 2)
      getMovesJumps(moves, from, current + 2);
  }

  // Restore the current state
  board[current] = originalCurrent;
}

} // namespace ChineseCheckers
