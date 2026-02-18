#include "simulation_engine.hpp"

#include <algorithm>
#include <iostream>

#include "robot_component.hpp"
#include "motor.hpp"
#include "linear_actuator.hpp"

void SimulationEngine::update()
{
    // Remove finished tasks
    this->currentTasks.erase
    (
        std::remove_if
        (
            this->currentTasks.begin(),
            this->currentTasks.end(),
            [](Task* task) { return task->isFinished; }
        ),
        this->currentTasks.end()
    );

    for (int i = 0; i < this->currentTasks.size(); i++)
    {
        this->doTask(*this->currentTasks[i]);
    }
}

void SimulationEngine::doTask(Task& task)
{
    if (auto logErrorTask = dynamic_cast<LogErrorTask*>(&task))
    {
        //std::cout << "Log task:\n"; 
        this->doLogErrorTask(*logErrorTask);
    }
    else if (auto logTask = dynamic_cast<LogTask*>(&task))
    {
        //std::cout << "Log Error task: " + logTask->message + '\n';
        this->doLogTask(*logTask);
    }
    else if (auto raiseTask = dynamic_cast<RaiseTask*>(&task))
    {
        //std::cout << "Raise task:\n";
        this->doRaiseTask(*raiseTask);
    }
    else if (auto rotateTask = dynamic_cast<RotateTask*>(&task))
    {
        //std::cout << "Rotate task:\n";
        this->doRotateTask(*rotateTask);
    }
    else if (auto translateTask = dynamic_cast<TranslateTask*>(&task))
    {
        //std::cout << "Translate task:\n";
        this->doTranslateTask(*translateTask);
    }
    else if (auto sequenceTask = dynamic_cast<SequenceTask*>(&task))
    {
        //std::cout << "Sequence task:\n";

        if (!sequenceTask->first->isFinished)
        {
            //std::cout << "Doing the first task\n";
            this->doTask(*sequenceTask->first);
        }
        else if (!sequenceTask->second->isFinished)
        {
            //std::cout << "Doing the second task\n";   
            this->doTask(*sequenceTask->second);
        }
        else
        {
            sequenceTask->isFinished = true;
        }
    }
}

void SimulationEngine::doLogErrorTask(LogErrorTask& task)
{
    std::cout << task.message << '\n';
    task.isFinished = true;
}

void SimulationEngine::doLogTask(LogTask& task)
{
    std::cout << task.message << '\n';
    task.isFinished = true;
}

void SimulationEngine::doRaiseTask(RaiseTask& task)
{
    this->raiseEvent(task.event);
}

void SimulationEngine::doRotateTask(RotateTask& task)
{
    if (this->robot == nullptr)
    {
        return;
    }

    RobotComponent* current = this->robot->getBase();
    Motor* motor = nullptr;

    while (current)
    {
        if (current->name == task.motorName)
        {
            if (auto* m = dynamic_cast<Motor*>(current))
            {
                motor = m;
                break;
            }
        }
        current = current->front;
    }

    if (!motor)
        return;

    bool targetReached =
        (task.angularDisplacement >= 0 && task.actualAngularDisplacement >= task.angularDisplacement) ||
        (task.angularDisplacement < 0  && task.actualAngularDisplacement <= task.angularDisplacement);

    if (targetReached)
    {
        task.isFinished = true;
        return;
    }

    double step = (task.angularDisplacement >= 0)
                  ? this->angularDisplacementSpeed
                  : -this->angularDisplacementSpeed;

    motor->rotate(step);
    task.actualAngularDisplacement += step;
}

void SimulationEngine::doTranslateTask(TranslateTask& task)
{
    if (!this->robot)
        return;

    RobotComponent* current = this->robot->getBase();
    LinearActuator* actuator = nullptr;

    // Find the actuator by name
    while (current)
    {
        if (current->name == task.linearActuatorName)
        {
            if (auto* a = dynamic_cast<LinearActuator*>(current))
            {
                actuator = a;
                break;
            }
        }
        current = current->front;
    }

    if (!actuator)
        return;

    // Check if we've reached the target displacement
    bool targetReached =
        (task.displacement >= 0 && task.actualDisplacement >= task.displacement) ||
        (task.displacement < 0  && task.actualDisplacement <= task.displacement);

    //std::cout << task.actualDisplacement << ' ' << task.displacement << '\n';

    if (targetReached)
    {
        //std::cout << "Finished target!";
        task.isFinished = true;
        return;
    }

    // Determine step direction
    double step = (task.displacement >= 0)
                  ? this->displacementSpeed
                  : -this->displacementSpeed;

    // Apply movement
    actuator->translate(step);
    task.actualDisplacement += step;
}

void SimulationEngine::raiseEvent(const std::string& event)
{
    // std::cout << "Raising event " + event + "\n";
    if (this->programMap.find(event) != this->programMap.end())
    {
        currentTasks.clear();

        for (int i = 0; i < this->programMap.at(event).size(); i++)
        {
            this->programMap.at(event)[i]->reset();
            currentTasks.push_back(this->programMap.at(event)[i].get());
        }
    }    
}