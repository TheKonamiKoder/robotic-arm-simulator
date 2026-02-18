// This is a macro that is used to help generate the AST nodes without having to write a lot of boilerplate code
// The first "parameter" is the name of the node type
// The next is the base class that it inserts from
// The third is any extra fields that the class has

// This is the list of abstract nodes that cannot be visited
#define AST_ABSTRACT_NODE_LIST \
    X(Expression, AST, ) \
    X(BinaryExpression, Expression, std::unique_ptr<Expression> left; std::unique_ptr<Expression> right;) \
    X(UnaryExpression, Expression, std::unique_ptr<Expression> operand;) \
    X(Statement, AST, ) \

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
    X(NotExpression, UnaryExpression, ) \
    X(VariableDeclarationStatement, Statement, std::string name; std::unique_ptr<Expression> value;) \
    X(AssignmentStatement, Statement, std::string name; std::unique_ptr<Expression> value;) \
    X(RotateStatement, Statement, std::unique_ptr<Expression> motorName; std::unique_ptr<Expression> angularDisplacement;) \
    X(TranslateStatement, Statement, std::unique_ptr<Expression> linearActuatorName; std::unique_ptr<Expression> displacement;) \
    X(RaiseStatement, Statement, std::string event;) \
    X(LogStatement, Statement, std::unique_ptr<Expression> message;) \
    X(BlockStatement, Statement, std::vector<std::unique_ptr<Statement>> statements;) \
    X(IfStatement, Statement, std::unique_ptr<Expression> condition; std::unique_ptr<BlockStatement> ifTrueStatements; std::unique_ptr<BlockStatement> ifFalseStatements;) \
    X(ThenStatement, Statement, std::unique_ptr<Statement> first; std::unique_ptr<Statement> next;)