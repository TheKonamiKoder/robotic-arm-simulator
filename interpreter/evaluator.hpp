#pragma once

#include "visitor.hpp"

class Evaluator : public Visitor
{
public:
    double result = 0;

    void visit(NumberLiteralExpression& expression) override;

    void visit(AdditionExpression& expression) override;
    void visit(SubtractionExpression& expression) override;
    void visit(MultiplicationExpression& expression) override;
    void visit(DivisionExpression& expression) override;

    void visit(UnaryMinusExpression& expression) override;
    void visit(UnaryPlusExpression& expression) override;
};