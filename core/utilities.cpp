#include "utilities.hpp"

using namespace std;
using namespace evias;

string vector_join (vector<string> input, string delim)
{
    string output = "";
    vector<string>::const_iterator it = input.begin();
    for (int i = 0; it != input.end(); it++, i++)
        output = output + (i > 0 ? delim : "") + (*it);

    return output;
}

string map_to_json(map<string,string> input)
{
    string json = "";
    map<string,string>::const_iterator it = input.begin();
    for (int i = 0; it != input.end(); it++, i++)
        json = json + (i == 0 ? "{" : ",") + "'" + (*it).first + "'='" + (*it).second + "'";

    return json;
}

