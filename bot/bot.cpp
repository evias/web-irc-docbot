#include "bot.hpp"

using namespace std;
using namespace evias;

bot* bot::instance_ = NULL;

bot::bot()
{
    config_ = __t::irc_conn_config::create(
        "irc.freenode.net",
        6667,
        "eVias_docBot",
        "noName",
        "developing@eVias.be");

    irc_ = boost::shared_ptr<ircClient>(new ircClient());
}

bot::bot(bot* p)
{
    if (instance_)
        throw std::logic_error(string("Singleton instance already created."));

   instance_ = p;
}

bot::~bot()
{
    irc_.reset();
    instance_ = NULL;
}

bot& bot::get()
{
    return *(pget());
}

bot* bot::pget()
{
    if (! instance_)
        instance_ = new bot;

    return instance_;
}

void bot::init()
{
    // connect
    irc_.get()->connect(config_);
    irc_.get()->reply_loop(__t::MSG_ENDMOTD);

    // join default channel
    irc_.get()->irc_channel_join("#evias.be");
    irc_.get()->reply_loop(__t::MSG_ENDLIST + "|" + __t::MSG_INVALIDCHAN);
}

ircClient* bot::get_irc()
{
    return irc_.get();
}



