#ifndef __EVIAS_DOCBOT_UTILITIES__
#define __EVIAS_DOCBOT_UTILITIES__

#include <string>
#include <vector>
#include <map>
#include <algorithm>

#include <boost/algorithm/string.hpp>

#define mem_rm(object) { delete object; object = NULL; }

namespace evias {
namespace utilities {

    using std::string;
    using std::vector;
    using std::map;

    inline string vector_join (vector<string> input, string delim = " ")
    {
        string output = "";
        vector<string>::const_iterator it = input.begin();
        for (int i = 0; it != input.end(); it++, i++)
            output = output + (i > 0 ? delim : "") + (*it);

        return output;
    }

    inline string map_to_json(map<string,string> input)
    {
        string json = "";
        map<string,string>::const_iterator it = input.begin();
        for (int i = 0; it != input.end(); it++, i++)
            json = json + (i == 0 ? "{" : ",") + "'" + (*it).first + "'='" + (*it).second + "'";

        return json;
    }

    template <typename T>
    inline bool in_vector(T f, vector<T> v)
    {
        typename vector<T>::const_iterator i;
        for (i = v.begin(); i != v.end(); i++)
            if (*i == f)
                return true;
        return false;
    }

    inline string ltrim(const string& s, string sz = " \t\r\n")
    {
        string out = s;
        string::size_type pos = -1;

        do if (pos == 0)
            out = out.replace(0, 1, "");
        while ((pos = out.find_first_of(sz)) != string::npos
               && pos == 0);

        return out;
    }

    inline string rtrim(const string& s, string sz = " \t\r\n")
    {
        string out = s;
        string::size_type pos = 0;

        do if (pos > 0)
            out = out.replace(pos, 1, "");
        while ((pos = out.find_last_of(sz)) != string::npos
               && pos == out.size()-1);

        return out;
    }

    inline string trim(const string &s, string sz = " \t\r\n")
    {
        string out = ltrim(rtrim(s, sz), sz);
        return out;
    }

    template <typename first_t, typename second_t>
    inline map<first_t, second_t> map_intersect_key(map<first_t, second_t> m1, map<first_t, second_t> m2)
    {
        map<first_t, second_t> intersection;
        typename map<first_t, second_t>::iterator it_keys   = m1.begin();
        typename map<first_t, second_t>::iterator it_values = m1.begin();
        for (; it_keys != m1.end(); it_keys++)
            if ((it_values = m2.find(it_keys->first)) != m2.end())
                // keys from map1, values from map2
                intersection.insert(make_pair(it_keys->first, it_values->second));

        return intersection;
    }

    template <typename T>
    inline vector<T> vector_intersection(vector<T> v1, vector<T> v2)
    {
        typename vector<T>::iterator first  = v1.begin();
        typename vector<T>::iterator second = v2.begin();

        int s1 = v1.size();
        int s2 = v2.size();

        vector<T> intersection;
        for (; first != v1.end(); first++) {
            for (second = v2.begin(); second != v2.end(); second++) {
                if (*first == *second) {
                    intersection.push_back(*first);
                    break;
                }
            }
        }

        return intersection;
    }

};
};

#endif /** __EVIAS_DOCBOT_UTILITIES__ **/
