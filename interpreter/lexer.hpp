#pragma once

#include <string>
#include <vector>

#include "source_location.hpp"

class Token;

// This function will generate a token from a word, where a word corresponds to a single token
Token generateTokenFromWord(const std::string& word, SourceLocation SourceLocation = {});

// This function will generate a vector of tokens from a line using the generateTokenFromWord statement
std::vector<Token> generateTokensFromLine(const std::string& line, int lineNumber = 0);

// This function will generate a vector of tokens from a file
std::vector<Token> generateTokensFromFile(const std::string& filename);