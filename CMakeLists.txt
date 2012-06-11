CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

PROJECT(DOCBOT)

ADD_EXECUTABLE(./public/docBot.wt
main.cpp
)

TARGET_LINK_LIBRARIES(./public/docBot.wt
wthttp wt boost_signals wtdbo wtdbopostgres coreLib modelsLib webLib botLib
)

INCLUDE_DIRECTORIES(/usr/include .)

ADD_SUBDIRECTORY(core)
ADD_SUBDIRECTORY(core/models)
ADD_SUBDIRECTORY(web)
ADD_SUBDIRECTORY(bot)
