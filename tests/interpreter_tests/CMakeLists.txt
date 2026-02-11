set(TEST_DATA_DIR "${CMAKE_CURRENT_SOURCE_DIR}/interpreter_test_files")

add_executable(lexer_tests lexer_tests.cpp)
add_test(NAME Interpreter.Lexer.GenerateTokenFromWord COMMAND lexer_tests --GenerateTokenFromWord LABELS "Interpreter;Lexer")
add_test(NAME Interpreter.Lexer.GenerateTokensFromLine COMMAND lexer_tests --GenerateTokensFromLine LABELS "Interpreter;Lexer")
add_test(NAME Interpreter.Lexer.GenerateTokensFromFile COMMAND lexer_tests --GenerateTokensFromFile LABELS "Interpreter;Lexer")
target_compile_definitions(lexer_tests PRIVATE TEST_DATA_DIR="${TEST_DATA_DIR}")
target_link_libraries(lexer_tests PRIVATE interpreter test_utils)