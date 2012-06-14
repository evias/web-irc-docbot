#ifndef __EVIAS_DOCBOT_BOT_BOT__
#define __EVIAS_DOCBOT_BOT_BOT__

#include "irc_connection.hpp"
#include "commands.hpp"
#include "core/models/user.hpp"

#include <Wt/Dbo/Types>
#include <boost/shared_ptr.hpp>
#include <memory>

namespace evias {

    namespace __t = irc_traits;
    namespace __m = models;
    namespace dbo = Wt::Dbo;

    class bot
    {
        /** Singleton pattern. **/
    protected:
        bot();
        bot(bot*);

    private:
        bot(const bot&);
        const bot& operator=(const bot&);

    public:

        typedef dbo::collection<dbo::ptr<__m::user> > user_list_t;
        typedef dbo::ptr<__m::user>                   user_t;

        virtual ~bot();
        static bot& get();
        static bot* pget();

        void init();
        ircClient* get_irc();

    private:
        static bot*  instance_;

        boost::shared_ptr<ircClient> irc_;
        __t::irc_conn_config         config_;
        user_list_t                  authd_;
    };

}

#endif /** __EVIAS_DOCBOT_BOT_APPLICATION__ **/
