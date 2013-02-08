#include <cstdlib>

#include <iostream>
using std::cout;

#include "Server.h"

int main(int argc, char* argv[])
{
    bool quiet = false;
    bool print_board = false;
    Server *s = new Server();
    s->play_game(print_board, quiet);

    return EXIT_SUCCESS;
}
