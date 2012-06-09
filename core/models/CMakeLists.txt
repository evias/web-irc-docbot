ADD_LIBRARY(modelsLib
model.cpp
user.cpp
user_data.cpp
account_data.cpp
)
TARGET_LINK_LIBRARIES(modelsLib wtdbo wtdbopostgres coreLib)
