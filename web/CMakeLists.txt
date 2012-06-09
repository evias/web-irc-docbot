ADD_LIBRARY(webLib
interface.cpp
application.cpp
)
TARGET_LINK_LIBRARIES(webLib wt boost_signals wtdbo wtdbopostgres coreLib modelsLib)

