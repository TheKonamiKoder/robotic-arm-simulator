#pragma once

#include <vector>
#include <unordered_map>

#include "task.hpp"

#include "robot.hpp"

class SimulationEngine
{
private:
    std::vector<Task*> currentTasks;

    void doLogTask(LogTask& task);
    void doLogErrorTask(LogErrorTask& task);
    void doRaiseTask(RaiseTask& task);
    void doRotateTask(RotateTask& task);
    void doTranslateTask(TranslateTask& task);
public:
    Robot* robot;
    double angularDisplacementSpeed = 0.1;
    double displacementSpeed = 1;

    std::unordered_map<std::string, std::vector<std::unique_ptr<Task>>> programMap;

    void update();

    void doTask(Task& task);
    void raiseEvent(const std::string& event);
};