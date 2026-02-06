#include "robot_editor.hpp"

#include <SFML/Graphics.hpp>
#include <imgui-SFML.h>
#include <imgui.h>

#include "segment.hpp"
#include "linear_actuator.hpp"
#include "motor.hpp"
#include "robot.hpp"

void drawMenu()
{
    bool openRobotFilePopup = false;
    bool saveRobotFilePopup = false;

    if (ImGui::BeginMainMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            if (ImGui::MenuItem("New")) 
            {
                UIState::instance().selectedComponent = nullptr;
                UIState::instance().showRobotComponentInformation = false;
                UIState::instance().robot.reset();
                UIState::instance().showRobotInformation = true;
            }
            
            if (ImGui::MenuItem("Open"))
            {
                openRobotFilePopup = true;
            }

            if (ImGui::MenuItem("Save"))
            {
                saveRobotFilePopup = true;
            }
            
            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("View"))
        {
            if (ImGui::MenuItem("Robot Information"))
            {
                UIState::instance().showRobotInformation = true;
            }
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }

    if (openRobotFilePopup)
    {
        ImGui::OpenPopup("Open Robot File");
    }

    if (saveRobotFilePopup)
    {
        ImGui::OpenPopup("Save Robot File");
    }
}

void showOpenRobotFilePopup()
{
    static char filenameBuffer[256] = "";

    if (ImGui::BeginPopupModal("Open Robot File", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
    {
        ImGui::Text("Enter file name:");
        ImGui::SameLine();
        ImGui::InputText("##FileNameInput", filenameBuffer, sizeof(filenameBuffer));

        ImGui::Spacing();

        if (ImGui::Button("OK", ImVec2(120, 0)))
        {
            std::string filename = filenameBuffer;

            UIState::instance().robot.loadFromFile(filename);

            ImGui::CloseCurrentPopup();
        }

        ImGui::SameLine();

        if (ImGui::Button("Cancel", ImVec2(120, 0)))
        {
            ImGui::CloseCurrentPopup();
        }

        ImGui::EndPopup();
    }
}

void showSaveRobotFilePopup()
{
    static char filenameBuffer[256] = "";

    if (ImGui::BeginPopupModal("Save Robot File", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
    {
        ImGui::Text("Enter file name:");
        ImGui::SameLine();
        ImGui::InputText("##FileNameInput", filenameBuffer, sizeof(filenameBuffer));

        ImGui::Spacing();

        if (ImGui::Button("OK", ImVec2(120, 0)))
        {
            std::string filename = filenameBuffer;

            UIState::instance().robot.saveToFile(filename);

            ImGui::CloseCurrentPopup();
        }

        ImGui::SameLine();

        if (ImGui::Button("Cancel", ImVec2(120, 0)))
        {
            ImGui::CloseCurrentPopup();
        }

        ImGui::EndPopup();
    }
}

void showRobotInformation()
{
    static char robotNameBuffer[128];
    std::strncpy(robotNameBuffer, UIState::instance().robot.name.c_str(), sizeof(robotNameBuffer));

    ImGui::Begin("Robot Information", &UIState::instance().showRobotInformation);
        ImGui::Text("Robot Name:");
        ImGui::SameLine();
        if (ImGui::InputText("##RobotNameTextInput", robotNameBuffer, sizeof(robotNameBuffer)))
        {
            UIState::instance().robot.name = robotNameBuffer;
        }
        
        ImGui::Text("Robot Mass:");
        ImGui::SameLine();
        ImGui::Text(std::to_string(UIState::instance().robot.getMass()).c_str());

        if (UIState::instance().robot.getBase() != nullptr)
        {
            ImGui::Text("Robot Components:");

            RobotComponent* currentComponent = UIState::instance().robot.getBase();

            while (currentComponent != nullptr)
            {
                ImGui::Text(currentComponent->name.c_str());

                currentComponent = currentComponent->front;
            }   
        }
    ImGui::End();
}

void showRobotComponentInformation()
{
    if (UIState::instance().selectedComponent != nullptr)
    {
        static char robotComponentNameBuffer[128];
        std::strncpy
        (
            robotComponentNameBuffer, 
            UIState::instance().selectedComponent->name.c_str(), 
            sizeof(robotComponentNameBuffer)
        );

        ImGui::Begin("Robot Component Information", &UIState::instance().showRobotComponentInformation);
            ImGui::Text("Component Name:");
            ImGui::SameLine();
            if (ImGui::InputText("##RobotComponentNameTextInput", robotComponentNameBuffer, sizeof(robotComponentNameBuffer)))
            {
                UIState::instance().selectedComponent->name = robotComponentNameBuffer;    
            }

            ImGui::Text("Component Mass:");
            ImGui::SameLine();
            ImGui::InputDouble("##RobotComponentMassDoubleInput", &UIState::instance().selectedComponent->mass, 0.25, 0.1);

            float angleRadians = UIState::instance().selectedComponent->getAngle();

            if (angleRadians > M_PI)
            {
                angleRadians -= 2 * M_PI;
            }

            ImGui::Text("Component Angle:");
            ImGui::SameLine();
            if (ImGui::SliderAngle("##RobotComponentAngleSlider", &angleRadians, -180, 180))
            {
                float rotatedAngle = angleRadians - UIState::instance().selectedComponent->getAngle();

                // Add "phantom" motor to rotate rest of robot
                if (UIState::instance().robot.getBase() == UIState::instance().selectedComponent)
                {
                    Motor phantom = Motor(0, "", UIState::instance().selectedComponent->position, 0, 0, 2 * M_PI, 0);
                    phantom.front = UIState::instance().selectedComponent;
                    phantom.rotate(rotatedAngle);
                }
                else
                {
                    if (auto motor = dynamic_cast<Motor*>(UIState::instance().selectedComponent))
                    {
                        Motor phantom = Motor(0, "", UIState::instance().selectedComponent->position, 0, 0, 2 * M_PI, 0);
                        phantom.front = UIState::instance().selectedComponent;
                        phantom.rotate(rotatedAngle);
                    }
                    else
                    {
                        Coordinate position = UIState::instance()
                                                .robot
                                                .getEndEffectorPosition(UIState::instance().selectedComponent->back);

                        // Phantom motor is necessary in order to ensure that all the components attached to it will
                        // also rotate
                        Motor phantom(0, "", position, 0, 0, 2 * M_PI, 0);
                        phantom.front = UIState::instance().selectedComponent;
                        phantom.rotate(rotatedAngle);
                    }
                }
            }

            ImGui::Text
            (
                "Component position: (%.2f, %.2f)", 
                UIState::instance().selectedComponent->position.x, UIState::instance().selectedComponent->position.y
            );

            // Different information will be shown depending on the type of component
            if (auto segment = dynamic_cast<Segment*>(UIState::instance().selectedComponent))
            {
                showSegmentInformation();
            }
            else if (auto motor = dynamic_cast<Motor*>(UIState::instance().selectedComponent))
            {
               showMotorInformation(); 
            }
            else if (auto linearActuator = dynamic_cast<LinearActuator*>(UIState::instance().selectedComponent))
            {
                showLinearActuatorInformation();
            }
            
        ImGui::End();
    }  
}

void showSegmentInformation()
{
    Segment* segment = static_cast<Segment*>(UIState::instance().selectedComponent);

    ImGui::Text("Width:");
    ImGui::SameLine();

    double width = segment->width;
    if (ImGui::InputDouble("##SegmentWidthDoubleInput", &width, 5, 10))
    {
        if (width > 0)
        {
            segment->width = width;

            UIState::instance().width = width;
        }
    }

    ImGui::Text("Height:");
    ImGui::SameLine();

    double height = segment->height;
    if (ImGui::InputDouble("##SegmentHeightDoubleInput", &height, 5, 10))
    {
        if (height > 0)
        {
            double displacement = (height - segment->height) / 2;

            segment->height = height;

            UIState::instance().height = height;

            // Calculate dx and dy to change the position of the segment
            double segment_dx = 0;
            double segment_dy = 0;

            // Segment only needs to change position if it is not the base component
            if (segment->back != nullptr)
            {
                segment_dx = displacement * cos(segment->getAngle());
                segment_dy = displacement * sin(segment->getAngle());
            }

            segment->position.x += segment_dx;
            segment->position.y += segment_dy;

            double component_dx = displacement * cos(segment->getAngle());
            double component_dy = displacement * sin(segment->getAngle());

            RobotComponent* currentComponent = segment->front;

            while (currentComponent != nullptr)
            {
                currentComponent->position.x += segment_dx + component_dx;
                currentComponent->position.y += segment_dy + component_dy;

                currentComponent = currentComponent->front;
            }
        }
    }
}

void showLinearActuatorInformation()
{
    LinearActuator* linearActuator = static_cast<LinearActuator*>(UIState::instance().selectedComponent);

    ImGui::Text("Maximum Extension:");
    ImGui::SameLine();

    double maximumExtension = linearActuator->maximumExtension;
    if (ImGui::InputDouble("##LinearActuatorMaximumExtensionDoubleInput", &maximumExtension, 2.5, 5))
    {
        if (maximumExtension > 0)
        {
            linearActuator->maximumExtension = maximumExtension;
        }
    }

    ImGui::Text("Width:");
    ImGui::SameLine();

    double width = linearActuator->width;
    if (ImGui::InputDouble("##LinearActuatorWidthDoubleInput", &width, 5, 10))
    {
        if (width > 0)
        {
            linearActuator->width = width;

            UIState::instance().width = width;
        }
    }

    ImGui::Text("Height:");
    ImGui::SameLine();

    double height = linearActuator->height;
    if (ImGui::InputDouble("##LinearActuatorHeightDoubleInput", &height, 5, 10))
    {
        if (height > 0)
        {
            double displacement = (height - linearActuator->height) / 2;

            linearActuator->height = height;

            UIState::instance().height = height;

            // Calculate dx and dy to change the position of the segment
            double linearActuator_dx = 0;
            double linearActuator_dy = 0;

            // Segment only needs to change position if it is not the base component
            if (linearActuator->back != nullptr)
            {
                linearActuator_dx = displacement * cos(linearActuator->getAngle());
                linearActuator_dy = displacement * sin(linearActuator->getAngle());
            }

            linearActuator->position.x += linearActuator_dx;
            linearActuator->position.y += linearActuator_dy;

            double component_dx = displacement * cos(linearActuator->getAngle());
            double component_dy = displacement * sin(linearActuator->getAngle());

            RobotComponent* currentComponent = linearActuator->front;

            while (currentComponent != nullptr)
            {
                currentComponent->position.x += linearActuator_dx + component_dx;
                currentComponent->position.y += linearActuator_dy + component_dy;

                currentComponent = currentComponent->front;
            }
        }
    }
}

void showMotorInformation()
{
    Motor* motor = static_cast<Motor*>(UIState::instance().selectedComponent);

    ImGui::Text("Maximum Rotation:");
    ImGui::SameLine();

    float maximumAngleRadians = motor->maximumRotation;
    if (ImGui::SliderAngle("##MotorMaximumRotationAngleSlider", &maximumAngleRadians, 0, 360))
    {
        motor->maximumRotation = maximumAngleRadians;
    }

    ImGui::Text("Radius:");
    ImGui::SameLine();

    double radius = motor->radius;
    if (ImGui::InputDouble("##MotorRadiusDoubleInput", &radius, 5, 10))
    {
        // Input verification so that the user does not give a motor 0 radius or negative, which would break the system
        // Therefore the code only executes if radius is positive
        // Otherwise, the radius of the motor stays the same
        if (radius > 0)
        {
            double displacement = radius - motor->radius;
            
            motor->radius = radius;

            // Change UIState height and width in order to keep everything in proportion
            UIState::instance().height = (UIState::instance().height / UIState::instance().width) * (radius * 2);
            UIState::instance().width = radius * 2;

            // When radius changes, the positions of components attached to the motor will also change, to make
            // way for the bigger/smaller motor

            // Calculate dx and dy to change the position of the motor
            double motor_dx = 0;
            double motor_dy = 0;

            // Motor only needs to change position if it is not the base component
            if (motor->back != nullptr)
            {
                motor_dx = displacement * cos(motor->back->getAngle());
                motor_dy = displacement * sin(motor->back->getAngle());   
            }
            
            motor->position.x += motor_dx;
            motor->position.y += motor_dy;
            
            double component_dx = displacement * cos(motor->getAngle());
            double component_dy = displacement * sin(motor->getAngle());

            RobotComponent* currentComponent = motor->front;

            while (currentComponent != nullptr)
            {
                currentComponent->position.x += motor_dx + component_dx;
                currentComponent->position.y += motor_dy + component_dy;

                currentComponent = currentComponent->front;
            }
        }
    }
}

void addComponent(char type)
{
    Coordinate position(0, 0);
    
    // Calculate position of robot, if it is not the first component
    if (UIState::instance().selectedComponent != nullptr)
    {
        Coordinate endEffectorPosition = UIState::instance()
                                            .robot
                                            .getEndEffectorPosition(UIState::instance().selectedComponent);

        if ((type == 'S') || (type == 'L'))
        {
            position.x = endEffectorPosition.x + UIState::instance().height / 2;
        }
        else if (type == 'M')
        {
            position.x = endEffectorPosition.x + UIState::instance().width / 2;
        }
        
        position.y = endEffectorPosition.y;

        position.rotateAbout
        (
            endEffectorPosition, 
            UIState::instance().selectedComponent->getAngle()
        );
    }

    double angle = 0;
    if (UIState::instance().selectedComponent != nullptr)
    {
        angle = UIState::instance().selectedComponent->getAngle();
    }

    RobotComponent* robotComponent;
        
    switch (type)
    {
    case 'S':
        robotComponent = new Segment
        (
            UIState::instance().robot.generateId(), 
            "Segment", 
            position, 
            angle, 
            UIState::instance().density * UIState::instance().height * UIState::instance().width, 
            UIState::instance().height, 
            UIState::instance().width
        );
        break;
    case 'L':
        robotComponent = new LinearActuator
        (
            UIState::instance().robot.generateId(), 
            "Linear Actuator", 
            position, 
            angle, 
            UIState::instance().density * UIState::instance().height * UIState::instance().width * 2,
            UIState::instance().height, 
            UIState::instance().height, 
            UIState::instance().width
        );
        break;
    case 'M':
        robotComponent = new Motor
        (
            UIState::instance().robot.generateId(), 
            "Motor", 
            position, 
            angle, 
            UIState::instance().density * M_PI * (UIState::instance().width / 2) * (UIState::instance().width / 2),
            2 * M_PI,
            UIState::instance().width / 2
        );
        break;
    default:
        break;
    }

    if (UIState::instance().selectedComponent != nullptr)
    {
        if ((type == 'S') || (type == 'L'))
        {
            LinearActuator phantom(0, "", position, angle, 0, UIState::instance().height, 0, 0);
            phantom.front = UIState::instance().selectedComponent->front;
            phantom.translate(UIState::instance().height);
        }
        else if (type == 'M')
        {
            LinearActuator phantom(0, "", position, angle, 0, UIState::instance().width, 0, 0);
            phantom.front = UIState::instance().selectedComponent->front;
            phantom.translate(UIState::instance().height);
        }

        UIState::instance().robot.attachTo(UIState::instance().selectedComponent, robotComponent);
    }
    else
    {
        UIState::instance().robot.attachToEnd(robotComponent);
    }

    UIState::instance().selectedComponent = robotComponent;
}

void removeComponent()
{
    double dx = 0; 
    double dy = 0;

    RobotComponent* robotComponent = UIState::instance().selectedComponent;

    if (UIState::instance().robot.getBase() == robotComponent)
    {
        UIState::instance().selectedComponent = robotComponent->front;

        // Need to normalise coordinates so that the new base has position (0, 0)
        if (UIState::instance().selectedComponent != nullptr)
        {
            dx = -UIState::instance().selectedComponent->position.x;
            dy = -UIState::instance().selectedComponent->position.y;

            UIState::instance().selectedComponent->position = Coordinate(0, 0);
        }
    }
    else
    {
        UIState::instance().selectedComponent = robotComponent->back;

        // Find vector that each robot component needs to be translated by so that components attached correctly
        if (UIState::instance().selectedComponent != nullptr)
        {
            Coordinate o = UIState::instance().robot.getEndEffectorPosition(UIState::instance().selectedComponent);
            Coordinate a = UIState::instance().robot.getEndEffectorPosition(robotComponent);

            dx = o.x - a.x;
            dy = o.y - a.y;
        }
    }

    // Remove component from logical representation of robot
    UIState::instance().robot.remove(robotComponent);

    if (UIState::instance().selectedComponent != nullptr)
    {
        // Remove component from visual representation of robot
        RobotComponent* currentComponent = UIState::instance().selectedComponent->front;
        
        while (currentComponent != nullptr)
        {
            currentComponent->position.x += dx;
            currentComponent->position.y += dy;

            currentComponent = currentComponent->front;
        }
    }
}