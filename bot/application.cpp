#include "application.hpp"
#include "bot/bot.hpp"

#include <Wt/Dbo/Impl>

using namespace evias::models;
using namespace evias;
using namespace irc;

application::application(const Wt::WEnvironment& env)
    : Wt::WApplication(env)
{
    // initialize models and configure models mapping.
    // XXX move to bootstrap
    model::create("host='localhost' user='developing' password='opendev' dbname='dbdoc'");
    model::db_session = new dbo::Session();
    model::getSession().setConnection(model::getAdapter());
    model::getSession().mapClass<user>("user");
    model::getSession().mapClass<user_traits::account>("user_account");
    model::getSession().mapClass<user_traits::profile>("user_profile");
    model::getSession().mapClass<user_traits::prefs>("user_prefs");
    model::getSession().mapClass<account_traits::account_type>("account_type");

    setTitle("docBot");
}

application::~application()
{
    model::destroy();
}

