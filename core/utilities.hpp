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

    std::string vector_join (std::vector<std::string>, std::string = " ");
    std::string map_to_json (std::map<std::string, std::string>);

    template <typename T>
    inline bool in_vector(T f, std::vector<T> v)
    {
        typename std::vector<T>::const_iterator i;
        for (i = v.begin(); i != v.end(); i++)
            if (*i == f)
                return true;
        return false;
    }

    inline std::string trim(const std::string& s, std::string sz = " \n")
    {
        using std::string;

        string out = s;
        string::size_type pos  = 0;

        do out = out.replace(pos, 1, "");
        while ((pos = out.find_last_of(sz)) != string::npos
               && pos == out.size()-1);

        return out;
    }

};
};

#endif /** __EVIAS_DOCBOT_UTILITIES__ **/
