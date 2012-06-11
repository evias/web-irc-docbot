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
#include <algorithm>
#include <sstream>

#define IRC_DEBUG 1

namespace evias {
namespace irc_traits {

    using std::string;
    using std::stringstream;

    const unsigned int IRC_USER_VOICE   = 1;
    const unsigned int IRC_USER_HALFOP  = 2;
    const unsigned int IRC_USER_OP      = 4;

    /** messages **/
    const string MSG_CHANPART = "PART";
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

    struct irc_response {
        char *nick;
        char *ident;
        char *host;
        char *target;
    };

    // command catch feature
    struct irc_command_hook {
        char                *ircCommand;
        irc_command_hook    *next;
        // pointer to function
        int (*callback) (char*, irc_response*, void*);
    };

    // people ..
    struct irc_user {
        char        *nick;
        char        *channel;
        char        flags;
        irc_user    *next;
    };

}
}

#endif /** __EVIAS_DOCBOT_IRC_TRAITS__ **/
