#include "ast.hpp"

#include <stdexcept>

#include "visitor.hpp"

AST::AST(SourceLocation sourceLocation) : sourceLocation(sourceLocation) {}


Expression::Expression(SourceLocation sourceLocation) : AST(sourceLocation) {}


NumericExpression::NumericExpression(SourceLocation sourceLocation) : Expression(sourceLocation) {}


NumberLiteralExpression::NumberLiteralExpression(double value) : value(value) {}

NumberLiteralExpression::NumberLiteralExpression(SourceLocation sourceLocation, double value)
    : NumericExpression(sourceLocation), value(value) {}

void NumberLiteralExpression::accept(Visitor& visitor) 
{
    visitor.visit(*this);
}


BinaryExpression::BinaryExpression(std::unique_ptr<NumericExpression> left, std::unique_ptr<NumericExpression> right)
    : left(std::move(left)), right(std::move(right)) {}

BinaryExpression::BinaryExpression
(
    SourceLocation sourceLocation, 
    std::unique_ptr<NumericExpression> left, 
    std::unique_ptr<NumericExpression> right
) : NumericExpression(sourceLocation), left(std::move(left)), right(std::move(right)) {}


AdditionExpression::AdditionExpression
(
    std::unique_ptr<NumericExpression> left, 
    std::unique_ptr<NumericExpression> right
) : BinaryExpression(std::move(left), std::move(right)) {}

AdditionExpression::AdditionExpression
(
    SourceLocation sourceLocation, 
    std::unique_ptr<NumericExpression> left, 
    std::unique_ptr<NumericExpression> right
) : BinaryExpression(sourceLocation, std::move(left), std::move(right)) {}

void AdditionExpression::accept(Visitor& visitor) 
{
    visitor.visit(*this);
}


SubtractionExpression::SubtractionExpression
(
    std::unique_ptr<NumericExpression> left, 
    std::unique_ptr<NumericExpression> right
) : BinaryExpression(std::move(left), std::move(right)) {}

SubtractionExpression::SubtractionExpression
(
    SourceLocation sourceLocation, 
    std::unique_ptr<NumericExpression> left, 
    std::unique_ptr<NumericExpression> right
) : BinaryExpression(sourceLocation, std::move(left), std::move(right)) {}

void SubtractionExpression::accept(Visitor& visitor)
{
    visitor.visit(*this);
}


MultiplicationExpression::MultiplicationExpression
(
    std::unique_ptr<NumericExpression> left, 
    std::unique_ptr<NumericExpression> right
) : BinaryExpression(std::move(left), std::move(right)) {}

MultiplicationExpression::MultiplicationExpression
(
    SourceLocation sourceLocation, 
    std::unique_ptr<NumericExpression> left, 
    std::unique_ptr<NumericExpression> right
) : BinaryExpression(sourceLocation, std::move(left), std::move(right)) {}

void MultiplicationExpression::accept(Visitor& visitor)
{
    visitor.visit(*this);
}


DivisionExpression::DivisionExpression
(
    std::unique_ptr<NumericExpression> left, 
    std::unique_ptr<NumericExpression> right
) : BinaryExpression(std::move(left), std::move(right)) {}

DivisionExpression::DivisionExpression
(
    SourceLocation sourceLocation, 
    std::unique_ptr<NumericExpression> left, 
    std::unique_ptr<NumericExpression> right
) : BinaryExpression(sourceLocation, std::move(left), std::move(right)) {}

void DivisionExpression::accept(Visitor& visitor)
{
    visitor.visit(*this);
}


UnaryExpression::UnaryExpression(std::unique_ptr<NumericExpression> operand) : operand(std::move(operand)) {}

UnaryExpression::UnaryExpression
(
    SourceLocation sourceLocation, 
    std::unique_ptr<NumericExpression> operand
): NumericExpression(sourceLocation), operand(std::move(operand)) {}


UnaryMinusExpression::UnaryMinusExpression(std::unique_ptr<NumericExpression> operand) 
    : UnaryExpression(std::move(operand)) {}

UnaryMinusExpression::UnaryMinusExpression
(
    SourceLocation sourceLocation, 
    std::unique_ptr<NumericExpression> operand
): UnaryExpression(sourceLocation, std::move(operand)) {}

void UnaryMinusExpression::accept(Visitor& visitor)
{
    visitor.visit(*this);
}


UnaryPlusExpression::UnaryPlusExpression(std::unique_ptr<NumericExpression> operand) 
    : UnaryExpression(std::move(operand)) {}

UnaryPlusExpression::UnaryPlusExpression
(
    SourceLocation sourceLocation, 
    std::unique_ptr<NumericExpression> operand
): UnaryExpression(sourceLocation, std::move(operand)) {}

void UnaryPlusExpression::accept(Visitor& visitor)
{
    visitor.visit(*this);
}