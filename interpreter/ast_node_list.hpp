// This is a macro that is used to help generate the AST nodes without having to write a lot of boilerplate code
// The first "parameter" is the name of the node type
// The next is the base class that it inserts from
// The third is any extra fields that the class has
#define AST_ABSTRACT_NODE_LIST \
    X(Expression, AST, ) \
    X(BinaryExpression, Expression, std::unique_ptr<Expression> left; std::unique_ptr<Expression> right;) \
    X(UnaryExpression, Expression, std::unique_ptr<Expression> operand;) 

#define AST_CONCRETE_NODE_LIST \
    X(NumberLiteralExpression, Expression, Number value;) \
    X(AdditionExpression, BinaryExpression, ) \
    X(SubtractionExpression, BinaryExpression, ) \
    X(MultiplicationExpression, BinaryExpression, ) \
    X(DivisionExpression, BinaryExpression, ) \
    X(UnaryPlusExpression, UnaryExpression, ) \
    X(UnaryMinusExpression, UnaryExpression, ) \
    X(VariableExpression, Expression, std::string name;)