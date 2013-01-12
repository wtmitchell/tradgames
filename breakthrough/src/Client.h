#ifndef CLIENT_H
#define CLIENT_H

#include <string>
using std::string;
#include <vector>
using std::vector;

namespace Client
{
    string read_msg();
    vector<string> read_msg_and_tokenize(string *response);
}

#endif
