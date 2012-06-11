#ifndef __EVIAS_DOCBOT_BOT_DOCUMENTOR__
#define __EVIAS_DOCBOT_BOT_DOCUMENTOR__

#include "irc_connection.hpp"
#include <boost/shared_ptr.hpp>
#include <memory>

namespace evias {

    namespace __t = irc_traits;

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
        virtual ~bot();
        static bot& get();
        static bot* pget();

        void init();
        ircClient* get_irc();

    private:
        static bot*  instance_;

        boost::shared_ptr<ircClient> irc_;
        __t::irc_conn_config         config_;
    };

}

#endif /** __EVIAS_DOCBOT_BOT_APPLICATION__ **/
