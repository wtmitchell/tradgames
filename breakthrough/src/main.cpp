#include <cstdlib>

#include <iostream>
using std::cout;

#include "game/GameState.h"
#include "Server.h"

int main(int argc, char* argv[])
{
    // Validate command line arguments

    /*
    // Start up server
    GameState* gs = new GameState();
    cout << *gs;
    gs->get_moves();
    GameState* gs2 = new GameState(7);
    cout << *gs2;
    GameState* gs3 = new GameState(4);
    cout << *gs3;
    */

    Server *s = new Server();
    s->play_game();

    return EXIT_SUCCESS;
}
