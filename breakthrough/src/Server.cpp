#include <iostream>
using std::cin;
using std::cerr;
using std::cout;
using std::endl;
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

    wait_for_start();
}

void Server::wait_for_start()
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

    cout << "Names are:\n";
    for (auto s : names)
        cout << s << "\n";
    cout << endl;

    /*if (names[0] == names[1] || names[0] == names[2] || names[1] == names[2])
    {
        cout << "FINAL " << names[0] << " beats " << names[1] << endl;
        stringstream err_msg;
        err_msg << "Have duplicate names on the server. Found players: '" << names[0]
                << "', '" << names[1] << "', and '" << names[2] << "'.";
        throw logic_error(err_msg.str());
        }*/
}
