#ifndef __EVIAS_DOCBOT_IRC_CONNECTION__
#define __EVIAS_DOCBOT_IRC_CONNECTION_

#include "irc_traits.hpp"
#include "core/utilities.hpp"

#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <map>

namespace evias {

    namespace irc_traits {
        struct irc_command_hook;
        struct irc_response;
        struct irc_conn_config;
        struct irc_user;
        struct connection;
        struct message_data_t;
        enum message_type_t;

        template <typename __class>
        class callback;
    }

    namespace __t = irc_traits;
    namespace __u = utilities;

    class ircClient
    {
    public :

        ircClient();
        ~ircClient();

        int  handle(__t::message_data_t);
        void register_callback(std::string, __t::callback<ircClient>);

        // execution management
        void setQuietMode(bool);

        // irc connection
        int  connect(__t::irc_conn_config);
        void quit();
        int  disconnect(std::string);
        bool is_connected();
        __t::connection get_connection();

        // data management
        int reply_loop(std::string="");
        void stack_response(std::vector<std::string>);
        void process_stack();
        std::pair<bool,std::string> stack_message(__t::message_type_t, std::string);

        std::string get_last_treated();
        void        log(std::string);
        void        log(std::string, std::vector<std::string>);

        // IRC protocol implementation
        int irc_pong();
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
        void _remote_config(__t::irc_conn_config);

        bool            quiet_;
        std::ofstream   log_out_;
        std::string     exec_log_file_;

        __t::irc_user*          chan_users_;
        __t::irc_conn_config    config_;
        __t::connection         conn_;

        std::vector<std::string> treated_msgs_;
        __t::message_stack_t     stack_;
        std::vector<__t::message_data_t>        processed_;

        std::map<std::string, __t::callback<ircClient> >   callbacks_;

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
