cmake_minimum_required(VERSION 3.10)
project(Parser)

find_package(CURL REQUIRED)
find_package(libssh2 REQUIRED) # Только если нужен libssh2

add_executable(parser main.cpp)
target_link_libraries(parser PRIVATE CURL::libcurl libssh2::libssh2) # Линкуем с libcurl (и libssh2, если нужно)
