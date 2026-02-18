#pragma once

#include "visitor.hpp"

#include <functional>

#include "environment.hpp"
#include "source_location.hpp"
#include "value.hpp"

class Expression;

class Evaluator : public Visitor
{
private:
    // This function will do the operation but also add context to the errors (if necessary)
    void doOperation(std::function<void()> operation, SourceLocation sourceLocation = {});
public:
    Environment& environment;
    Value result = Value();

    Evaluator(Environment& environment);

    // This is a helper function to return the result after visiting the expression node
    Value getResult(Expression& expression);

    void visit(VariableExpression& expression) override;

    void visit(NumberLiteralExpression& expression) override;
    void visit(AdditionExpression& expression) override;
    void visit(SubtractionExpression& expression) override;
    void visit(MultiplicationExpression& expression) override;
    void visit(DivisionExpression& expression) override;
    void visit(UnaryMinusExpression& expression) override;
    void visit(UnaryPlusExpression& expression) override;

    void visit(StringLiteralExpression& expression) override;

    void visit(BooleanLiteralExpression& expression) override;
    void visit(EqualsExpression& expression) override;
    void visit(NotEqualExpression& expression) override;
    void visit(GreaterThanExpression& expression) override;
    void visit(GreaterThanOrEqualExpression& expression) override;
    void visit(LessThanExpression& expression) override;
    void visit(LessThanOrEqualExpression& expression) override;
    void visit(OrExpression& expression) override;
    void visit(AndExpression& expression) override;
    void visit(NotExpression& expression) override;
};