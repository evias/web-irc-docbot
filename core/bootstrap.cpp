/**
 * @file : bootstrap.cpp
 * @package :   eVias' docBot
 * @author :    Grégory Saive
 * @copyright : © 2012 Grégory Saive
 * @license :   GNU/GPL 3.0
 *
 * @brief
 * implementation of the class bootstrap, bootstrap the docBot
 * application run. This class represents the application bootstrap
 * It consist in configuring the IRC bot, connecting it and opening
 * the web platform user session handler.
 **/

#include "bootstrap.hpp"
#include "bot/documentor.hpp"

#include <cctype>
#include <cstdio>
#include <cstdlib>
#include <unistd.h>
#include <iostream>
#include <pthread.h>

using namespace std;
using namespace evias;

bootstrap::bootstrap()
    : quiet_mode_(false), verbose_mode_(true)
{
    // initialize bot
    documentor::get();
}

bootstrap::~bootstrap()
{
}

