cmake_minimum_required (VERSION 2.6)
set (PROJECT_NAME "messenger-engine" CXX)
project (${PROJECT_NAME})

set (Boost_NO_SYSTEM_PATHS ON)
set (Boost_USE_MULTITHREADED ON)
set (Boost_USE_STATIC_LIBS ON)
set (Boost_USE_STATIC_RUNTIME OFF)
set (BOOST_ALL_DYN_LINK OFF)
find_package (Boost REQUIRED COMPONENTS system)
if (Boost_FOUND)
    include_directories (SYSTEM ${Boost_INCLUDE_DIR})
endif ()

add_executable (${PROJECT_NAME} main utils)

target_link_libraries (${PROJECT_NAME} ${Boost_LIBRARIES})
