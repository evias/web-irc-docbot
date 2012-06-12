#include "irc_connection.hpp"

#include <boost/regex.hpp>
#include <boost/algorithm/string.hpp>
#include <sstream>

using namespace std;
using namespace evias;
using namespace irc_traits;

ircClient::ircClient( )
{
    hooks_      = 0;
    chan_users_ = 0;
    quiet_      = false;

    config_.connected = false;
    config_.nick_sent = false;
    config_.pass_sent = false;
    config_.user_sent = false;
    config_.auth_server = false;
    config_.nick    = "";
    config_.user    = "";
    config_.name    = "";
    config_.pass    = "";
    config_.server  = "";
    exec_log_file_  = "public/shared/logs/docbot_execution_log.txt";
    log_out_.open(exec_log_file_.c_str(), ios::app);
}

ircClient::~ircClient( )
{
    if (log_out_.is_open())
        log_out_.close();
    if (hooks_)
        unhook(hooks_);
}

void ircClient::log(string msg)
{
    if (quiet_)
        return ;

    if (! log_out_.is_open())
        log_out_.open(exec_log_file_.c_str(), ios::app);

    log_out_ << msg << endl;
}

/**
  * hooks [callbacks] management
  *
  **/

void ircClient::_add_hook(irc_command_hook* hook, char *cmd_name, int (*callback) (char*,irc_response*, void*))
{
    if (hook->callback) {
        if (! hook->next) {
            hook->next                = new irc_command_hook;
            hook->next->callback    = 0;
            hook->next->ircCommand    = 0;
            hook->next->next        = 0;
        }
        _add_hook(hook->next, cmd_name, callback);
    }
    else {
        hook->callback        = callback;
        hook->ircCommand    = new char[ strlen( cmd_name ) + 1 ];

        strcpy(hook->ircCommand, cmd_name);
    }
}

void ircClient::hook_cmd(char* cmd_name, int (*callback) (char *, irc_response*, void*))
{
    if (! hooks_) {
        hooks_             = new irc_command_hook;
        hooks_->callback   = 0;
        hooks_->ircCommand = 0;
        hooks_->next       = 0;
    }

    _add_hook(hooks_, cmd_name, callback);
}

void ircClient::unhook(irc_command_hook* cmd_hook)
{
    if (cmd_hook->next)
        // recursivity
        unhook(cmd_hook->next);

    if (cmd_hook->ircCommand)
        delete cmd_hook->ircCommand;

    delete cmd_hook;
    cmd_hook = NULL;
}

void ircClient::callback(char* irc_command, char* params, irc_response* hostd)
{
    irc_command_hook* p;

    if (! hooks_)
        // no callbacks available
        return;

    p = hooks_;
    while (p) {
        // filter command hooks
        if (strcmp(p->ircCommand, irc_command) == 0) {
            // execute callback
            (*(p->callback)) (params, hostd, this);
            p = 0;
        }
        else
            p = p->next;
    }
}

/**
 * connection management
 * the below implemented methods configure the IRC
 * connection through a socket. the data is sent in the
 * connect() method [XXX IDENTD ssh auth].
 *
 **/

void ircClient::_remote_config(irc_conn_config conf)
{
    config_.server = conf.server;
    config_.port   = conf.port;
    config_.nick   = conf.nick;
    config_.user   = conf.user;
    config_.name   = conf.name;
    config_.pass   = conf.pass;

    // @todo : IDENTD
    //  @brief : use second socket (one for tcp_connect:6667, one for tcp_listen:22)
    conn_.irc_socket   = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    conn_.local_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    if (conn_.irc_socket == INVALID_SOCKET || conn_.local_socket == INVALID_SOCKET) {
        log("Invalid Socket error");
        return;
    }

    // resolve hostname
    conn_.host = gethostbyname(config_.server.c_str());
    if (! conn_.host) {
        closesocket(conn_.irc_socket);
        closesocket(conn_.local_socket);
        log(string("Invalid hostname '").append(config_.server));
        return;
    }

    memcpy(&conn_.remote.sin_addr, conn_.host->h_addr, 4);

    conn_.remote.sin_family = AF_INET;
    conn_.remote.sin_port   = htons(config_.port);
}

int ircClient::connect(irc_conn_config conf)
{
    _remote_config(conf);

    if ((::connect(conn_.irc_socket, (const sockaddr*)& conn_.remote, sizeof(conn_.remote))
        == SOCKET_ERROR)) {

        log ("[ERROR] Could not open listening connection");

        closesocket(conn_.irc_socket);
        closesocket(conn_.local_socket);
        return 1;
    }

    stringstream buf;
    buf << "NICK " << config_.nick.c_str()
        << "\r\n"
        << "USER " << config_.user.c_str()
            << " * 0 : " << config_.name.c_str()
        << "\r\n";

    if (config_.auth_server)
        buf << "PASS " << config_.pass.c_str()
            << "\r\n";

    // send nick, user & name
    send(conn_.irc_socket, buf.str().c_str(), strlen(buf.str().c_str()), 0);

    log("[DEBUG] Connection established.");

    config_.connected = true;
    return 0;
}

void ircClient::quit( )
{
    if (config_.connected) {
        log ("Disconnected from server");

        config_.connected = false;
        disconnect((char*) "eVias docBot");

        closesocket(conn_.irc_socket);
    }
}

