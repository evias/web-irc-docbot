/**
 * @file : bootstrap.cpp
 * @package :   eVias' docBot
 * @author :    Grégory Saive
 * @copyright : © 2012 Grégory Saive
 * @license :   GNU/GPL 3.0
 *
 * @brief
 * implementation of the class application, bootstrap the docBot
 * application run. This class configures the session of the user.
 * No matter if the user is a logged in user or not, there will be
 * one instance of this class available for him/her.
 **/

#include "application.hpp"

#include <Wt/Dbo/backend/Postgres>
#include <Wt/Dbo/Session>

#include "core/models/user_traits.hpp"

using namespace evias;
using namespace evias::models;

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

    view_ = evias::interface::create(root());
}

application::~application()
{
    model::destroy();
}

