#include "interpreter.hpp"

#include <iostream>

#include "ast.hpp"
#include "error.hpp"
#include "environment.hpp"

void Interpreter::execute(std::function<void()> stuffToDo, SourceLocation sourceLocation)
{
    try
    {
        stuffToDo();
    }
    catch(RPLError& error)
    {
        if ((error.sourceLocation.line == 0) && (error.sourceLocation.column == 0))
        {
            error.sourceLocation = sourceLocation;
        }
        this->task = std::make_unique<LogErrorTask>(LogErrorTask(std::string(error.what())));
        std::cout << error.what();
    }
}

Interpreter::Interpreter(Environment& environment) : environment(environment), evaluator(Evaluator(evaluator)) {}

void Interpreter::visit(VariableDeclarationStatement& statement) 
{
    this->execute
    (
        [&](){
            this->environment.symbolTable.define(statement.name, this->evaluator.getResult(*statement.value));},
        statement.sourceLocation
    );
}

void Interpreter::visit(AssignmentStatement& statement) 
{
    this->execute
    (
        [&](){this->environment.symbolTable.assign(statement.name, this->evaluator.getResult(*statement.value));},
        statement.sourceLocation
    );
}

void Interpreter::visit(RotateStatement& statement) 
{
    this->execute
    (
        [&]()
        {
            this->task = std::make_unique<RotateTask>(RotateTask
            (
                this->evaluator.getResult(*statement.motorName).asString(), 
                this->evaluator.getResult(*statement.angularDisplacement).asNumber()
            ));
        },
        statement.sourceLocation
    );
}

void Interpreter::visit(TranslateStatement& statement) 
{
    this->execute
    (
        [&]()
        {
            this->task = std::make_unique<TranslateTask>(TranslateTask
            (
                this->evaluator.getResult(*statement.linearActuatorName).asString(),
                this->evaluator.getResult(*statement.displacement).asNumber()
            ));
        },
        statement.sourceLocation
    );
}

void Interpreter::visit(RaiseStatement& statement) 
{
    this->execute([&](){this->task = std::make_unique<RaiseTask>(RaiseTask(statement.event));}, statement.sourceLocation);
}

void Interpreter::visit(LogStatement& statement) 
{
    this->execute
    (
        [&](){this->task = std::make_unique<LogTask>(LogTask(this->evaluator.getResult(*statement.message).toString()));},
        statement.sourceLocation
    );
}

void Interpreter::visit(BlockStatement& statement) 
{
    this->execute(
        [&](){
    for (int i = 0; i < statement.statements.size(); i++)
    {
        statement.statements[i]->accept(*this);
        if (this->task != nullptr)
        {
            this->tasks.push_back(std::move(this->task));
        }
    }}, statement.sourceLocation);
}

void Interpreter::visit(IfStatement& statement) 
{
    this->execute
    (
        [&]()
        {
            if (this->evaluator.getResult(*statement.condition).asBoolean())
            {
                statement.ifTrueStatements->accept(*this);
            }
            else
            {
                statement.ifFalseStatements->accept(*this);
            }
        },
        statement.sourceLocation
    );
}

void Interpreter::visit(ThenStatement& statement) 
{
    this->execute
    (
        [&]()
        {
            statement.first->accept(*this);
            std::unique_ptr<Task> first = std::move(this->task);
            
            statement.next->accept(*this);
            std::unique_ptr<Task> next = std::move(this->task);

            this->task = std::make_unique<SequenceTask>(std::move(first), std::move(next));
        },
        statement.sourceLocation
    );
}
