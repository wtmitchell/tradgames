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
using std::stoi;
using std::string;
#include <vector>
using std::vector;

#include "Server.h"

#include "Client.h"

Server::Server()
{}

void Server::play_game()
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

    // Store the server's set of messages it is waiting to hear echoed
    set<string> echo;

    // Start game
    stringstream cmd;
    cmd << "BEGIN BREAKTHROUGH " << player_names[0] << " " << player_names[1];

    cout << cmd.str() << endl;
    echo.insert(cmd.str());
    // start timer

    // Player 1 has turn 0
    int turn = 0;
    int moves = 1;

    // Main game loop
    for (;;)
    {
        // Read message
        string msg;
        vector<string> tokens = Client::read_msg_and_tokenize(&msg);
        //move_stop = microsec_clock::universal_time();
        set<string>::iterator it = echo.find(msg);

        if (it != echo.end())
        {
            // We received an expected echo
            echo.erase(it);
            continue;
        }

        // Stop timer

        // Print out turn and time information

        if (tokens.size() == 6
            //&& tokens[0] == bs::lexical_cast<string>(players[turn])
            && tokens[1] == "MOVE")
        {
            cerr << "Move msg: '" << msg << "'" << endl;
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

        int players = stoi(tokens[1]);

        if (tokens.size() == 2 && tokens[0] == "#players" && players >= 3)
        {
            // Enough have joined. Get player names
            for (int i = 0; i < players; ++i)
            {
                cout << "#getname "<< i << endl;
                string name_response;
                vector<string> name_tokens = Client::read_msg_and_tokenize(&name_response);
                if (name_tokens.size() == 3 && name_tokens[0] == "#getname" &&
                    stoi(name_tokens[1]) == i)
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
    for (auto s : names)
    {
        if (s != "server" && s != "observer")
        {
            if (i > 1)
                cerr << "Too many clients connected, using only "
                     << player_names[0] << " and " << player_names[1]
                     << endl;

            player_names[i] = s;
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
