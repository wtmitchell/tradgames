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

bool my_turn;
GameState* gs;
int curr_player;
string name;
string opp_name;
int player_num;

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
        if (curr_player == player_num)
        {
            // Play Move
            cout << "My turn!" << endl;
            Move m = next_move();

            if (m.isNull())
            {
                // Concede to the server
                cout << "# I, " << name << ", have no moves to play." << endl;

                curr_player = (curr_player % 2) + 1;
                // End game locally
                gs->apply_move(m);
                continue;
            }
            print_and_recv_echo(gs->pretty_print_move(m));

            // It is the opponents turn
            curr_player = (curr_player % 2) + 1;
        }
        else
        {
            // Wait for move from other player
            // Get server's next instruction
            string server_msg;
            vector<string> tokens = Client::read_msg_and_tokenize(&server_msg);

            if (tokens.size() == 7 && tokens[0] == "MOVE")
            {
                cerr << "Have move from opponent" << endl;
                // Translate to local coordinates
                /*
                Move m = mt.net_to_move(state, lexical_cast<int>(tokens[1]),
                                        lexical_cast<int>(tokens[2]));

                // Apply the move and continue
                state.apply_move(m);
                */

                // It is now my turn
                curr_player = (curr_player % 2) + 1;
            }
            else if (tokens.size() == 4 && tokens[0] == "FINAL" && tokens[2] == "BEATS")
            {
                // Game over
                if (tokens[2] == name && tokens[4] == opp_name)
                {
                    cout << "# I, " << name << ", have won!" << endl;
                }
                else if (tokens[4] == name && tokens[2] == opp_name)
                {
                    cout << "# I, " << name << ", have lost." << endl;
                }
                else
                {
                    cerr << "Did not find expected players in FINAL command.\n"
                         << "# Found '"<< tokens[2] <<"' and '" << tokens[4] << "'. "
                         << "Expected '" << name << "' and '" << opp_name <<"'.\n"
                         << "# Received message '" << server_msg << "'";
                }
            }
            else
            {
                // Unknown command
                cerr << "Unknown command of '" << server_msg
                     << "' from the server";
            }
        }
    }
}

/* Sends a msg to stdout and verifies that the next message to come in
   is it echoed back. This is used by the server to validate moves */
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
                player_num = 1;
                break;
            }
            else if (tokens[3] == name)
            {
                // They go first
                opp_name = tokens[2];
                player_num = 2;
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
    curr_player = 1;
}
