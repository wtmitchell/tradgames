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
      const unsigned row = i / 9;
      const unsigned col = i % 9;

      // Single space moves
      // Up Left
      if (col > 0 && board[i - 1] == 0)
        moves.push_back({i, i - 1});
      // Up Right
      if (row > 0 && board[i - 9] == 0)
        moves.push_back({i, i - 9});
      // Left
      if (col > 0 && row < 8 && board[i + 8] == 0)
        moves.push_back({i, i + 8});
      // Right
      if (col < 8 && row > 0 && board[i - 8] == 0)
        moves.push_back({i, i - 8});
      // Down Left
      if (row < 8 && board[i + 9] == 0)
        moves.push_back({i, i + 9});
      // Down Right
      if (col < 8 && board[i + 1] == 0)
        moves.push_back({i, i + 1});

      // Jumps
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
} // namespace ChineseCheckers
