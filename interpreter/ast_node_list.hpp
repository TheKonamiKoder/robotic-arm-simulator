// This is a macro that is used to help generate the AST nodes without having to write a lot of boilerplate code
// The first "parameter" is the name of the node type
// The next is the base class that it inserts from
// The third is any extra fields that the class has

// This is the list of abstract nodes that cannot be visited
#define AST_ABSTRACT_NODE_LIST \
    X(Expression, AST, ) \
    X(BinaryExpression, Expression, std::unique_ptr<Expression> left; std::unique_ptr<Expression> right;) \
    X(UnaryExpression, Expression, std::unique_ptr<Expression> operand;) 

// This is the list of concrete nodes which can be visited
#define AST_CONCRETE_NODE_LIST \
    X(VariableExpression, Expression, std::string name;) \
    X(NumberLiteralExpression, Expression, Number value;) \
    X(AdditionExpression, BinaryExpression, ) \
    X(SubtractionExpression, BinaryExpression, ) \
    X(MultiplicationExpression, BinaryExpression, ) \
    X(DivisionExpression, BinaryExpression, ) \
    X(UnaryPlusExpression, UnaryExpression, ) \
    X(UnaryMinusExpression, UnaryExpression, ) \
    X(StringLiteralExpression, Expression, String value;) \
    X(BooleanLiteralExpression, Expression, Boolean value;) \
    X(EqualsExpression, BinaryExpression, ) \
    X(NotEqualExpression, BinaryExpression, ) \
    X(GreaterThanExpression, BinaryExpression, ) \
    X(GreaterThanOrEqualExpression, BinaryExpression, ) \
    X(LessThanExpression, BinaryExpression, ) \
    X(LessThanOrEqualExpression, BinaryExpression, ) \
    X(OrExpression, BinaryExpression, ) \
    X(AndExpression, BinaryExpression, ) \
    X(NotExpression, UnaryExpression, )