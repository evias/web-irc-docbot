#include "irc_connection.hpp"

#include <boost/regex.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/regex.hpp>
#include <sstream>

using namespace std;
using namespace evias;
using namespace irc_traits;

ircClient::ircClient( )
{
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

    g_operators_   = "\\-\\+=\\*/";
    g_punctuation_ = "\\.:,;\\?!";
    g_brackets_    = "\\(\\)\\[\\]\\{\\}";
    g_spaces_      = "\\n\\r\\0\\t ";
    g_special_     = "\\\\\"'#~%\\^&@\\$_";
    g_server_id_   = "[\\w0-9\\.]+";
    g_user_id_     = "[\\w0-9_`\\[\\]]+!~[\\w0-9\\.\\-_\\[\\]]+@[\\w0-9\\.\\-_\\[\\]]+";
    g_message_id_  = "[A-Z]+|[0-9]{3}";
    g_target_id_   = "[\\*]{1}|[\\w0-9_`\\[\\]]+";

    g_data_ = ":?[\\w" + g_operators_ + g_punctuation_ + g_brackets_ + g_spaces_ + g_special_ + "]+";

    r_server_response_ = "^(:" + g_server_id_ + ") (" + g_message_id_ + ") (" + g_target_id_ + ") (" + g_data_ + ")";
    r_server_query_    = "^(" + g_message_id_ + ") (" + g_data_ + ")";
    r_request_         = "^(:" + g_user_id_ + ") (" + g_message_id_ + ") (" + g_target_id_ + ") (" + g_data_ + ")";
    r_block_remainal_  = "^[^:](" + g_data_ + ")";
}

ircClient::~ircClient( )
{
    if (log_out_.is_open())
        log_out_.close();
}

void ircClient::log(string msg)
{
    if (quiet_)
        return ;

    if (! log_out_.is_open())
        log_out_.open(exec_log_file_.c_str(), ios::app);

    log_out_ << msg << endl;
}

void ircClient::log(string prepend, vector<string> lines)
{
    for (vector<string>::iterator l = lines.begin(); l != lines.end(); l++)
        log(prepend + *l);
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

    vector<string> end_codes;
    if (! end_msg.empty())
        boost::split_regex(end_codes, end_msg, boost::regex("\\|"));

    hostent* resolv;
    if (! config_.connected) {
        log("[ERROR] Connection not available.");
        return 1;
    }

    while (1) {
        ret_len = recv(conn_.irc_socket, buffer, 1023, 0 );

        if (ret_len == SOCKET_ERROR || ! ret_len)
            break;

        buffer[ret_len] = '\0';

        // replace CRLF for easier split algorithm.
        string buf = buffer;
        vector<string> lines;

        // split into lines
        boost::algorithm::split_regex(lines, buf, boost::regex("[\\n]+|\\r\\n"));

        stack_response(lines);

        if ((bool) end_msg.size()
            && ! (__u::vector_intersection<string>(treated_msgs_, end_codes)).empty())
            break;
    }

    return 0;
}

void ircClient::stack_response(vector<string> lines)
{
    boost::cmatch matches;
    boost::regex expr_server_response(r_server_response_, boost::regex::perl);
    boost::regex expr_server_query(r_server_query_, boost::regex::perl);
    boost::regex expr_request(r_request_, boost::regex::perl);
    boost::regex expr_block_remainal(r_block_remainal_, boost::regex::perl);

    string lost_data_buf;
    vector<string> stacked_lines;
    for (vector<string>::iterator line = lines.begin(); line != lines.end(); line++) {

        __t::message_data_t data;
        __t::message_type_t type;
        string              msg    = "";
        string              code   = "";
        string              server = "";
        string              target = "";

        code = "";
        // determine message type with regular expressions
        if (regex_match((*line).c_str(), matches, expr_server_response)) {
            server = matches[1];
            code   = matches[2];
            target = matches[3];
            msg    = matches[4];
            type   = TYPE_SERVER_RESPONSE;
        }
        else if (regex_match((*line).c_str(), matches, expr_server_query)) {
            code = matches[1];
            msg  = matches[2];
            type = TYPE_SERVER_QUERY;
        }
        else if (regex_match((*line).c_str(), matches, expr_request)) {
            server = matches[1];
            code   = matches[2];
            target = matches[3];
            msg    = matches[4];
            type   = TYPE_REQUEST;
        }
        else if (regex_match((*line).c_str(), matches, expr_block_remainal)) {
            msg  = __u::trim(*line, "\t\r\n");
            type = TYPE_BLOCK_REMAINAL;
        }
        else {
            msg  = __u::trim(*line, "\t\r\n");
            type = TYPE_UNKNOWN;
        }

        data.server  = server;
        data.code    = code;
        data.target  = target;
        data.message = msg;
        data.type    = type;

        stack_.push(data);
    }

    // log block of stacked lines
    process_stack();
}

void ircClient::process_stack()
{
    while (! stack_.empty()) {

        message_data_t current_msg = stack_.front();

        handle(current_msg);

        processed_.push_back(current_msg);
        stack_.pop();
    }
}

void ircClient::register_callback(string code, __t::callback<ircClient> cb)
{
    map<string, __t::callback<ircClient> >::iterator i_find = callbacks_.find(code);
    if (i_find != callbacks_.end())
        callbacks_.erase(i_find);

    callbacks_.insert(make_pair(code, cb));
}

int ircClient::handle(__t::message_data_t msg)
{
    if (msg.code.empty())
        return 0;

    treated_msgs_.push_back(msg.code);

    map<string, __t::callback<ircClient> >::iterator i_find = callbacks_.find(msg.code);
    if (i_find == callbacks_.end())
        return 1;

    // XXX arguments to callback execution
    __t::callback<ircClient> handler(i_find->second);
    if (handler.is_args0()) return handler();
    else if (handler.is_args1()) return handler("");
    else if (handler.is_args2()) return handler("", "");
    else if (handler.is_args3()) return handler("", "", "");
}

int ircClient::irc_pong()
{
    log("[DEBUG] sending PONG to '" + config_.server + "'");

    stringstream buf;
    buf << "PONG " << config_.server << " :"
        << "\r\n";

    send(get_connection().irc_socket, buf.str().c_str(), strlen(buf.str().c_str()), 0);
    return 0;
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


