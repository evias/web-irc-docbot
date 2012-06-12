ADD_EXECUTABLE(../public/docWeb.wt
interface.cpp
application.cpp
main.cpp
)
TARGET_LINK_LIBRARIES(../public/docWeb.wt
wthttp wt boost_signals wtdbo wtdbopostgres coreLib modelsLib)

