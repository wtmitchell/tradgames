#include <cstdlib>

#include <iostream>
using std::cout;

#include "Common/Moderator.h"
#include "ChineseCheckers/State.h"
#include "ChineseCheckers/Client.h"

int main(int argc, char *argv[]) {
  bool quiet = false;
  bool printBoard = false;
  double turnTimeLimit = 30.0; // in seconds
  bool logGame = true;
  Common::Moderator<ChineseCheckers::State, ChineseCheckers::Client> m;
  m.playGame(printBoard, quiet, turnTimeLimit, logGame);

  return EXIT_SUCCESS;
}
