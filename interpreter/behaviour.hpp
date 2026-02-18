#pragma once

#include <string>
#include <vector>

#include "token.hpp"

class Behaviour
{
public:
    std::string event;
    std::vector<Token> tokens;

    Behaviour(std::string event, std::vector<Token> tokens) : event(event), tokens(tokens) {}
};
