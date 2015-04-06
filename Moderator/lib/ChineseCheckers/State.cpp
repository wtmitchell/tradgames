//===------------------------------------------------------------*- C++ -*-===//
#include "ChineseCheckers/State.h"

#include <algorithm>
#include <array>
#include <set>
#include <sstream>
#include <string>
#include <vector>

#include "Common/String.h"

namespace ChineseCheckers {
bool Move::isNull() const {
  return from == to;
}

bool operator==(const Move &lhs, const Move &rhs) {
  return lhs.from == rhs.from && lhs.to == rhs.to;
}

// Lexicographic
bool operator<(const Move &lhs, const Move &rhs) {
  return lhs.from < rhs.from || (!(rhs.from < lhs.from) && lhs.to < rhs.to);
}

std::ostream &operator<<(std::ostream &out, const Move &m) {
  out << "{" << m.from << ", " << m.to << "}";
  return out;
}

State::State() {
  reset();
}

void State::getMoves(std::set<Move> &moves) const {
  moves.clear();
  for (unsigned i = 0; i < 81; ++i) {
    if (board[i] == currentPlayer) {
      getMovesSingleStep(moves, i);
      getMovesJumps(moves, i, i);
    }
  }
}

bool State::applyMove(Move m) {
  // Ensure the from and to are reasonable
  if (m.from > 80 || m.to > 80 || m.from == m.to)
    return false;

  // Check the move
  if (!isValidMove(m))
    return false;

  // Apply the move
  std::swap(board[m.from], board[m.to]);

  // Update whose turn it is
  swapTurn();

  return true;
}

bool State::undoMove(Move m) {
  // Ensure the from and to are reasonable
  if (m.from > 80 || m.to > 80 || m.from == m.to)
    return false;

  // Undo the move
  std::swap(board[m.from], board[m.to]);
  swapTurn();

  // Check the move is valid from this state that is back one step
  if (!isValidMove(m)) {
    // Woops, it was not valid, undo our changes
    swapTurn();
    std::swap(board[m.from], board[m.to]);

    return false;
  }

  return true;
}

bool State::gameOver() const {
  return player1Wins() || player2Wins();
}

int State::winner() const {
  if (player1Wins())
    return 1;
  if (player2Wins())
    return 2;
  return -1; // No one has won
}

void State::reset() {
  board = {{1, 1, 1, 1, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0,
            0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 2, 2, 0, 0, 0,
            0, 0, 0, 2, 2, 2, 0, 0, 0, 0, 0, 2, 2, 2, 2}};
  currentPlayer = 1;

}

bool State::loadState(const std::string &newState) {
  auto tokenized = Common::split(newState);

  // Ensure the length
  if (tokenized.size() != 82)
    return false;

  // Validate first item, whose turn it is
  if (tokenized[0] != "1" && tokenized[0] != "2")
    return false;
  currentPlayer = std::stoi(tokenized[0]);

  // Ensure rest of tokens are valid
  for(size_t i = 1, e = tokenized.size(); i != e; ++i) {
    int val = std::stoi(tokenized[i]);
    if (0 <= val && val <= 2)
      board[i - 1] = val;
    else
      return false;
  }
  return true;
}

std::string State::dumpState() const {
  std::stringstream out;
  out << currentPlayer;
  for (const auto i : board)
    out << " " << i;

  return out.str();
}

void State::getMovesSingleStep(std::set<Move> &moves, unsigned from) const {
  unsigned row = from / 9;
  unsigned col = from % 9;

  // Up Left
  if (col > 0 && board[from - 1] == 0)
    moves.insert({from, from - 1});

  // Up Right
  if (row > 0 && board[from - 9] == 0)
    moves.insert({from, from - 9});

  // Left
  if (col > 0 && row < 8 && board[from + 8] == 0)
    moves.insert({from, from + 8});

  // Right
  if (col < 8 && row > 0 && board[from - 8] == 0)
    moves.insert({from, from - 8});

  // Down Left
  if (row < 8 && board[from + 9] == 0)
    moves.insert({from, from + 9});

  // Down Right
  if (col < 8 && board[from + 1] == 0)
    moves.insert({from, from + 1});
}

void State::getMovesJumps(std::set<Move> &moves, unsigned from,
                          unsigned current) const {

  // If from != current we have a valid move to get here
  if (from != current) {
    auto isDupe = !moves.insert({from, current}).second;
    if (isDupe)
      return;
  }

    // Mark the current state as visited
  int originalCurrent = board[current];
  board[current] = -1;
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

bool State::isValidMove(const Move &m) const {
  // Ensure from and to make sense
  if (board[m.from] != currentPlayer || board[m.to] != 0)
    return false;

  // Get current available moves
  std::set<Move> moves;
  getMoves(moves);

  // Find the move among the set of available moves
  bool found = std::find(moves.begin(), moves.end(), m) != moves.end();

  return found;
}

Move State::translateToLocal(const std::vector<std::string> &tokens) const {
  // The numbers in the MOVE command sent by the moderator is already in the
  // format we need
  try {
    return Move{static_cast<unsigned>(std::stoi(tokens[2])),
        static_cast<unsigned>(std::stoi(tokens[4]))};
  } catch (std::invalid_argument e) {
    return Move{0, 0};
  } catch (std::out_of_range e) {
    return Move{0, 0};
  }

}

std::string State::listMoves() const {
  std::set<Move> moves;
  getMoves(moves);

  std::stringstream ss;
  for (const auto i : moves)
    ss << i.from << ", " << i.to << "; ";

  return ss.str();
}

void State::swapTurn() {
  currentPlayer = currentPlayer == 1 ?  2 : 1;
}

bool State::player1Wins() const {
  // Win by having all of bottom triangle filled and at least one is from the
  // first player

  bool p1inTriangle = false;
  for (const auto i : {53u, 61u, 62u, 69u, 70u, 71u, 77u, 78u, 79u, 80u}) {
    if (board[i] == 0)
      return false;
    if (board[i] == 1)
      p1inTriangle = true;
  }

  return p1inTriangle;
}

bool State::player2Wins() const {
  // Win by having all of top triangle filled and at least one is from the
  // second player

  bool p2inTriangle = false;
  for (const auto i : {0u, 1u, 2u, 3u, 9u, 10u, 11u, 18u, 19u, 27u}) {
    if (board[i] == 0)
      return false;
    if (board[i] == 2)
      p2inTriangle = true;
  }

  return p2inTriangle;
}

} // namespace ChineseCheckers
