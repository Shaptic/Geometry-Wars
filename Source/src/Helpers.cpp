#include <sstream>

#include "Helpers.h"

using namespace std;

int num_len(const int number)
{
    int tmp     = number;
    int count   = 0;

    while(tmp > 0)
    {
        tmp = tmp / 10;
        count++;
    }
    return count;
}

void toupper(string& str)
{
    for(unsigned int i=0; i < str.length(); i++)
        str[i] = toupper(str[i]);
}

string toupper_ret(const string& str)
{
    string tmp(str);
    for(unsigned int i=0; i < str.length(); i++)
        tmp[i] = toupper(str[i]);
    return tmp;
}

string combine(const string& str1, const char* str2)
{
    stringstream ss;
    ss << str1 << str2;
    return ss.str();
}

vector<string> split(const string& str, char token)
{
    string tmp(str);
    stringstream ss;
    vector<string> results;
    int index;

    while(true)
    {
        index = tmp.find(token);
        if(index == -1 || index == string::npos)
            break;

        results.push_back(tmp.substr(0, index));
        tmp = tmp.substr(index + 1, -1);
    }
    results.push_back(tmp);

    return results;
}