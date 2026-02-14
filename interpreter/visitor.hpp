#pragma once

#include "ast_node_list.hpp"

// Forward declare the node types
#define X(Concrete, Base, ExtraFields) class Concrete;
    AST_CONCRETE_NODE_LIST
#undef X

class Visitor
{   
public:
    virtual ~Visitor() = default;

// Only concrete classes need to be visited
#define X(Concrete, Base, ExtraFields) virtual void visit(Concrete&) = 0;
    AST_CONCRETE_NODE_LIST
#undef X
};