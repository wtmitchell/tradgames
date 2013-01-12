#include <cstdlib>
#include <iostream>
using std::cerr;
using std::cout;
using std::endl;
#include <string>
using std::string;

#include "game/GameState.h"
#include "Client.h"

bool my_turn;
GameState* gs;
int curr_player;
string name;
string opp_name;
int player_num;

const int board_size = 8; // We play on an 8x8 board

void wait_for_start();
void play_game();

int main(int argc, char* argv[])
{
    // Determine our name from command line
    if (argc >= 2)
        name = argv[1];
    else
        name = "Random";

    gs = new GameState(board_size);
    play_game();

    return EXIT_SUCCESS;
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
        if (my_turn)
        {
            // Play Move
        }
        else
        {
            // Wait for move from other player
        }
        /*
        if (player_num == curr_player)
        {
            // My turn
            Move m = next_move();

            if (m.piece == NULL_MOVE)
            {
                // Concede to the server
                cout << "# I, " << name << ", have no moves to play." << endl;

                curr_player = (curr_player % 2) + 1;
                // End game locally
                state.apply_move(m);
                continue;
            }

            // Translate to net coordinates
            size_t from, to;
            mt.move_to_net(state, m, &from, &to);

            // Construct MOVE command
            stringstream cmd;
            cmd << "MOVE " << from << " " << to;

            // Double check that the move is valid
            if (!Client::valid_move_for_player(state, m, player_num))
            {
                stringstream err_msg;
                err_msg << "I was about to play an invalid move: "
                        << m << ".\n# The sent command would have been: '"
                        << cmd.str() << "'\n#quit";
                throw logic_error(err_msg.str());
            }

            print_and_recv_echo(cmd.str());

            // Apply our move to the state
            state.apply_move(m);

            // It is the opponents turn
            curr_player = (curr_player % 2) + 1;
        }
        else
        {
            // Not my turn
            // Get server's next instruction
            string server_msg;
            vector<string> tokens = Client::read_msg_and_tokenize(&server_msg);

            if (tokens.size() == 3 && tokens[0] == "MOVE")
            {
                try
                {
                    // Translate to local coordinates
                    Move m = mt.net_to_move(state, lexical_cast<int>(tokens[1]),
                                            lexical_cast<int>(tokens[2]));

                    // Require a piece at the from location
                    if (m.piece == NO_MOVES)
                    {
                        stringstream err_msg;
                        err_msg << "Received invalid position in MOVE command. "
                                << "There is no piece at "
                                << lexical_cast<int>(tokens[1]);
                        throw logic_error(err_msg.str());
                    }

                    // Make sure the move is valid
                    if (!Client::valid_move_for_current_player(state, m))
                    {
                        stringstream err_msg;
                        err_msg << "Received invalid MOVE command. '"
                                << server_msg << "' is not valid for "
                                << "the current player.";
                        throw logic_error(err_msg.str());
                    }

                    // Apply the move and continue
                    state.apply_move(m);

                    // It is the opponents turn
                    curr_player = (curr_player % 2) + 1;
                    continue;
                }
                catch(exception& e)
                {
                    stringstream err_msg;
                    err_msg << e.what() << "\n# Last Received Message '"
                            << server_msg << "'";
                    throw logic_error(err_msg.str());
                }
            }
            else if (tokens.size() == 4 && tokens[0] == "FINAL" && tokens[2] == "beats")
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
                    stringstream err_msg;
                    err_msg << "Did not find expected players in FINAL command.\n"
                            << "# Found '"<< tokens[2] <<"' and '" << tokens[4] << "'. "
                            << "Expected '" << name << "' and '" << opp_name <<"'.\n"
                            << "# Received message '" << server_msg << "'";
                    throw logic_error(err_msg.str());
                }
            }
            else if (tokens.size() == 1 && tokens[0] == "ps")
            {
                // Internal command to print state
                cout << state;
            }
            else
            {
                // Unknown command
                stringstream err_msg;
                err_msg << "Unknown command of '" << server_msg
                        << "' from the server";
                throw logic_error(err_msg.str());
            }
            }*/
    }
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
