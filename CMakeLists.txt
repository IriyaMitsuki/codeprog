cmake_minimum_required(VERSION 3.10)
project(Parser)

find_package(CURL REQUIRED)

add_executable(parser main.cpp)
target_link_libraries(parser CURL::libcurl) # Линкуем с libcurl
