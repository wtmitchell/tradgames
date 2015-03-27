#include <cstdlib>

#include <iostream>
using std::cout;

#include "Common/Moderator.h"
#include "ChineseCheckers/State.h"
#include "ChineseCheckers/Client.h"

int main(int /*argc*/, char **/*argv*/) {
  bool quiet = false;
  bool printBoard = true; // to stderr
  double turnTimeLimit = 30.0; // in seconds
  bool logGame = false; // to file
  bool enforceTimeLimit = false;
  Common::Moderator<ChineseCheckers::State, ChineseCheckers::Client> m;
  m.playGame(printBoard, quiet, turnTimeLimit, logGame, enforceTimeLimit);

  return EXIT_SUCCESS;
}
