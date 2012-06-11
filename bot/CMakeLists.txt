ADD_LIBRARY(botLib
irc_connection.cpp
documentor.cpp
)
TARGET_LINK_LIBRARIES(botLib boost_signals coreLib)
