#ifndef __EVIAS_DOCBOT_BOOTSTRAP__
#define __EVIAS_DOCBOT_BOOTSTRAP__

/**
 * @file : bootstrap.hpp
 * @package :   eVias' docBot
 * @author :    Grégory Saive
 * @copyright : © 2012 Grégory Saive
 * @license :   GNU/GPL 3.0
 *
 * @brief
 * define a class for boostrapping the application run.
 **/

#include <Wt/WApplication>
#include <Wt/WContainerWidget>
#include <Wt/WPushButton>
#include <Wt/WString>

#include <Wt/Dbo/Types>

#include "models/model.hpp"
#include "bot/bot.hpp"

namespace evias {

    class bootstrap
    {
    public:
        bootstrap();
        virtual ~bootstrap();

    private:

        bool verbose_mode_;
        bool quiet_mode_;
    };

};

#endif /** __EVIAS_DOCBOT_BOOTSTRAP__ **/
