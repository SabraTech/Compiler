cmake_minimum_required(VERSION 3.6)
project(Compiler)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp Parser.cpp Parser.h NFA.cpp NFA.h Node.h Node.cpp DFA.cpp DFA.h Utilities.h Utilities.cpp
        MinimizationTable.cpp MinimizationTable.h Phase1.cpp Phase1.h Phase2.cpp Phase2.h ParsingTable.h "ParsingTable .cpp")
add_executable(Compiler ${SOURCE_FILES})