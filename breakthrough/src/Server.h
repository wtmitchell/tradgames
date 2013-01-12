#ifndef SERVER_H
#define SERVER_H

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
};

#endif
