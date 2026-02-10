add_executable(lexer_tests lexer_tests.cpp)
add_test(NAME Interpreter.Lexer.GenerateTokenFromWord COMMAND lexer_tests --GenerateTokenFromWord LABELS "Interpreter;Lexer")
add_test(NAME Interpreter.Lexer.GenerateTokenFromLine COMMAND lexer_tests --GenerateTokensFromLine LABELS "Interpreter;Lexer")
target_link_libraries(lexer_tests PRIVATE interpreter test_utils)