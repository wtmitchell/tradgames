#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <stdexcept>
#include <string>

#include "Common/Moderator.h"
#include "ChineseCheckers/State.h"
#include "ChineseCheckers/Client.h"

bool commandExists(char **begin, char **end, const std::string &name);
char *getOption(char **begin, char **end, const std::string &name);

int main(int argc, char **argv) {
  // Defaults
  bool quiet = false;
  bool printBoard = true; // to stderr
  double turnTimeLimit = 30.0; // in seconds
  bool logGame = false; // to file
  bool enforceTimeLimit = false;

  // Check if command line arguments overrides any of these
  if (commandExists(argv, argv + argc, "--quiet")) {
    quiet = true;
    printBoard = false;
  }

  if (commandExists(argv, argv + argc, "--enforce")) {
    enforceTimeLimit = true;
    char *limit = getOption(argv, argv + argc, "--enforce");
    if (limit != nullptr) {
      try {
        turnTimeLimit = std::stod(limit);
      } catch (std::invalid_argument e) {
        std::cerr << "Invalid timeout of (std::invalid_argument) " << limit << std::endl;
      } catch (std::out_of_range e) {
        std::cerr << "Invalid timeout of (std::out_of_range) " << limit << std::endl;
      }
    }
  }

  if (!printBoard)
    std::cout << "--quiet enabled. Will not print GUI updates to std::err" << std::endl;

  if (enforceTimeLimit)
    std::cout << "Will enforce time limit of " << turnTimeLimit << " seconds." << std::endl;

  Common::Moderator<ChineseCheckers::State, ChineseCheckers::Client> m;
  m.playGame(printBoard, quiet, turnTimeLimit, logGame, enforceTimeLimit);

  return EXIT_SUCCESS;
}

bool commandExists(char **begin, char **end, const std::string &name) {
  return std::find(begin, end, name) != end;
}

char *getOption(char **begin, char **end, const std::string &name)
{
    char **itr = std::find(begin, end, name);
    if (itr != end && ++itr != end)
    {
        return *itr;
    }
    return nullptr;
}
