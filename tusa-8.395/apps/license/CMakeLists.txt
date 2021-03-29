cmake_minimum_required (VERSION 2.6)

add_executable(sign

sign1.cpp

)

target_link_libraries(sign core core_web mysqlclient crypto z  pthread dl)

install(TARGETS sign DESTINATION ${BIN_TARGET_DIR})

