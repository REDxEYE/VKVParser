cmake_minimum_required(VERSION 3.20)
project(KVParser)

set(CMAKE_CXX_STANDARD 20)

include_directories(./include)


set(KVParserSRC
        src/library.cpp
        src/kv_node.cpp
        src/kv_lexer.cpp
        src/kv_parser.cpp
        )

set(KVParserTestSRC
        src/main.cpp
        src/kv_node.cpp
        src/kv_lexer.cpp
        src/kv_parser.cpp
        )

add_library(KVParser SHARED ${KVParserSRC})
add_executable(KVParserTest ${KVParserTestSRC})
