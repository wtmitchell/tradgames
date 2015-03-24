#include <iostream>
using std::cin;
using std::cerr;
using std::cout;
using std::endl;
#include <stdexcept>
using std::logic_error;
#include <set>
using std::set;
#include <sstream>
using std::stringstream;
#include <string>
using std::string;
#include <vector>
using std::vector;

#include "Server.h"

#include "Client.h"
#include "game/GameState.h"
#include "String.h"
#include "Timer.h"

Server::Server()
{
    gs = new GameState();
    logging = false;
}

Server::~Server()
{
    if (log_file.is_open())
        log_file.close();
}

void Server::play_game(bool print_board, bool quiet, double turn_time_limit, bool log_game)
{
    // Identify myself
    cout << "#name server\n"
         << "#master"
         << endl;

    try {
        wait_for_start();
    }
    catch (logic_error e)
    {
        // Duplicate names can't be recovered from
        return;
    }

    // Set up logging
    if (log_game)
        set_up_logging();

    // Setup the timer
    Timer *move_timer = new Timer();

    // Start game
    stringstream cmd;
    cmd << "BEGIN BREAKTHROUGH " << player_names[0] << " " << player_names[1];
    broadcast(cmd.str());

    // start timer
    move_timer->start();

    // Player 1 has turn 0
    int turn = 0;
    turn_count = 0;

    // Main game loop
    for (;;)
    {
        // Read message
        string msg;
        vector<string> tokens = Client::read_msg_and_tokenize(&msg);

        // Stop the timer
        move_timer->stop();

        // Look for echoes
        set<string>::iterator it = echo.find(msg);
        if (it != echo.end())
        {
            // We received an expected echo
            echo.erase(it);
            continue;
        }

        if (tokens.size() == 7
            && static_cast<size_t>(String::stoi(tokens[0])) == player_ids[turn]
            && tokens[1] == "MOVE"
            && tokens[4] == "TO")
        {
            ++turn_count;
            // Print out turn and time information
            stringstream time_msg;
            time_msg << "Turn "<< turn_count << " by " << player_ids[turn] << ":"
                     << player_names[turn] << " took " << *move_timer;
            diagnostic(time_msg.str());

            // Took too long
            if (move_timer->seconds_elapsed() > turn_time_limit)
            {
                stringstream forfeit_msg;
                forfeit_msg << "Too long. Exceeds time limit of " << turn_time_limit << " seconds. "
                            << "Took " << move_timer->seconds_elapsed() << " seconds. "
                            << player_ids[turn] << ":" << player_names[turn]
                            << " forfeits.";
                diagnostic(forfeit_msg.str());

                if (print_board)
                    cerr << *gs << endl;

                final((turn + 1) % 2, turn);
                broadcast("#quit");
                continue;
            }


            // Received move from current player
            Move m = gs->translate_to_local(tokens);

            // Validate move
            if (!gs->valid_move(m))
            {
                // Display board if requested
                if (print_board)
                    cerr << *gs << endl;
                stringstream invalid_msg;
                invalid_msg << "Invalid move: " << msg;
                diagnostic(invalid_msg.str());

                final((turn + 1) % 2, turn);
                broadcast("#quit");
                continue;
            }

            // Apply move and echo it
            gs->apply_move(m);
            broadcast(gs->pretty_print_move(m));

            // Start timer for next player's move
            move_timer->start();

            // Display board if requested
            if (print_board)
                cerr << *gs << endl;

            // Check if game is over
            if (gs->game_over())
            {
                // Game was just won by last played move
                final(turn, (turn + 1) % 2);
                broadcast("#quit");
                continue;
            }

            // Alternate whose turn
            turn = (turn + 1) % 2;
        }
        else
        {
            if (!quiet)
                cerr << "Received unknown or out of turn message: " << msg << endl;
        }
    }
}

void Server::wait_for_start() throw (logic_error)
{
    // Wait for the game to begin
    for (;;)
    {
        cout << "#players" << endl;
        string response;
        vector<string> tokens = Client::read_msg_and_tokenize(&response);

        int players = String::stoi(tokens[1]);

        if (tokens.size() == 2 && tokens[0] == "#players" && players >= 3)
        {
            // Enough have joined. Get player names
            for (int i = 0; i < players; ++i)
            {
                cout << "#getname "<< i << endl;
                string name_response;
                vector<string> name_tokens = Client::read_msg_and_tokenize(&name_response);
                if (name_tokens.size() == 3 && name_tokens[0] == "#getname" &&
                    String::stoi(name_tokens[1]) == i)
                {
                    names.push_back(name_tokens[2]);
                }
                else
                {
                    cerr << "Did not received expected response '#getname " << i <<" name'."
                         << " Received message '" << name_response << "'" << endl;
                }
            }
            break;
        }
    }

    // Determine player names
    int i = 0;
    for (size_t j = 0; j < names.size(); ++j)
    {
        if (names[j] != "server" && names[j] != "observer")
        {
            if (i > 1)
                cerr << "Too many clients connected, using only "
                     << player_names[0] << " and " << player_names[1]
                     << endl;

            player_names[i] = names[j];
            player_ids[i] = j;
            ++i;
        }

    }

    // Verify a game can begin
    if (player_names[0] == player_names[1])
    {
        cerr << "Both players have duplicate names: " << player_names[0]
             << endl;
        cout << "FINAL " << player_names[0] << " beats " << player_names[1]
             << endl;
        throw(logic_error("Duplicate names"));
    }
}

// Open files for logging
void Server::set_up_logging()
{
    string filename = player_names[0] + "-vs-" + player_names[1] + ".txt";
    log_file.open(filename.c_str());
    logging = true;
}

void Server::broadcast(const string& msg)
{
    echo.insert(msg);
    if (logging)
        log_file << msg << endl;
    cout << msg << endl;
}

void Server::diagnostic(const string& msg)
{
    if (logging)
        log_file << "# " << msg << endl;
    cerr << msg << endl;
}

void Server::final(size_t winner, size_t loser)
{
    stringstream final_msg;
    final_msg << "FINAL " << player_names[winner] << " BEATS "
              << player_names[loser];
    broadcast(final_msg.str());
    stringstream movecount_msg;
    movecount_msg << turn_count << " moves were played in total";
    diagnostic(movecount_msg.str());
}
