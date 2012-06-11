#ifndef __EVIAS_DOCBOT_BOT_COMMANDS__
#define __EVIAS_DOCBOT_BOT_COMMANDS__

#include "core/models/user.hpp"

#include <Wt/Dbo/Types>
#include <string>

namespace evias {

    namespace dbo = Wt::Dbo;
    namespace __m = models;

    struct abstract_command {
        virtual bool isset() = 0;
    };

    struct bot_user_auth : abstract_command {
        dbo::ptr<__m::user> user_;

        bool operator()(std::string, std::string);
        bool isset();
    };

}

#endif /** __EVIAS_DOCBOT_BOT_COMMANDS__ **/

