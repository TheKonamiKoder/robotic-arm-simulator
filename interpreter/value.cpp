#include "value.hpp"

std::string Value::getTypeName(Value::Type type)
{
    switch (type)
    {
    case Value::Type::Number:
        return "Number";
    case Value::Type::String:
        return "String";
    case Value::Type::Boolean:
        return "Boolean";
    case Value::Type::None:
        return "String";
    default:
        return "Unknown";
    }
}

Value::Value(Number numberValue) : type(Type::Number), numberValue(numberValue) {}

Value::Value(String stringValue) : type(Type::String), stringValue(stringValue) {}

Value::Value(Boolean booleanValue) : type(Type::Boolean), booleanValue(booleanValue) {}

Number Value::asNumber() const 
{
    return this->numberValue;
}

String Value::asString() const
{
    return this->stringValue;
}

Boolean Value::asBoolean() const
{
    return this->booleanValue;
}

Value::Type Value::getType() const
{
    return this->type;
}

std::string Value::getTypeName() const
{
    return Value::getTypeName(this->getType());
}

bool operator==(const Value& lhs, const Value& rhs)
{
    if (lhs.type != rhs.type)
    {
        return false;
    }
    
    switch (lhs.type)
    {
    case Value::Type::Number:
        return lhs.numberValue == rhs.numberValue;
    case Value::Type::String:
        return lhs.stringValue == rhs.stringValue;
    case Value::Type::Boolean:
        return lhs.booleanValue == rhs.booleanValue;
    case Value::Type::None:
        return true;
    default:        // If the type is not any
        return false;
    }
}