#include "evaluator.hpp"

#include <iostream>

#include "ast.hpp"
#include "error.hpp"

Value Evaluator::getResult(Expression& expression)
{
    expression.accept(*this);
    return this->result;
}

void Evaluator::doOperation(std::function<void()> operation, SourceLocation sourceLocation)
{
    try
    {
        operation();
    }
    catch(RPLError& error)
    {
        error.sourceLocation = sourceLocation;
        throw;
    }
}

Evaluator::Evaluator(Environment& environment) : environment(environment) {}

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

void Evaluator::visit(NumberLiteralExpression& expression)
{
    this->result = expression.value;
}

void Evaluator::visit(AdditionExpression& expression)
{
    this->doOperation
    (
        [&](){this->result = this->getResult(*expression.left) + this->getResult(*expression.right);}, 
        expression.sourceLocation
    );
}

void Evaluator::visit(SubtractionExpression& expression)
{
    this->doOperation
    (
        [&](){this->result = this->getResult(*expression.left) - this->getResult(*expression.right);}, 
        expression.sourceLocation
    );
}

void Evaluator::visit(MultiplicationExpression& expression)
{
    this->doOperation
    (
        [&](){this->result = this->getResult(*expression.left) * this->getResult(*expression.right);}, 
        expression.sourceLocation
    );
}

void Evaluator::visit(DivisionExpression& expression)
{
    this->doOperation
    (
        [&](){this->result = this->getResult(*expression.left) / this->getResult(*expression.right);}, 
        expression.sourceLocation
    );
}

void Evaluator::visit(UnaryMinusExpression& expression)
{
    this->doOperation([&](){this->result = -1.0 * this->getResult(*expression.operand);}, expression.sourceLocation);
}

void Evaluator::visit(UnaryPlusExpression& expression)
{
    this->doOperation([&](){this->result = 1.0 * this->getResult(*expression.operand);}, expression.sourceLocation);
}

void Evaluator::visit(StringLiteralExpression& expression)
{
    this->result = expression.value;
}

void Evaluator::visit(BooleanLiteralExpression& expression)
{
    this->result = expression.value;
}

void Evaluator::visit(EqualsExpression& expression)
{
    this->doOperation
    (
        [&](){this->result = this->getResult(*expression.left) == this->getResult(*expression.right);}, 
        expression.sourceLocation
    );
}

void Evaluator::visit(NotEqualExpression& expression)
{
    this->doOperation
    (
        [&](){this->result = this->getResult(*expression.left) != this->getResult(*expression.right);}, 
        expression.sourceLocation
    );   
}

void Evaluator::visit(GreaterThanExpression& expression)
{
    this->doOperation
    (
        [&](){this->result = this->getResult(*expression.left) > this->getResult(*expression.right);}, 
        expression.sourceLocation
    );
}

void Evaluator::visit(GreaterThanOrEqualExpression& expression)
{
    this->doOperation
    (
        [&](){this->result = this->getResult(*expression.left) >= this->getResult(*expression.right);}, 
        expression.sourceLocation
    );
}

void Evaluator::visit(LessThanExpression& expression)
{
    this->doOperation
    (
        [&](){this->result = this->getResult(*expression.left) < this->getResult(*expression.right);}, 
        expression.sourceLocation
    );
}

void Evaluator::visit(LessThanOrEqualExpression& expression)
{
    this->doOperation
    (
        [&](){this->result = this->getResult(*expression.left) <= this->getResult(*expression.right);}, 
        expression.sourceLocation
    );
}

void Evaluator::visit(OrExpression& expression)
{
    this->doOperation
    (
        [&](){this->result = this->getResult(*expression.left) || this->getResult(*expression.right);}, 
        expression.sourceLocation
    );

}

void Evaluator::visit(AndExpression& expression)
{
    this->doOperation
    (
        [&](){this->result = this->getResult(*expression.left) && this->getResult(*expression.right);}, 
        expression.sourceLocation
    );
}

void Evaluator::visit(NotExpression& expression)
{
    this->doOperation([&](){this->result = !this->getResult(*expression.operand);}, expression.sourceLocation);
}
