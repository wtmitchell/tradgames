#ifndef SERVER_H
#define SERVER_H

#include <array>
using std::array;
#include <string>
using std::string;
#include <vector>
using std::vector;

class Server
{
 public:
    Server();

    void play_game();

 private:
    void wait_for_start();

    vector<string> names;
    array<string, 2> player_names;
};

#endif
