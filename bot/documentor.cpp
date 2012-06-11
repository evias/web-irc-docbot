#include "documentor.hpp"

using namespace std;
using namespace evias;

documentor* documentor::instance_ = NULL;

documentor::documentor()
{
    config_ = __t::irc_conn_config::create(
        "irc.freenode.net",
        6667,
        "eVias_docBot",
        "noName",
        "developing@eVias.be");

    irc_ = boost::shared_ptr<ircClient>(new ircClient());
}

documentor::documentor(documentor* p)
{
    if (instance_)
        throw std::logic_error(string("Singleton instance already created."));

   instance_ = p;
}

documentor::~documentor()
{
    irc_.reset();
    instance_ = NULL;
}

documentor& documentor::get()
{
    return *(pget());
}

documentor* documentor::pget()
{
    if (! instance_)
        instance_ = new documentor;

    return instance_;
}

void documentor::init_bot_features()
{
    irc_.get()->connect(config_);
    irc_.get()->reply_loop(__t::MSG_ENDMOTD);

    irc_.get()->irc_channel_join("#evias.be");
}

ircClient* documentor::get_irc()
{
    return irc_.get();
}



