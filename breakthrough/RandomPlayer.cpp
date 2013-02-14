#include <cstdlib>
#include <iostream>
using std::cerr;
using std::cout;
using std::endl;
#include <string>
using std::string;

#include "game/GameState.h"
#include "game/Move.h"
#include "Client.h"
#include "Math.h"
#include "game/Players.h"

GameState* gs;
Players current_player;
Players my_player;
string name;
string opp_name;

const int board_size = 8; // We play on an 8x8 board

Move next_move();
void play_game();
void print_and_recv_echo(string msg);
void wait_for_start();

int main(int argc, char* argv[])
{
    // Determine our name from command line
    if (argc >= 2)
        name = argv[1];
    else
        name = "Random";

    Math::seed_rand();

    gs = new GameState(board_size);
    play_game();

    return EXIT_SUCCESS;
}

Move next_move()
{
    vector<Move> moves = gs->get_moves();

    if (moves.size() > 0)
    {
        unsigned int choice = Math::uniform_rand(static_cast<unsigned int>(moves.size()));
        return moves[choice];
    }

    return Move(0, 0); //null move
}


void play_game()
{
    // Identify myself
    cout << "#name " << name << endl;

    // Wait for start of game
    wait_for_start();

    // Main game loop
    for (;;)
    {
        if (current_player == my_player)
        {
            // My turn
            if (gs->game_over())
            {
                cerr << "By looking at the board, I know that I, " << name << ", have lost." << endl;
                current_player = (current_player == Players::player1)
                    ? Players::player2 : Players::player1;
                continue;
            }
            // Determine next move
            Move m = next_move();

            // Double check it is valid
            if (!gs->valid_move(m))
            {
                cerr << "I was about to play an invalid move: "
                     << gs->pretty_print_move(m) << endl;
                cout << "#quit" << endl;
            }

            // Apply it
            gs->apply_move(m);

            if (m.isNull())
            {
                // Concede to the server so we know what is going on
                cout << "# I, " << name << ", have no moves to play." << endl;

                current_player = (current_player == Players::player1)
                    ? Players::player2 : Players::player1;
                // End game locally, server should detect and send #quit
                continue;
            }

            // Tell the world
            print_and_recv_echo(gs->pretty_print_move(m));

            // It is the opponents turn
            current_player = (current_player == Players::player1)
                ? Players::player2 : Players::player1;
        }
        else
        {
            // Wait for move from other player
            // Get server's next instruction
            string server_msg;
            vector<string> tokens = Client::read_msg_and_tokenize(&server_msg);

            if (tokens.size() == 6 && tokens[0] == "MOVE")
            {
                // Translate to local coordinates
                Move m = gs->translate_to_local(tokens);

                // Apply the move and continue
                gs->apply_move(m);

                // It is now my turn
                current_player = (current_player == Players::player1)
                    ? Players::player2 : Players::player1;
            }
            else if (tokens.size() == 4 && tokens[0] == "FINAL" && tokens[2] == "BEATS")
            {
                // Game over
                if (tokens[1] == name && tokens[3] == opp_name)
                    cerr << "I, " << name << ", have won!" << endl;
                else if (tokens[3] == name && tokens[1] == opp_name)
                    cerr << "I, " << name << ", have lost." << endl;
                else
                    cerr << "Did not find expected players in FINAL command.\n"
                         << "Found '"<< tokens[1] <<"' and '" << tokens[3] << "'. "
                         << "Expected '" << name << "' and '" << opp_name <<"'.\n"
                         << "Received message '" << server_msg << "'" << endl;
            }
            else
            {
                // Unknown command
                cerr << "Unknown command of '" << server_msg
                     << "' from the server";
            }
        }
    }
    cerr << "Quiting" << endl;
}

/* Sends a msg to stdout and verifies that the next message to come in
   is it echoed back. This is how the server to validates moves */
void print_and_recv_echo(string msg)
{
    cout << msg << endl; // Note the endl flushes the stream
    string echo_recv = Client::read_msg();
    if (msg != echo_recv)
        cerr << "Expected echo of '" << msg << "'. Received '" << echo_recv << "'";
}


void wait_for_start()
{
    for (;;)
    {
        string response;
        vector<string> tokens = Client::read_msg_and_tokenize(&response);

        if (tokens.size() == 4 && tokens[0] == "BEGIN" && tokens[1] == "BREAKTHROUGH")
        {
            // Found BEGIN GAME message, determine if we play first
            if (tokens[2] == name)
            {
                // We go first!
                opp_name = tokens[3];
                my_player = Players::player1;
                break;
            }
            else if (tokens[3] == name)
            {
                // They go first
                opp_name = tokens[2];
                my_player = Players::player2;
                break;
            }
            else
            {
                cerr << "Did not find '" << name
                     << "', my name, in the BEGIN BREAKTHROUGH command.\n"
                     << "# Found '"<< tokens[2] <<"' and '" << tokens[3] << "'"
                     << " as player names. Received message '" << response << "'";
                cout << "#quit";
                exit(EXIT_FAILURE);
            }
        }
    }

    // Player 1 goes first
    current_player = Players::player1;
}
