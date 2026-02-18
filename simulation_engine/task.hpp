#pragma once

#include <string>
#include <vector>

class Task 
{
public:
    bool isFinished = false;
};

class LogTask : public Task
{
public:
    std::string message;

    LogTask(std::string message) : message(message) {}
};

class LogErrorTask : public LogTask 
{
public:
    LogErrorTask(std::string message) : LogTask(message) {}
};

class RaiseTask : public Task
{
public:
    std::string event;

    RaiseTask(std::string event) : event(event) {}
};

class RotateTask : public Task
{
public:
    std::string motorName;
    double angularDisplacement;

    RotateTask(std::string motorName, double angularDisplacement) 
        : motorName(motorName), angularDisplacement(angularDisplacement) {}
};

class TranslateTask : public Task
{
public:
    std::string linearActuatorName;
    double displacement;

    TranslateTask(std::string linearActuatorName, double linearActuator) 
        : linearActuatorName(linearActuatorName), displacement(displacement) {}
};

class SequenceTask : public Task
{
public:
    Task first;
    Task second;

    SequenceTask(Task first, Task second) : first(first), second(second) {}
};