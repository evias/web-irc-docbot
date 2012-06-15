#ifndef __EVIAS_DOCBOT_IRC_TRAITS__
#define __EVIAS_DOCBOT_IRC_TRAITS__

#include <unistd.h>
#include <errno.h>
#include <cstring>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>

#define closesocket( s ) close( s )
#define SOCKET_ERROR    -1
#define INVALID_SOCKET    -1

#include <cstdio>
#include <cstdlib>

#include <string>
#include <queue>
#include <set>
#include <algorithm>
#include <sstream>
#include <stdexcept>

#define IRC_DEBUG 1

namespace evias {
namespace irc_traits {

    using std::string;
    using std::queue;
    using std::set;
    using std::vector;
    using std::stringstream;

    const unsigned int IRC_USER_VOICE   = 1;
    const unsigned int IRC_USER_HALFOP  = 2;
    const unsigned int IRC_USER_OP      = 4;

    /** messages **/
    const string MSG_CHANPART = "PART";
    const string MSG_PRIVMSG  = "PRIVMSG";
    /** responses **/
    const string MSG_ENDMOTD = "376";
    const string MSG_ENDLIST = "366";
    /** ERRORS **/
    const string MSG_NOTONCHAN   = "442";
    const string MSG_INVALIDCHAN = "403";

    struct irc_conn_config {
        bool connected;
        bool nick_sent;
        bool pass_sent;
        bool user_sent;
        bool auth_server;
        int    port;
        string server;
        string nick;
        string user;
        string name;
        string pass;

        static irc_conn_config create(string s, int p, string n, string u, string na, string pa = "")
        {
            irc_conn_config conf;
            conf.server = s;
            conf.port   = p;
            conf.nick   = n;
            conf.user   = u;
            conf.name   = na;
            conf.pass   = pa;
            conf.connected   = false;
            conf.nick_sent   = false;
            conf.pass_sent   = false;
            conf.user_sent   = false;
            conf.auth_server = false;
            return conf;
        };
    };

    struct connection {
        int irc_socket;
        int local_socket;
        sockaddr_in remote;
        hostent*    host;
    };

    // people ..
    struct irc_user {
        char        *nick;
        char        *channel;
        char        flags;
        irc_user    *next;
    };

    enum message_type_t {
        TYPE_BLOCK_REMAINAL  = 0,
        TYPE_SERVER_RESPONSE = 1,
        TYPE_SERVER_QUERY    = 2,
        TYPE_REQUEST         = 3,
        TYPE_UNKNOWN         = 4
    };

    struct message_data_t {
        // is unknown means must be appended [msg-1 was truncated]
        string  server;
        string  code;
        string  target;
        string  message;
        message_type_t type;
    };

    struct message_stack_t {
        typedef vector<message_data_t> data_t;

        data_t  stack;

        void push(message_data_t d)
            { stack.push_back(d); }
        void pop()
            { stack.erase(stack.begin()); }
        message_data_t front()
            { return *(stack.begin()); }
        bool empty()
            { return stack.empty(); }

        operator vector<string>()
        {
            vector<string> output;
            vector<message_data_t>::iterator it = stack.begin();
            for (int i = 0; i < stack.size(); i++, it++) {
                message_data_t d = *it;
                output.push_back("[" + d.code + "] '" + d.message + "'");
            }
            return output;
        }
    };

    template <typename __class>
    class callback
    {
    public:
        typedef int (__class::*callback0_t) ();
        typedef int (__class::*callback1_t) (string);
        typedef int (__class::*callback2_t) (string, string);
        typedef int (__class::*callback3_t) (string, string, string);

        callback(__class* c, int (__class::*cb)())
            : callee_(c), args1_callback_(NULL), args2_callback_(NULL), args3_callback_(NULL)
        {
            args0_callback_ = cb;
        };

        callback(__class* c, int (__class::*cb)(string))
            : callee_(c), args0_callback_(NULL), args2_callback_(NULL), args3_callback_(NULL)
        {
            args1_callback_ = cb;
        };

        callback(__class* c, int (__class::*cb)(string,string))
            : callee_(c), args0_callback_(NULL), args1_callback_(NULL), args3_callback_(NULL)
        {
            args2_callback_ = cb;
        };

        callback(__class* c, int (__class::*cb)(string,string,string))
            : callee_(c), args0_callback_(NULL), args1_callback_(NULL), args2_callback_(NULL)
        {
            args3_callback_ = cb;
        };

        callback(const callback& cb)
            : args0_callback_(cb.args0_callback_), args1_callback_(cb.args1_callback_),
              args2_callback_(cb.args2_callback_), args3_callback_(cb.args3_callback_),
              callee_(cb.callee_)
        {
        };

        virtual ~callback() {};

        bool is_args0()
            { return args0_callback_ != NULL; }
        bool is_args1()
            { return args1_callback_ != NULL; }
        bool is_args2()
            { return args2_callback_ != NULL; }
        bool is_args3()
            { return args3_callback_ != NULL; }

        int operator()()
        {
            if (args0_callback_ == NULL)
                throw std::logic_error("Wrong callback::operator() called.");

            return (callee_->*args0_callback_)();
        };

        int operator()(std::string a1)
        {
            if (args1_callback_ == NULL)
                throw std::logic_error("Wrong callback::operator() called.");

            return (callee_->*args1_callback_)(a1);
        };

        int operator()(std::string a1, std::string a2)
        {
            if (args2_callback_ == NULL)
                throw std::logic_error("Wrong callback::operator() called.");

            return (callee_->*args2_callback_)(a1, a2);
        };

        int operator()(std::string a1, std::string a2, std::string a3)
        {
            if (args3_callback_ == NULL)
                throw std::logic_error("Wrong callback::operator() called.");

            return (callee_->*args3_callback_)(a1, a2, a3);
        };

    private:
        __class*    callee_;

        int (__class::*args0_callback_) ();
        int (__class::*args1_callback_) (std::string);
        int (__class::*args2_callback_) (std::string, std::string);
        int (__class::*args3_callback_) (std::string, std::string, std::string);

    };

}
}

#endif /** __EVIAS_DOCBOT_IRC_TRAITS__ **/
