#pragma once

#include <string>
#include <unordered_map>

#include "value.hpp"

class SymbolTable
{
private:
    std::unordered_map<std::string, Value> symbolTableMap;

    // This function will throw a runtime error if the name doesn't exist
    void throwErrorIfUndefined(const std::string& name) const;
public:
    SymbolTable() = default;
    SymbolTable(std::unordered_map<std::string, Value> symbolTableMap);

    // Will be used to define a new function (used for the declaration)
    void define(const std::string& name, const Value& value);

    // Will be used to assign a value to a variable
    void assign(const std::string& name, const Value& value);

    // This is a helper function to check if the name exists
    bool contains(const std::string& name) const;

    // Will be used to look up the value of the variable
    Value lookup(const std::string& name) const;
};