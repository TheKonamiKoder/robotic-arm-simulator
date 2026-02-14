add_library(interpreter
    error.cpp
    evaluator.cpp
    lexer.cpp
    source_location.cpp
    symbol_table.cpp
    token.cpp
    value.cpp
)

target_include_directories(interpreter PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
