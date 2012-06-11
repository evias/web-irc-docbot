#include "documentor.hpp"

using namespace std;
using namespace evias;

documentor* documentor::instance_ = NULL;

documentor::documentor()
{
    config_.server = "irc.freenode.net";
    config_.port   = 6667;
    config_.nick   = "eVias_docBot";
    config_.user   = "noName";
    config_.name   = "developing@eVias.be";

    irc_ = boost::shared_ptr<ircClient>(new ircClient());
    instance_ = (documentor*)(this);
}

documentor::documentor(documentor* p)
{
    if (instance_)
        throw std::exception();

   instance_ = p;
}

documentor::~documentor()
{
    irc_.reset();
    instance_ = NULL;
}

documentor& documentor::get()
{
    return *instance_;
}

documentor* documentor::pget()
{
    return instance_;
}

void documentor::loop()
{
    irc_.get()->connect(config_);
    irc_.get()->reply_loop(__t::MSG_ENDMOTD);
}

ircClient* documentor::get_irc()
{
    return irc_.get();
}



