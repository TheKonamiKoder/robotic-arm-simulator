#pragma once

#include <string>
#include <unordered_map>

#include "symbol_table.hpp"

// The environment class will allow the interpreter to interact with the simulation engine
class Environment
{
public:
    // The symbol table will store the variables
    SymbolTable symbolTable;
};