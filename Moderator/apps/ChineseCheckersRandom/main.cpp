#include <cstdlib>
#include <string>

#include "Common/RandomPlayer.h"
#include "ChineseCheckers/State.h"
#include "ChineseCheckers/Client.h"

int main(int argc, char *argv[]) {
  // Determine our name from command line
  std::string name;
  if (argc >= 2)
    name = argv[1];
  else
    name = "Random";

  Common::Random<ChineseCheckers::State, ChineseCheckers::Client> rp(name);
  rp.playGame();

  return EXIT_SUCCESS;
}
