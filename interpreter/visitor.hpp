#pragma once

class NumberLiteralExpression;
class AdditionExpression;
class SubtractionExpression;
class MultiplicationExpression;
class DivisionExpression;
class UnaryMinusExpression;
class UnaryPlusExpression;

class Visitor
{   
public:
    virtual ~Visitor() = default;

    virtual void visit(NumberLiteralExpression& expression) = 0;

    virtual void visit(AdditionExpression& expression) = 0;
    virtual void visit(SubtractionExpression& expression) = 0;
    virtual void visit(MultiplicationExpression& expression) = 0;
    virtual void visit(DivisionExpression& expression) = 0;

    virtual void visit(UnaryMinusExpression& expression) = 0;
    virtual void visit(UnaryPlusExpression& expression) = 0;
};