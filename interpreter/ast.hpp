#pragma once

#include "token.hpp"

#include <memory>

class Visitor;

// The source location is important as it is used for reporting errors
struct SourceLocation
{
    int line = 0;
};

class AST
{
public:
    SourceLocation sourceLocation;

    AST() = default;
    AST(SourceLocation sourceLocation);
    virtual ~AST() = default;

    virtual void accept(Visitor& visitor) = 0;
};

class Expression : public AST 
{
public:
    Expression() = default;
    Expression(SourceLocation sourceLocation);
};

class NumericExpression : public Expression 
{
public:
    NumericExpression() = default;
    NumericExpression(SourceLocation sourceLocation);
};

class NumberLiteralExpression : public NumericExpression
{
public:
    double value;
    
    NumberLiteralExpression() = default;
    explicit NumberLiteralExpression(double value);
    NumberLiteralExpression(SourceLocation sourceLocation, double value);

    void accept(Visitor& visitor) override;
};

class BinaryExpression : public NumericExpression
{
public:
    std::unique_ptr<NumericExpression> left;
    std::unique_ptr<NumericExpression> right;

    BinaryExpression() = default;
    BinaryExpression(std::unique_ptr<NumericExpression> left, std::unique_ptr<NumericExpression> right);
    BinaryExpression
    (
        SourceLocation sourceLocation, 
        std::unique_ptr<NumericExpression> left, 
        std::unique_ptr<NumericExpression> right
    );
};

class AdditionExpression : public BinaryExpression
{
public:
    AdditionExpression() = default;
    AdditionExpression(std::unique_ptr<NumericExpression> left, std::unique_ptr<NumericExpression> right);
    AdditionExpression
    (
        SourceLocation sourceLocation, 
        std::unique_ptr<NumericExpression> left, 
        std::unique_ptr<NumericExpression> right
    );

    void accept(Visitor& visitor) override;
};

class SubtractionExpression : public BinaryExpression
{
public:
    SubtractionExpression() = default;
    SubtractionExpression(std::unique_ptr<NumericExpression> left, std::unique_ptr<NumericExpression> right);
    SubtractionExpression
    (
        SourceLocation sourceLocation, 
        std::unique_ptr<NumericExpression> left, 
        std::unique_ptr<NumericExpression> right
    );

    void accept(Visitor& visitor) override;
};

class MultiplicationExpression : public BinaryExpression
{
public:
    MultiplicationExpression() = default;
    MultiplicationExpression(std::unique_ptr<NumericExpression> left, std::unique_ptr<NumericExpression> right);
    MultiplicationExpression
    (
        SourceLocation sourceLocation, 
        std::unique_ptr<NumericExpression> left, 
        std::unique_ptr<NumericExpression> right
    );

    void accept(Visitor& visitor) override;
};

class DivisionExpression : public BinaryExpression
{
public:
    DivisionExpression() = default;
    DivisionExpression(std::unique_ptr<NumericExpression> left, std::unique_ptr<NumericExpression> right);
    DivisionExpression
    (
        SourceLocation sourceLocation, 
        std::unique_ptr<NumericExpression> left, 
        std::unique_ptr<NumericExpression> right
    );

    void accept(Visitor& visitor) override;
};

class UnaryExpression : public NumericExpression
{
public:
    std::unique_ptr<NumericExpression> operand;

    UnaryExpression() = default;
    UnaryExpression(std::unique_ptr<NumericExpression> operand);
    UnaryExpression(SourceLocation sourceLocation, std::unique_ptr<NumericExpression> operand);
};

class UnaryMinusExpression : public UnaryExpression
{
public:
    UnaryMinusExpression() = default;
    UnaryMinusExpression(std::unique_ptr<NumericExpression> operand);
    UnaryMinusExpression(SourceLocation sourceLocation, std::unique_ptr<NumericExpression> operand);

    void accept(Visitor& visitor) override;
};

class UnaryPlusExpression : public UnaryExpression
{
public:
    UnaryPlusExpression() = default;
    UnaryPlusExpression(std::unique_ptr<NumericExpression> operand);
    UnaryPlusExpression(SourceLocation sourceLocation, std::unique_ptr<NumericExpression> operand);

    void accept(Visitor& visitor) override;
};
