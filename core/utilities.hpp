#ifndef __EVIAS_DOCBOT_UTILITIES__
#define __EVIAS_DOCBOT_UTILITIES__

#include <string>
#include <vector>
#include <map>

#define mem_rm(object) { delete object; object = NULL; }

namespace evias {
namespace utilities {

    std::string vector_join (std::vector<std::string>, std::string = " ");
    std::string map_to_json (std::map<std::string, std::string>);

};
};

#endif /** __EVIAS_DOCBOT_UTILITIES__ **/
