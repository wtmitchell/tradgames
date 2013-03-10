#include <cstdlib>

#include <iostream>
using std::cout;

#include "Server.h"

int main(int argc, char* argv[])
{
    bool quiet = false;
    bool print_board = false;
    double turn_time_limit = 30.0; // in seconds
    bool log_game = true;
    Server *s = new Server();
    s->play_game(print_board, quiet, turn_time_limit, log_game);

    return EXIT_SUCCESS;
}
