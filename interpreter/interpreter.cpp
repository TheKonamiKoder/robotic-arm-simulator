#include "interpreter.hpp"

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
        this->simulationEngine.addTask(LogErrorTask(std::string(error.what())));
    }
}

Interpreter::Interpreter(Environment& environment) : environment(environment), evaluator(Evaluator(evaluator)), simulationEngine(SimulationEngine()) {}

void Interpreter::visit(VariableDeclarationStatement& statement) 
{
    this->execute
    (
        [&](){this->environment.symbolTable.define(statement.name, this->evaluator.getResult(*statement.value));},
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
            this->task = RotateTask
            (
                this->evaluator.getResult(*statement.motorName).asString(), 
                this->evaluator.getResult(*statement.angularDisplacement).asNumber()
            );
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
            this->task = TranslateTask
            (
                this->evaluator.getResult(*statement.linearActuatorName).asString(),
                this->evaluator.getResult(*statement.displacement).asNumber()
            );
        },
        statement.sourceLocation
    );
}

void Interpreter::visit(RaiseStatement& statement) 
{
}

void Interpreter::visit(LogStatement& statement) 
{
    this->execute
    (
        [&](){this->task = LogTask(this->evaluator.getResult(*statement.message).toString());},
        statement.sourceLocation
    );
}

void Interpreter::visit(BlockStatement& statement) 
{
    for (int i = 0; i < statement.statements.size(); i++)
    {
        statement.statements[i]->accept(*this);
        this->tasks.push_back(this->task);
    }
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
            Task first = this->task;

            statement.next->accept(*this);
            Task next = this->task;

            this->task = SequenceTask(first, next);
        },
        statement.sourceLocation
    );
}
