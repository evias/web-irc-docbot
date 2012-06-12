ADD_EXECUTABLE(../public/docBot.wt
irc_connection.cpp
commands.cpp
bot.cpp
application.cpp
main.cpp
)
TARGET_LINK_LIBRARIES(../public/docBot.wt
wthttp wt boost_signals wtdbo wtdbopostgres boost_regex coreLib modelsLib)

