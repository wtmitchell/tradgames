#ifndef STRING_H
#define STRING_H

#include <algorithm>
using std::find_if;
#include <functional>
using std::not1;
using std::ptr_fun;
//#include <cctype>
#include <locale>
using std::isspace;
#include <sstream>
using std::stringstream;
#include <string>
using std::string;
#include <vector>
using std::vector;

namespace String
{
    // Trim from left side / start
    inline string& ltrim(string &s)
    {
        s.erase(s.begin(), find_if(s.begin(), s.end(),
                                   not1(ptr_fun<int, int>(isspace))));
        return s;
    }

    // Trim from right side / end
    inline string& rtrim(string &s)
    {
        s.erase(find_if(s.rbegin(), s.rend(),
                        not1(ptr_fun<int, int>(isspace))).base(), s.end());
        return s;
    }

    // Trim from both ends
    inline string& trim(string &s) {
        return ltrim(rtrim(s));
    }

    // Split based on delimiter
    inline vector<string> split(const string &s, char delim) {
        vector<string> tokens;
        stringstream ss(s);
        string item;
        while(std::getline(ss, item, delim)) {
            tokens.push_back(item);
        }
        return tokens;
    }

    inline int stoi(std::string str) {
        return atoi(str.c_str());
    }
}
#endif
