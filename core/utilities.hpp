#ifndef __EVIAS_DOCBOT_UTILITIES__
#define __EVIAS_DOCBOT_UTILITIES__

#include <string>
#include <vector>
#include <map>

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

};
};

#endif /** __EVIAS_DOCBOT_UTILITIES__ **/
