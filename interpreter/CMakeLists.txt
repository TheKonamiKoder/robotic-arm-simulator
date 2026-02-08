add_library(interpreter
    token.cpp
    lexer.cpp
)

target_include_directories(interpreter PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
