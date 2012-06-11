/**
 * @file :      main.cpp
 * @package :   eVias' docBot
 * @author :    Grégory Saive
 * @copyright : © 2012 Grégory Saive
 * @license :   GNU/GPL 3.0
 *
 * @brief
 * application entry point.
 **/

#include "core/bootstrap.hpp"
#include "web/application.hpp"
#include "bot/bot.hpp"

Wt::WApplication *createApplication(const Wt::WEnvironment& env)
{
    return new evias::application(env);
}

void* start_irc_thread(void* data)
{
    evias::bot::get()
        .init();

    return data;
}


int main(int argc, char **argv)
{
    using evias::bootstrap;
    bootstrap* b = new bootstrap();

    pthread_t thread;
    pthread_create(&thread, NULL, start_irc_thread, NULL);
    pthread_join(thread, NULL);

    return Wt::WRun(argc, argv, &createApplication);
}
