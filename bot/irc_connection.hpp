#ifndef __EVIAS_DOCBOT_IRC_CONNECTION__
#define __EVIAS_DOCBOT_IRC_CONNECTION_

#include "irc_traits.hpp"
#include "core/utilities.hpp"

#include <iostream>
#include <vector>

namespace evias {

    namespace irc_traits {
        struct irc_command_hook;
        struct irc_response;
        struct irc_conn_config;
        struct irc_user;
        struct connection;
    }

    namespace __t = irc_traits;
    namespace __u = utilities;

    class ircClient
    {
        public :
            ircClient();
            ~ircClient();

            // execution management
            void setQuietMode(bool);

            // irc command hooks (callbacks management)
            void unhook(__t::irc_command_hook*);
            void hook_cmd(char*, int (*) (char*, __t::irc_response*, void*));
            void callback(char*, char*, __t::irc_response*);

            // irc connection
            int  connect(__t::irc_conn_config);
            void quit();
            int  disconnect(std::string);
            bool is_connected();
            __t::connection get_connection();

            // data management
            int reply_loop(std::string="");
            void parse_response(char*);
            void split_response(char*);

            std::string get_last_treated();
            void        log(std::string);

        private :
            // hook private management. [internal use only]
            void _add_hook(__t::irc_command_hook*, char*, int (*) (char*, __t::irc_response*, void*));
            void _remote_config(__t::irc_conn_config);

            bool quiet_;
            __t::irc_conn_config config_;
            __t::connection      conn_;
            std::string          last_treated_;
            std::vector<std::string>  end_codes_;
            __t::irc_user*            chan_users_;
            __t::irc_command_hook*    hooks_;
    };

}

#endif
