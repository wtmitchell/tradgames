#include "ChineseCheckers/Client.h"

#include <sstream>
#include <string>

namespace ChineseCheckers {
std::string Client::startGameMessage(const std::string &player1, const std::string &player2) {
  std::stringstream cmd;
  cmd << "BEGIN CHINESECHECKERS " << player1 << " " << player2;
  return cmd.str();
}

bool Client::isValidStartGameMessage(const std::vector<std::string> &tokens) {
  return tokens.size() == 4 && tokens[0] == "BEGIN" && tokens[1] == "CHINESECHECKERS";
}


bool Client::isValidMoveMessage(unsigned currentPlayer, const std::vector<std::string> &tokens) {
  return tokens.size() == 5 &&
         static_cast<unsigned>(std::stoi(tokens[0])) == currentPlayer &&
         tokens[1] == "MOVE" &&
         static_cast<unsigned>(std::stoi(tokens[2])) < 81 &&
         tokens[3] == "TO" && static_cast<unsigned>(std::stoi(tokens[4])) < 81;
}

std::string Client::moveMessage(Move m) {
  std::stringstream cmd;
  cmd << "MOVE FROM " << m.from << " TO " << m.to;
  return cmd.str();
}

} // namespace ChineseCheckers
