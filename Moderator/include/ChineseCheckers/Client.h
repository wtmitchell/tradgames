//===------------------------------------------------------------*- C++ -*-===//
///
/// \file
/// \brief Code specific to a game of Chinese Checkers
///
//===----------------------------------------------------------------------===//
#ifndef CHINESECHECKERS_CLIENT_H_INCLUDED
#define CHINESECHECKERS_CLIENT_H_INCLUDED

#include <string>
#include <vector>

#include "ChineseCheckers/State.h"

namespace ChineseCheckers {
class Client {
public:
  static std::string startGameMessage(const std::string &player1, const std::string &player2);
  static bool isValidStartGameMessage(const std::vector<std::string> &tokens);
  static bool isValidMoveMessage(unsigned currentPlayer, const std::vector<std::string> &tokens);
  static std::string moveMessage(Move m);
};
} // namespace ChineseCheckers
#endif
