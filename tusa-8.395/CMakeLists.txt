
cmake_minimum_required (VERSION 2.6)

project (megatron)

if(UNIX AND NOT APPLE)
SET(LINUX true)
endif()

SET(ENABLE_MYSQL true)
#SET(FreeBSD true)
SET(DFS true)
set(CMAKE_SHARED_LINKER_FLAGS "-Xlinker --export-dynamic -Xlinker -zdefs")


OPTION(MT_CORE_SERVICES_SINGLE_PLUGING "Build apps/core-plugins - all core plugins in one file" ON)
include_directories(core core-web
		stdafx
			)

include_directories(system /usr/local/include)

#add_definitions(-std=gnu++0x)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -fPIC ")
# -g -ggdb3 -pg
link_directories( /usr/local/lib)
#link_directories(~/bin/lib /usr/local/lib)



IF(LINUX)
SET(DL "dl")
ENDIF()


SET(PROJECT_BASE_DIR ~/tusa-bin)
SET(PLUGIN_TARGET_DIR ${PROJECT_BASE_DIR}/plugin)
SET(BIN_TARGET_DIR ${PROJECT_BASE_DIR}/bin)
SET(LIB_TARGET_DIR ${PROJECT_BASE_DIR}/lib)
SET(LOG_TARGET_DIR ${PROJECT_BASE_DIR}/log)
SET(DATA_TARGET_DIR ${PROJECT_BASE_DIR}/data)
SET(CONFIG_TARGET_DIR ${PROJECT_BASE_DIR}/conf)
SET(CACHE_TARGET_DIR ${PROJECT_BASE_DIR}/cache)




set(CMAKE_CONFIGURATION_TYPES "Debug" CACHE STRING "" FORCE )
add_definitions(   -DMUTEX_INSPECTOR_DEBUG  -DWITH_MUTEXES -DWITH_SENDERS -D_NO_PROXY -D__WITH_FILES_CONF__ -D__NO_LICENSE)


ADD_SUBDIRECTORY(chatserver)
ADD_SUBDIRECTORY(core)
ADD_SUBDIRECTORY(core-web)
ADD_SUBDIRECTORY(apps)

    

