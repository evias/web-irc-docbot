#ifndef __EVIAS_DOCBOT_IRC_CONNECTION__
#define __EVIAS_DOCBOT_IRC_CONNECTION_

#include "irc_traits.hpp"
#include "core/utilities.hpp"

#include <iostream>
#include <fstream>
#include <vector>
#include <map>

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
            typedef std::map<__t::message_type_t, std::vector<std::string> > message_stack_t;
            typedef std::pair<__t::message_type_t, std::string >             stack_entry_t;

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
            void process_response(std::vector<std::string>);
            void process_stack();
            std::pair<bool,std::string> stack_message(__t::message_type_t, std::string);

            std::string get_last_treated();
            void        log(std::string);
            void        log(std::string, std::vector<std::string>);

            // IRC protocol implementation
            int irc_notice(std::string, std::string);
            int irc_privmsg(std::string, std::string);
            int irc_nick(std::string);
            int irc_channel_isop(std::string, std::string);
            int irc_channel_isvoice(std::string, std::string);
            int irc_channel_join(std::string);
            int irc_channel_part(std::string);
            int irc_channel_kick(std::string, std::string, std::string = "");
            int irc_channel_mode(std::string, std::string, std::string = "");

        private :
            // hook private management. [internal use only]
            void _add_hook(__t::irc_command_hook*, char*, int (*) (char*, __t::irc_response*, void*));
            void _remote_config(__t::irc_conn_config);

            bool            quiet_;
            std::ofstream   log_out_;
            std::string     exec_log_file_;
            std::string     last_treated_msg_;

            __t::irc_user*          chan_users_;
            __t::irc_conn_config    config_;
            __t::connection         conn_;
            __t::irc_command_hook*  hooks_;

            message_stack_t stack_;
            stack_entry_t   last_stacked_;

            std::string g_operators_;
            std::string g_punctuation_;
            std::string g_brackets_;
            std::string g_spaces_;
            std::string g_special_;
            std::string g_server_id_;
            std::string g_user_id_;
            std::string g_message_id_;
            std::string g_target_id_;
            std::string g_data_;

            std::string r_server_response_;
            std::string r_server_query_;
            std::string r_request_;
            std::string r_block_remainal_;
    };

}

#endif
