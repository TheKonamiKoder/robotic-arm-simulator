add_executable(lexer_tests lexer_tests.cpp)
add_test(NAME Interpreter.LexerTests COMMAND lexer_tests LABELS "Interpreter")
target_link_libraries(lexer_tests PRIVATE interpreter test_utils)