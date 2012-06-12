#ifndef __EVIAS_DOCBOT_BOT_APPLICATION__
#define __EVIAS_DOCBOT_BOT_APPLICATION__

#include <Wt/Dbo/Types>
#include <Wt/WApplication>

namespace evias {
namespace irc {

    class application
        : public Wt::WApplication
    {
    public:
        application(const Wt::WEnvironment&);
        ~application();
    };
}
}

#endif
