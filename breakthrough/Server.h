#ifndef SERVER_H
#define SERVER_H

#include <stdexcept>
using std::logic_error;
#include <string>
using std::string;
#include <vector>
using std::vector;

#include "game/GameState.h"

class Server
{
 public:
    Server();

    void play_game(bool print_board, bool quiet, double turn_time_limit);

 private:
    void wait_for_start() throw (logic_error);

    GameState* gs;
    vector<string> names;
    string player_names[2];
    size_t player_ids[2];
};

#endif
