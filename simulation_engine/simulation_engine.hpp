#pragma once

#include <vector>
#include <unordered_map>

#include "task.hpp"

#include "robot.hpp"

class SimulationEngine
{
private:
    std::vector<Task> currentTasks;
public:
    Robot* robot;
    double angularDisplacementSpeed = 0.1;
    double displacementSpeed = 0.1;

    std::unordered_map<std::string, std::vector<Task>> programMap;

    void update();
    
    void simulateLogTask(LogTask task);
    void simulateLogErrorTask(LogErrorTask task);
    void simulateRaiseTask(RaiseTask task);
    void simulateRotateTask(RotateTask task);
    void simulateTranslateTask(TranslateTask task);

    void addTask(Task task);
};