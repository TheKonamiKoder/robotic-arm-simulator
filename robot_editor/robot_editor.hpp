#pragma once

#include "robot.hpp"

class RobotComponent;
class Coordinate;
class Segment;
class LinearActuator;
class Motor;
class Robot;

// This represents the global state of the robot editor
struct UIState
{
    bool showRobotInformation = false;
    bool showRobotComponentInformation = false;
    bool showRobotEditorSettings = false;
    bool showSaveRobotFilePopup = false;

    bool highlightSelectedComponent = true;
    bool showEndEffectors = true;
    bool showCentreOfMass = true;

    bool saved = true;

    double zoom = 1;

    double height = 100;
    double width = 50;
    double density = 0.001;

    Robot robot;
    RobotComponent* selectedComponent = nullptr;

    static UIState& instance()
    {
        static UIState inst;
        return inst;
    }
};

// This will draw the menu bar to the screen
void drawMenu();

// This will show popup that will ask the user to enter a file name of the robot that the user wants to load into the 
// application
void showOpenRobotFilePopup();

// This will show a popup that will ask the user to enter a file name where they would like to save the current robot
void showSaveRobotFilePopup();

// This will show the settings for the robot editor, such as whether to show the end effector or to highlight the
// selected component
void showRobotEditorSettings();

// This tool will show information about the robot, including its name, mass etc. and which robot components form part
// of it
void showRobotInformation();

// This tool will show infromation about the selected robot component
// It will also allow the user to edit properties of the selected robot component
void showRobotComponentInformation();

// This function will show segment specific information
// It will also be used within the robot information tool
void showSegmentInformation();

// This function will show linear actuator specific information
// It will also be used within the robot information tool
void showLinearActuatorInformation();

// This function will show motor specific information
// It will be a part of the robot infomration tool
void showMotorInformation();

// Will add a component to the robot
// The type can either be: 'S' for segment, 'L' for motor or 'M' for motor
void addComponent(char type);

// This will remove the selected component from the robot
void removeComponent();