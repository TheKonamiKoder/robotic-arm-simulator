#include "value.hpp"

#include "error.hpp"

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
        return "None";
    default:
        return "Unknown";
    }
}

Value::Value(Number numberValue) : type(Type::Number), numberValue(numberValue) {}

Value::Value(String stringValue) : type(Type::String), stringValue(stringValue) {}

Value::Value(const char* stringValue) : type(Type::String), stringValue(stringValue) {}

Value::Value(Boolean booleanValue) : type(Type::Boolean), booleanValue(booleanValue) {}

Number Value::asNumber() const 
{
    if (this->type != Type::Number)
    {
        throw TypeError(Value::getTypeName(Type::Number), this->getTypeName());
    }

    return this->numberValue;
}

String Value::asString() const
{
    if (this->type != Type::String)
    {
        throw TypeError(Value::getTypeName(Type::String), this->getTypeName());
    }
    
    return this->stringValue;
}

Boolean Value::asBoolean() const
{
    if (this->type != Type::Boolean)
    {
        throw TypeError(Value::getTypeName(Type::Boolean), this->getTypeName());
    }    

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

Value operator+(const Value& lhs, const Value& rhs)
{
    return lhs.asNumber() + rhs.asNumber();
}

Value operator-(const Value& lhs, const Value& rhs)
{
    return lhs.asNumber() - rhs.asNumber();
}

Value operator*(const Value& lhs, const Value& rhs)
{
    return lhs.asNumber() * rhs.asNumber();
}

Value operator/(const Value& lhs, const Value& rhs)
{
    if (rhs.asNumber() == 0)
    {
        throw DivisionByZeroError();
    }
    
    return lhs.asNumber() / rhs.asNumber();
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

bool operator!=(const Value& lhs, const Value& rhs)
{
    return !(lhs == rhs);
}

bool operator>(const Value& lhs, const Value& rhs)
{    
    return lhs.asNumber() > rhs.asNumber();
}

bool operator>=(const Value& lhs, const Value& rhs)
{
    return lhs.asNumber() >= rhs.asNumber();
}

bool operator<(const Value& lhs, const Value& rhs)
{
    return lhs.asNumber() < rhs.asNumber();
}

bool operator<=(const Value& lhs, const Value& rhs)
{
    return lhs.asNumber() <= rhs.asNumber();
}

bool operator||(const Value& lhs, const Value& rhs)
{
    return lhs.asBoolean() || rhs.asBoolean();
}

bool operator&&(const Value& lhs, const Value& rhs)
{
    return lhs.asBoolean() && rhs.asBoolean();
}

bool Value::operator!() const
{
    return !this->asBoolean();
}

std::ostream& operator<<(std::ostream& os, const Value& value)
{
    switch (value.type)
    {
    case Value::Type::Number:
        return os << value.numberValue;
    case Value::Type::String:
        return os << value.stringValue;
    case Value::Type::Boolean:
        return (value.booleanValue) ? os << "TRUE" : os << "FALSE";
    case Value::Type::None:
        return os << "None";
    default:
        return os << "Unknown";
    }
}