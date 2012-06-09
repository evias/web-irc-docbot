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

Wt::WApplication *createApplication(const Wt::WEnvironment& env)
{
    return new evias::application(env);
}

int main(int argc, char **argv)
{
    using evias::bootstrap;
    bootstrap* b = new bootstrap();

    return Wt::WRun(argc, argv, &createApplication);
}
