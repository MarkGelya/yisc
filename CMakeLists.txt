cmake_minimum_required(VERSION 3.0)

project(yisc)

add_executable(yisc main.cpp)

install(TARGETS yisc RUNTIME DESTINATION bin)
