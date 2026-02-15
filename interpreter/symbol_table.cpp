#include "symbol_table.hpp"

#include "error.hpp"

void SymbolTable::throwErrorIfUndefined(const std::string& name) const
{
    if (!this->contains(name))
    {
        throw UndefinedIdentifierError(name);
    }
}

SymbolTable::SymbolTable(std::unordered_map<std::string, Value> symbolTableMap) : symbolTableMap(symbolTableMap) {}

bool SymbolTable::contains(const std::string& name) const
{
    return this->symbolTableMap.find(name) != this->symbolTableMap.end();
}

void SymbolTable::define(const std::string& name, const Value& value)
{
    this->symbolTableMap[name] = value;
}

void SymbolTable::assign(const std::string& name, const Value& value)
{
    throwErrorIfUndefined(name);
    this->symbolTableMap[name] = value;
}

Value SymbolTable::lookup(const std::string& name) const
{
    throwErrorIfUndefined(name);
    return this->symbolTableMap.at(name);
}