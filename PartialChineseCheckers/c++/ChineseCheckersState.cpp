//===------------------------------------------------------------*- C++ -*-===//
#include "ChineseCheckersState.h"

#include <algorithm>
#include <array>
#include <iterator>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

Move::operator std::string() const {
  std::stringstream ss;
  ss << "MOVE FROM " << from << " TO " << to;
  return ss.str();
}

bool operator==(const Move &lhs, const Move &rhs) {
  return lhs.from == rhs.from && lhs.to == rhs.to;
}

// Lexicographic
bool operator<(const Move &lhs, const Move &rhs) {
  return lhs.from < rhs.from || (!(rhs.from < lhs.from) && lhs.to < rhs.to);
}

ChineseCheckersState::ChineseCheckersState() {
  reset();
}

void ChineseCheckersState::getMoves(std::vector<Move> &moves) const {
  // WARNING: This function must not return duplicate moves
  moves.clear();

  for (unsigned i = 0; i < 81; ++i) {
    if (board[i] == currentPlayer) {
      getMovesSingleStep(moves, i);
      // Need to add jump moves
    }
  }
}

bool ChineseCheckersState::applyMove(Move m) {
  // Ensure the from and to are reasonable
  if (m.from > 80 || m.to > 80 || m.from == m.to)
    return false;

  // Check the move
  // FIXME: This should be uncommented once you have getMoves working!!
  /*
  if (!isValidMove(m))
    return false;
  */

  // Apply the move
  std::swap(board[m.from], board[m.to]);

  // Update whose turn it is
  swapTurn();

  return true;
}

bool ChineseCheckersState::undoMove(Move m) {
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

bool ChineseCheckersState::gameOver() const {
  return player1Wins() || player2Wins();
}

int ChineseCheckersState::winner() const {
  if (player1Wins())
    return 1;
  if (player2Wins())
    return 2;
  return -1; // No one has won
}

void ChineseCheckersState::reset() {
  board = {{1, 1, 1, 1, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0,
            0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 2, 2, 0, 0, 0,
            0, 0, 0, 2, 2, 2, 0, 0, 0, 0, 0, 2, 2, 2, 2}};
  currentPlayer = 1;

}

bool ChineseCheckersState::loadState(const std::string &newState) {
  // Tokenize newState using whitespace as delimiter
  std::stringstream ss(newState);
  std::istream_iterator<std::string> begin(ss);
  std::istream_iterator<std::string> end;
  std::vector<std::string> tokenized(begin, end);

  // Ensure the length
  if (tokenized.size() != 82)
    return false;

  // Validate first item, whose turn it is
  if (tokenized[0] != "1" && tokenized[0] != "2")
    return false;

  try {
    currentPlayer = std::stoi(tokenized[0]);
  } catch (std::invalid_argument e) {
    return false;
  } catch (std::out_of_range e) {
    return false;
  }

  // Ensure rest of tokens are valid
  for (size_t i = 1, e = tokenized.size(); i != e; ++i) {
    try {
      int val = std::stoi(tokenized[i]);
      if (0 <= val && val <= 2)
        board[i - 1] = val;
      else
        return false;
    } catch (std::invalid_argument e) {
      return false;
    } catch (std::out_of_range e) {
      return false;
    }
  }
  return true;
}

std::string ChineseCheckersState::dumpState() const {
  std::stringstream out;
  out << currentPlayer;
  for (const auto i : board)
    out << " " << i;

  return out.str();
}

void ChineseCheckersState::getMovesSingleStep(std::vector<Move> &moves, unsigned from) const {
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

bool ChineseCheckersState::isValidMove(const Move &m) const {
  // Ensure from and to make sense
  if (board[m.from] != currentPlayer || board[m.to] != 0)
    return false;

  // NOTE: Checking validity in this way is inefficient

  // Get current available moves
  std::vector<Move> moves;
  getMoves(moves);

  // Find the move among the set of available moves
  bool found = std::find(moves.begin(), moves.end(), m) != moves.end();

  return found;
}

Move ChineseCheckersState::translateToLocal(const std::vector<std::string> &tokens) const {
  // The numbers in the MOVE command sent by the moderator is already in the
  // format we need
  return Move{static_cast<unsigned>(std::stoi(tokens[2])),
              static_cast<unsigned>(std::stoi(tokens[4]))};
}

void ChineseCheckersState::swapTurn() {
  currentPlayer = currentPlayer == 1 ? 2 : 1;
}

bool ChineseCheckersState::player1Wins() const {
  // Wins by having all the bottom triangle filled and at least one is from the
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

bool ChineseCheckersState::player2Wins() const {
  // Wins by having all of top triangle filled and at least one is from the
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

