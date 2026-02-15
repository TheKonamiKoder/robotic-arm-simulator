#pragma once

#include <memory>

#include "ast_node_list.hpp"
#include "source_location.hpp"
#include "value.hpp"

class Visitor;

class AST
{
public:
    SourceLocation sourceLocation;
    virtual ~AST() = default;
    virtual void accept(Visitor& visitor) = 0;
};

// This will generate class definitions from the abstract classes
#define X(Abstract, Base, ExtraFields) \
class Abstract: public Base \
{ \
public: \
    ExtraFields \
};
AST_ABSTRACT_NODE_LIST
#undef X

// This will generate the class definition for the concrete classes
#define X(Concrete, Base, ExtraFields) \
class Concrete : public Base \
{ \
public: \
    ExtraFields \
    void accept(Visitor& visitor) override; \
};
AST_CONCRETE_NODE_LIST
#undef X