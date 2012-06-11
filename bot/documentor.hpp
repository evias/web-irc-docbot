#ifndef __EVIAS_DOCBOT_BOT_DOCUMENTOR__
#define __EVIAS_DOCBOT_BOT_DOCUMENTOR__

#include "irc_connection.hpp"
#include <boost/shared_ptr.hpp>
#include <memory>

namespace evias {

    namespace __t = irc_traits;

    class documentor
    {
        /** Singleton pattern. **/
    protected:
        documentor();
        documentor(documentor*);

    private:
        documentor(const documentor&);
        const documentor& operator=(const documentor&);

    public:
        virtual ~documentor();
        static documentor& get();
        static documentor* pget();

        void init_bot_features();
        ircClient* get_irc();

    private:
        static documentor*  instance_;

        boost::shared_ptr<ircClient> irc_;
        __t::irc_conn_config         config_;
    };

}

#endif /** __EVIAS_DOCBOT_BOT_APPLICATION__ **/
