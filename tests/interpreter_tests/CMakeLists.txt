set(TEST_DATA_DIR "${CMAKE_CURRENT_SOURCE_DIR}/interpreter_test_files")

add_executable(lexer_tests lexer_tests.cpp)
add_test(NAME Interpreter.Lexer.GenerateTokenFromWord COMMAND lexer_tests --GenerateTokenFromWord LABELS "Interpreter;Lexer")
add_test(NAME Interpreter.Lexer.GenerateTokensFromLine COMMAND lexer_tests --GenerateTokensFromLine LABELS "Interpreter;Lexer")
add_test(NAME Interpreter.Lexer.GenerateTokensFromFile COMMAND lexer_tests --GenerateTokensFromFile LABELS "Interpreter;Lexer")
target_compile_definitions(lexer_tests PRIVATE TEST_DATA_DIR="${TEST_DATA_DIR}")
target_link_libraries(lexer_tests PRIVATE interpreter test_utils)

add_executable(parser_tests parser_tests.cpp)
add_test(NAME Interpreter.Parser.ConvertInfixToPostfix COMMAND parser_tests --ConvertInfixToPostfix LABELS "Interpreter;Parser")
add_test(NAME Interpreter.Parser.ParseNextExpression COMMAND parser_tests --ParseNextExpression LABELS "Interpreter;Parser")
target_link_libraries(parser_tests PRIVATE interpreter test_utils)

add_executable(evaluator_tests evaluator_tests.cpp)
add_test(NAME Interpreter.Evaluator.Literals COMMAND evaluator_tests --Literals LABELS "Interpreter;Evaluator")
add_test(NAME Interpreter.Evaluator.NumericExpressions COMMAND evaluator_tests --NumericExpressions LABELS "Interpreter;Evaluator")
add_test(NAME Interpreter.Evaluator.LogicalExpressions COMMAND evaluator_tests --LogicalExpressions LABELS "Interpreter;Evaluator")
target_link_libraries(evaluator_tests PRIVATE interpreter test_utils)