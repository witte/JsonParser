cmake_minimum_required(VERSION 3.21)
project(JsonParser LANGUAGES CXX)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_OSX_DEPLOYMENT_TARGET 11.0)
set(CMAKE_CXX_EXTENSIONS OFF)

# set(CMAKE_OSX_ARCHITECTURES "x86_64")


add_compile_options(
       -Wall -Wextra -Wpedantic -Weverything
       -Wno-c++98-compat -Wno-c++98-compat-pedantic
       -Wno-padded -Wno-unused-macros
       #-Werror
)


add_library(JsonParser include/JsonSerializer.h
                       include/JsonParser.h
                       src/JsonParser.cpp)
target_include_directories(JsonParser PUBLIC include)


# Tests
add_executable(JsonParserTests tests/Structs.h
                               tests/JsonParserTests.cpp)
target_link_libraries(JsonParserTests PUBLIC JsonParser)
