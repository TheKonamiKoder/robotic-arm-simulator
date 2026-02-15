#include "visitor.hpp"
#include "ast.hpp"

#define X(Concrete, ...) \
void Concrete::accept(Visitor& visitor) \
{\
    visitor.visit(*this); \
}
AST_CONCRETE_NODE_LIST
#undef X