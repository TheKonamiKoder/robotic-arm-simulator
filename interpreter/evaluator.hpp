#pragma once

#include "environment.hpp"
#include "source_location.hpp"
#include "value.hpp"
#include "visitor.hpp"

class Evaluator : public Visitor
{
private:
    void throwErrorIfNotNumber(SourceLocation sourceLocation);
public:
    Environment& environment;
    Value result = Value();

    Evaluator(Environment& environment);

    void visit(NumberLiteralExpression& expression) override;

    void visit(AdditionExpression& expression) override;
    void visit(SubtractionExpression& expression) override;
    void visit(MultiplicationExpression& expression) override;
    void visit(DivisionExpression& expression) override;

    void visit(UnaryMinusExpression& expression) override;
    void visit(UnaryPlusExpression& expression) override;

    void visit(VariableExpression& expression) override;
};