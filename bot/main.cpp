/**
 * @file :      main.cpp
 * @package :   eVias' docBot
 * @author :    Grégory Saive
 * @copyright : © 2012 Grégory Saive
 * @license :   GNU/GPL 3.0
 *
 * @brief
 * bot entry point.
 **/

#include "application.hpp"
#include "bot.hpp"

Wt::WApplication *createApplication(const Wt::WEnvironment& env)
{
    return new evias::irc::application(env);
}

int main(int argc, char **argv)
{
    evias::bot::get()
        .init();

    evias::bot::get()
        .listen();

    return Wt::WRun(argc, argv, &createApplication);
}
