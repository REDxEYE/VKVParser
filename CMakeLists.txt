cmake_minimum_required(VERSION 3.20)
project(VMTParser)

set(CMAKE_CXX_STANDARD 20)

include_directories(./include)


set(VMTParserSRC
        src/library.cpp
        src/vmt_node.cpp
        include/vmt_lexer.cpp
        include/vmt_parser.cpp
        )

set(VMTParserTestSRC
        src/main.cpp
        src/vmt_node.cpp
        include/vmt_lexer.cpp
        include/vmt_parser.cpp
        )

add_library(VMTParser SHARED ${VMTParserSRC})
add_executable(VMTParserTest ${VMTParserTestSRC})
