#include "value.hpp"

std::string Value::getTypeName(Value::Type type)
{
    switch (type)
    {
    case Value::Type::Number:
        return "Number";
        break;
    case Value::Type::String:
        return "String";
        break;
    case Value::Type::Boolean:
        return "Boolean";
        break;
    default:
        break;
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