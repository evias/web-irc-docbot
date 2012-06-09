#ifndef __EVIAS_DOCBOT_WEB_APPLICATION__
#define __EVIAS_DOCBOT_WEB_APPLICATION__

/**
 * @file : bootstrap.hpp
 * @package :   eVias' docBot web platform
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

#include "core/models/model.hpp"

namespace evias {

    class application
        : public Wt::WApplication
    {
    public:
        application(const Wt::WEnvironment&);
        ~application();
    };

};

#endif /** __EVIAS_DOCBOT_WEB_APPLICATION__ **/
