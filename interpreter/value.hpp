#pragma once

#include <string>

// Type aliases defined so that the internal types representing Number, String and Boolean can be changed if necessary
// in the future
using Number = double;
using String = std::string;
using Boolean = bool;

class Value
{
public:
    enum class Type
    {
        Number,
        String,
        Boolean,
        None
    };

    // This is a function used to get the name of any type
    static std::string getTypeName(Type type);
private:
    Type type = Type::None;
    Number numberValue;
    String stringValue;
    Boolean booleanValue;
public:
    Value() = default;
    Value(Number numberValue);
    Value(String stringValue);
    Value(Boolean booleanValue);

    Number asNumber() const;
    String asString() const;
    Boolean asBoolean() const;
    
    Type getType() const;

    // This will return the name of the value's type
    std::string getTypeName() const;
};