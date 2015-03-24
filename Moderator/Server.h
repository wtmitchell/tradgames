#ifndef SERVER_H
#define SERVER_H

#include <fstream>
#include <set>
using std::set;
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
    ~Server();

    void play_game(bool print_board, bool quiet, double turn_time_limit, bool log_game);

 private:
    void wait_for_start() throw (logic_error);
    void set_up_logging();

    void broadcast(const string& msg);
    void diagnostic(const string& msg);
    void final(size_t winner, size_t loser);

    GameState* gs;
    set<string> echo;
    vector<string> names;
    string player_names[2];
    size_t player_ids[2];
    std::ofstream log_file;
    bool logging;
    int turn_count;
};

#endif
