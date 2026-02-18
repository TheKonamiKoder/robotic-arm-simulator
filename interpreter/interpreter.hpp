#pragma once

#include <functional>

#include "source_location.hpp"
#include "environment.hpp"
#include "evaluator.hpp"
#include "visitor.hpp"

#include "simulation_engine.hpp"
#include "task.hpp"


class Interpreter : public Visitor
{
private:
    // This function will execute the statement, but will also catch any errors
    void execute(std::function<void()> stuffToDo, SourceLocation sourceLocation = {});
public:
    Environment& environment;
    Evaluator& evaluator;
    SimulationEngine& simulationEngine;

    Task task;
    std::vector<Task> tasks;

    Interpreter(Environment& environment);

    void visit(VariableDeclarationStatement& statement) override;
    void visit(AssignmentStatement& statement) override;
    void visit(RotateStatement& statement) override;
    void visit(TranslateStatement& statement) override;
    void visit(RaiseStatement& statement) override;
    void visit(LogStatement& statement) override;
    void visit(BlockStatement& statement) override;
    void visit(IfStatement& statement) override;
    void visit(ThenStatement& statement) override;
};