#include "evaluator.hpp"

#include "ast.hpp"
#include "error.hpp"

void Evaluator::visit(NumberLiteralExpression& expression)
{
    this->result = expression.value;
}

void Evaluator::visit(AdditionExpression& expression)
{
    expression.left->accept(*this);
    double leftValue = this->result;

    expression.right->accept(*this);
    double rightValue = this->result;

    this->result = leftValue + rightValue;
}

void Evaluator::visit(SubtractionExpression& expression)
{
    expression.left->accept(*this);
    double leftValue = this->result;

    expression.right->accept(*this);
    double rightValue = this->result;

    this->result = leftValue - rightValue;
}

void Evaluator::visit(MultiplicationExpression& expression)
{
    expression.left->accept(*this);
    double leftValue = this->result;

    expression.right->accept(*this);
    double rightValue = this->result;

    this->result = leftValue * rightValue;
}

void Evaluator::visit(DivisionExpression& expression)
{
    expression.left->accept(*this);
    double leftValue = this->result;

    expression.right->accept(*this);
    double rightValue = this->result;

    if (rightValue == 0)
    {
        throw EvaluationError("Dividng by 0", ErrorType::DivisionByZero, expression.right->sourceLocation.line);
    }

    this->result = leftValue / rightValue;
}

void Evaluator::visit(UnaryMinusExpression& expression)
{
    expression.operand->accept(*this);
    this->result = -this->result;
}

void Evaluator::visit(UnaryPlusExpression& expression)
{
    expression.operand->accept(*this);
}