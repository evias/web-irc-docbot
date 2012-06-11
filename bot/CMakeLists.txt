ADD_LIBRARY(botLib
irc_connection.cpp
bot.cpp
)
TARGET_LINK_LIBRARIES(botLib boost_signals coreLib)
