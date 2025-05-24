## This project requires systemC to be compiled with cmake using C++17:
## cd <systemc directory>
## mkdir build
## cd build
## cmake ../ -DCMAKE_CXX_STANDARD=17
## make

cmake_minimum_required(VERSION 3.20)
project(yisc)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_C_STANDARD 17)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_PREFIX_PATH $ENV{SYSTEMC_HOME})
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_compile_options(-O3 -g -Wall -Wextra -Wunused-function -Wpedantic)

include_directories("./include/")
file(GLOB SRC
    "./src/tb.cpp"
    "./src/components/cpu/*.cpp"
    "./src/components/clock_gen/*.cpp"
    "./src/components/mem/*.cpp"
    "./src/*.cpp"
)

find_package(SystemCLanguage CONFIG REQUIRED)
set (CMAKE_CXX_STANDARD ${SystemC_CXX_STANDARD})

add_executable(yisc ${SRC})
target_link_libraries(yisc SystemC::systemc)
