#include "evaluator.hpp"

#include "ast.hpp"
#include "error.hpp"

void Evaluator::throwErrorIfNotNumber(SourceLocation sourceLocation)
{
    if (this->result.getType() != Value::Type::Number)
    {
        throw TypeError(Value::getTypeName(Value::Type::Number), this->result.getTypeName(), sourceLocation);
    }
}

Evaluator::Evaluator(Environment& environment) : environment(environment) {}

void Evaluator::visit(NumberLiteralExpression& expression)
{
    this->result = expression.value;
}

void Evaluator::visit(AdditionExpression& expression)
{
    expression.left->accept(*this);
    this->throwErrorIfNotNumber(expression.left->sourceLocation);
    Number leftValue = this->result.asNumber();

    expression.right->accept(*this);
    this->throwErrorIfNotNumber(expression.right->sourceLocation);
    Number rightValue = this->result.asNumber();

    this->result = leftValue + rightValue;
}

void Evaluator::visit(SubtractionExpression& expression)
{
    expression.left->accept(*this);
    this->throwErrorIfNotNumber(expression.left->sourceLocation);
    Number leftValue = this->result.asNumber();

    expression.right->accept(*this);
    this->throwErrorIfNotNumber(expression.right->sourceLocation);
    Number rightValue = this->result.asNumber();

    this->result = leftValue - rightValue;
}

void Evaluator::visit(MultiplicationExpression& expression)
{
    expression.left->accept(*this);
    this->throwErrorIfNotNumber(expression.left->sourceLocation);
    Number leftValue = this->result.asNumber();

    expression.right->accept(*this);
    this->throwErrorIfNotNumber(expression.right->sourceLocation);
    Number rightValue = this->result.asNumber();

    this->result = leftValue * rightValue;
}

void Evaluator::visit(DivisionExpression& expression)
{
    expression.left->accept(*this);
    this->throwErrorIfNotNumber(expression.left->sourceLocation);
    Number leftValue = this->result.asNumber();

    expression.right->accept(*this);
    this->throwErrorIfNotNumber(expression.right->sourceLocation);
    Number rightValue = this->result.asNumber();

    if (rightValue == 0)
    {
        throw DivisionByZeroError(expression.right->sourceLocation);
    }

    this->result = leftValue / rightValue;
}

void Evaluator::visit(UnaryMinusExpression& expression)
{
    expression.operand->accept(*this);
    this->throwErrorIfNotNumber(expression.operand->sourceLocation);
    
    this->result = -this->result.asNumber();
}

void Evaluator::visit(UnaryPlusExpression& expression)
{
    expression.operand->accept(*this);
    this->throwErrorIfNotNumber(expression.operand->sourceLocation);
}

void Evaluator::visit(VariableExpression& expression)
{
    try
    {
        this->result = this->environment.symbolTable.lookup(expression.name);
    }
    catch(const UndefinedIdentifierError& error)
    {
        // Add the source location to the error (since the environment does not know about the source location)
        throw UndefinedIdentifierError(error.getIdentifier(), expression.sourceLocation);
    }
}