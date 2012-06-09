ADD_LIBRARY(webLib
application.cpp
)
TARGET_LINK_LIBRARIES(webLib wt boost_signals wtdbo wtdbopostgres coreLib modelsLib)

