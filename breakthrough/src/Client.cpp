#include <iostream>
using std::cin;
#include <string>
using std::string;

#include "Client.h"

#include "String.h"


/* Reads a line, up to a newline from the server */
string Client::read_msg()
{
    string msg;
    getline(cin, msg);
    msg = String::rtrim(msg);
    return msg;
}

/* Reads a line from the server and tokenizes the results based on a delimiter
   of a space. If the actual server message is requested via a non-null argument,
   it is recorded */
vector<string> Client::read_msg_and_tokenize(string *response)
{
    string msg = read_msg();
    vector<string> tokens = String::split(msg, ' ');

    if (response != NULL)
    {
        *response = msg;
    }

    return tokens;
}
