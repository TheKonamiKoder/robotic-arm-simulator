#pragma once

#include <string>
#include <vector>

class Token;

namespace lexer
{
    std::vector<Token> generateTokensFromLine(std::string& line, int lineNumber = 0);
    Token generateTokenFromWord(const std::string& word, int lineNumber = 0);
}