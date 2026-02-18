#pragma once

#include <memory>
#include <string>
#include <vector>

class Task 
{
public:
    bool isFinished = false;

    virtual ~Task() = default;
    virtual void reset() {this->isFinished = false;}
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
    double actualAngularDisplacement = 0;

    RotateTask(std::string motorName, double angularDisplacement) 
        : motorName(motorName), angularDisplacement(angularDisplacement) {}

    void reset() override {this->isFinished = false; this->actualAngularDisplacement = 0;}
};

class TranslateTask : public Task
{
public:
    std::string linearActuatorName;
    double displacement;
    double actualDisplacement = 0;

    TranslateTask(std::string linearActuatorName, double displacement) 
        : linearActuatorName(linearActuatorName), displacement(displacement) {}

    void reset() override {this->isFinished = false; this->actualDisplacement = 0;}
};

class SequenceTask : public Task
{
public:
    std::unique_ptr<Task> first;
    std::unique_ptr<Task> second;

    SequenceTask(std::unique_ptr<Task> first, std::unique_ptr<Task> second) 
        : first(std::move(first)), second(std::move(second)) {}
    
    void reset() override {this->isFinished = false; this->first->reset(); this->second->reset();}
};