int ircClient::disconnect(string quitmsg)
{
    if (config_.connected) {
        stringstream buf;
        buf << "QUIT ";
        if (! quitmsg.empty())
            buf << quitmsg;
        buf << "\r\n";

        if (send(conn_.irc_socket, buf.str().c_str(), strlen(buf.str().c_str()), 0)
                == SOCKET_ERROR)
            return 1;
    }
    return 0;
}

bool ircClient::is_connected()
{
    return config_.connected;
}

connection ircClient::get_connection()
{
    return conn_;
}

/**
 * data management, below implemented member functions handle
 * IRC response data. It is split into chunks and a loop is
 * implemented for catching IRC server responses.
 */

int ircClient::reply_loop(string end_msg)
{
    stringstream buf;
    sockaddr_in cli;
    int  cli_size;
    int  cli_ret;
    int  ret_len;
    char buffer[1024];

    if (! end_msg.empty())
        boost::split(end_codes_, end_msg, boost::is_any_of("|"));

    hostent* resolv;
    if (! config_.connected) {
        log("[ERROR] Connection not available.");
        return 1;
    }

    while (1) {
        ret_len = recv(conn_.irc_socket, buffer, 1023, 0 );

        if (ret_len == SOCKET_ERROR || ! ret_len)
            return 1;

        buffer[ret_len] = '\0';

        string buf = buffer;
        do {
            boost::replace_first(buf, "\r\n", "§");
        }
        while(buf.find("\r\n") != string::npos);

        vector<string> lines;
        boost::split(lines, buffer, boost::algorithm::is_any_of("§"));

        process_response(lines);

        if ((bool) end_msg.size()
            && __u::in_vector<string>(last_treated_, end_codes_))
            return 0;
    }

    return 0;
}

void ircClient::process_response(vector<string> lines)
{
    string reg_server   = "^(\\:[a-zA-Z0-9\\.]+)";
    string reg_response = reg_server + " ([A-Z0-9]+) ([A-Za-z0-9\\._`]+) \\:([\\.+])\\\\r\\\\n$";

    boost::cmatch matches;
    boost::regex expr(reg_response);

    for (vector<string>::iterator line = lines.begin(); line != lines.end(); line++) {
        if (regex_match((*line).c_str(), matches, expr)) {
            string server, code, target, data;
            server.assign(matches[1].first, matches[1].second);
            code.assign(matches[2].first, matches[2].second);
            target.assign(matches[3].first, matches[3].second);
            data.assign(matches[4].first, matches[4].second);

            stringstream l;
            l << "{server:'" << server << "';"
              << "message:'" << code << "';"
              << "target:'" << target << "';"
              << "data:'" << data << "'};"
              << endl;

            log("[DATA] " + l.str());
        }
        else {
            log("[RAWDATA]" + *line);
        }
    }
}

int ircClient::irc_notice(string target, string msg)
{
    stringstream buf;
    buf << "NOTICE " << target << " :" << msg
        << "\r\n";

    send(get_connection().irc_socket, buf.str().c_str(), strlen(buf.str().c_str()), 0);
    return 0;
}

int ircClient::irc_privmsg(string target, string msg)
{
    stringstream buf;
    buf << "PRIVMSG " << target << " :" << msg
        << "\r\n";

    send(get_connection().irc_socket, buf.str().c_str(), strlen(buf.str().c_str()), 0);
    return 0;
}

int ircClient::irc_nick(string newnick)
{
    stringstream buf;
    buf << "NICK " << newnick
        << "\r\n";

    send(get_connection().irc_socket, buf.str().c_str(), strlen(buf.str().c_str()), 0);
    return 0;
}


int ircClient::irc_channel_isop(string chan, string nick)
{
    __t::irc_user* user;
    user = chan_users_;

    while (user) {
        if (!strcmp(user->channel, chan.c_str()) && !strcmp( user->nick, nick.c_str()))
            return user->flags & IRC_USER_OP;

        user = user->next;
    }
    return 0;
}

int ircClient::irc_channel_isvoice(string chan, string nick)
{
    __t::irc_user* user;
    user = chan_users_;

    while (user) {
        if (!strcmp(user->channel, chan.c_str()) && !strcmp(user->nick, nick.c_str()))
            return user->flags&IRC_USER_VOICE;

        user = user->next;
    }
    return 0;
}

int ircClient::irc_channel_join(string chan)
{
    stringstream buf;
    buf << "JOIN " << chan
        << "\r\n";

    send(get_connection().irc_socket, buf.str().c_str(), strlen(buf.str().c_str()), 0);
    return 0;
}

int ircClient::irc_channel_part(string chan)
{
    stringstream buf;
    buf << "PART " << chan
        << "\r\n";

    send(get_connection().irc_socket, buf.str().c_str(), strlen(buf.str().c_str()), 0);
    return 0;
}

int ircClient::irc_channel_kick(string chan, string nick, string msg)
{
    stringstream buf;
    buf << "KICK " << chan << " " << nick;
    if (! msg.empty())
        buf << " :" << msg;

    buf << "\r\n";

    send(get_connection().irc_socket, buf.str().c_str(), strlen(buf.str().c_str()), 0);
    return 0;
}

int ircClient::irc_channel_mode(string chan, string modes, string targets)
{
    stringstream buf;
    buf << "MODE " << chan << " " << modes;
    if (! targets.empty())
        buf << " " << targets;

    buf << "\r\n";

    send(get_connection().irc_socket, buf.str().c_str(), strlen(buf.str().c_str()), 0);
    return 0;
}


