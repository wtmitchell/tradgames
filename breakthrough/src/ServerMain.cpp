#include <cstdlib>

#include <iostream>
using std::cout;

#include "Server.h"

int main(int argc, char* argv[])
{

    Server *s = new Server();
    s->play_game();

    return EXIT_SUCCESS;
}
