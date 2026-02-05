#define _USE_MATH_DEFINES

#include <array>
#include <cmath>
#include <iostream>

#include <SFML/Graphics.hpp>
#include <imgui-SFML.h>
#include <imgui.h>

#include "segment.hpp"
#include "linear_actuator.hpp"
#include "motor.hpp"
#include "robot.hpp"

#include "robot_graphics.hpp"

struct UIState
{
    bool showRobotInformation = false;
    bool showRobotComponentInformation = false;

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
            // if (auto motor = dynamic_cast<Motor*>(UIState::instance().selectedComponent))
            // {
            //     ImGui::Text("Maximum Rotation:");
            //     ImGui::SameLine();

            //     float maximumAngleRadians = motor->maximumRotation;
                
            //     if(ImGui::SliderAngle("##MotorMaximumRotationAngleSlider", &maximumAngleRadians, 0, 360))
            //     {
            //         motor->maximumRotation = maximumAngleRadians;
            //     }

            //     ImGui::Text("Radius:");
            //     ImGui::SameLine();

            //     double radius = motor->radius;
            //     if (ImGui::InputDouble("##MotorRadiusDoubleInput", &radius, 5, 10))
            //     {
            //         UIState::instance().width = radius * 2;

            //         // When radius changes, the positions of components attached to the motor will also change, to make
            //         // way for the bigger/smaller motor
                    
            //         double displacement = radius - motor->radius;

            //         // If displacement is positive, push attached components away from motor
            //         if (displacement > 0)
            //         {
            //             if (UIState::instance().robot.getEnd() != motor)
            //             {                        
            //                 LinearActuator frontPhantom
            //                 (
            //                     0, "", motor->front->position, motor->getAngle(), 0, displacement, 0, 0
            //                 );
            //                 frontPhantom.front = motor->front;
            //                 frontPhantom.translate(displacement);
            //             }

            //             if (UIState::instance().robot.getBase() != motor)
            //             {                        
            //                 LinearActuator backPhantom
            //                 (
            //                     0, "", motor->back->position, motor->back->getAngle() + M_PI, 0, displacement, 0, 0
            //                 );
            //                 backPhantom.front = motor->back;
            //                 backPhantom.translate(displacement, true);
            //             }
            //         }
            //         // If displacement is negative, push the attached components towards the motor
            //         else if (displacement < 0)
            //         {
            //             if (UIState::instance().robot.getEnd() != motor)
            //             {
            //                 // Linear actuator is attached to end and pushes down on all of the components
            //                 LinearActuator frontPhantom
            //                 (
            //                     0, 
            //                     "", 
            //                     UIState::instance().robot.getEnd()->position, 
            //                     motor->getAngle() + M_PI, 
            //                     0, 
            //                     -displacement, 
            //                     0, 
            //                     0
            //                 );
            //                 frontPhantom.front = UIState::instance().robot.getEnd();
            //                 // Temporarily disconnect motor from front so it doesn't get pushed
            //                 motor->front->back = nullptr;
            //                 frontPhantom.translate(-displacement, true);
            //                 motor->front->back = motor; // 
            //             }

            //             if (UIState::instance().robot.getBase() != motor)
            //             {
            //                 // Linear actuator is attached to front and pushes down on all the components
            //                 LinearActuator backPhantom
            //                 (
            //                     0,
            //                     "",
            //                     UIState::instance().robot.getBase()->position,
            //                     motor->back->getAngle(),
            //                     0,
            //                     -displacement,
            //                     0,
            //                     0
            //                 );
            //                 backPhantom.front = UIState::instance().robot.getBase();
            //                 backPhantom.translate(-displacement);
            //             }
            //         }
                    
            //     }
            // }
            

        ImGui::End();
    }  
}

// Will add a component to the robot
// Type is a char
// Can either be: 'S' for segment, 'L' for motor or 'M' for motor
void addComponent(char type)
{
    Coordinate position(0, 0);
    
    // Calculate position of robot
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

int main()
{
    sf::RenderWindow window = sf::RenderWindow(sf::VideoMode({1920u, 1080u}), "Robot Simulator");
    window.setFramerateLimit(144);
    if (!ImGui::SFML::Init(window))
    {
        return -1;
    }

    sf::View robot_view;
    robot_view.setCenter(sf::Vector2f(0, 0));
    robot_view.setSize(sf::Vector2f(window.getSize()));

    window.setView(robot_view);

    ImGuiIO& io = ImGui::GetIO();
    io.FontGlobalScale = 2.0f;   // Increase size of all text, so it is more readable

    sf::Clock clock;
    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent())
        {
            ImGui::SFML::ProcessEvent(window, *event);

            if (event->is<sf::Event::Closed>())
            {
                window.close();
            }

            if (const auto* resized = event->getIf<sf::Event::Resized>())
            {
                // Update the view to the new size of the window
                robot_view.setSize(sf::Vector2f(resized->size));
            }

            if (const auto* mouseWheelScrolled = event->getIf<sf::Event::MouseWheelScrolled>())
            {
                if (!io.WantCaptureMouse)
                {
                    UIState::instance().zoom += -mouseWheelScrolled->delta * 0.01;

                    // Clamp zoom to 0.1 and 10
                    if (UIState::instance().zoom < 0.1)
                    {
                        UIState::instance().zoom = 0.1;
                    }
                    else if (UIState::instance().zoom > 10)
                    {
                        UIState::instance().zoom = 10;
                    }
                    else
                    {
                        robot_view.zoom(1 - mouseWheelScrolled->delta * 0.1);
                    }
                }
            }

            if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>())
            {
                if (!io.WantCaptureKeyboard)
                {
                    if (keyPressed->scancode == sf::Keyboard::Scancode::S)
                    {
                        addComponent('S');
                        UIState::instance().showRobotComponentInformation = true;
                    }
                    
                    if (keyPressed->scancode == sf::Keyboard::Scancode::L)
                    {
                        addComponent('L');
                        UIState::instance().showRobotComponentInformation = true;
                    }

                    if (keyPressed->scancode == sf::Keyboard::Scancode::M)
                    {
                        addComponent('M');
                        UIState::instance().showRobotComponentInformation = true;
                    }

                    if (keyPressed->scancode == sf::Keyboard::Scancode::Delete)
                    {
                        removeComponent();
                    }

                    if (keyPressed->scancode == sf::Keyboard::Scancode::Left)
                    {
                        if (UIState::instance().robot.getBase() != UIState::instance().selectedComponent)
                        {
                            UIState::instance().selectedComponent = UIState::instance().selectedComponent->back;
                        }
                    }

                    if (keyPressed->scancode == sf::Keyboard::Scancode::Right)
                    {
                        if (UIState::instance().robot.getEnd() != UIState::instance().selectedComponent)
                        {
                            UIState::instance().selectedComponent = UIState::instance().selectedComponent->front;
                        }
                    }
                }
            }
        }

        ImGui::SFML::Update(window, clock.restart());
        
        window.clear(sf::Color::White);

        if (UIState::instance().selectedComponent != nullptr)
        {
            robot_view.setCenter(coordinateToSFMLPosition(UIState::instance().selectedComponent->position));
        }

        window.setView(robot_view);

        drawMenu();
        showOpenRobotFilePopup();
        showSaveRobotFilePopup();
        
        if (UIState::instance().showRobotInformation)
        {
            showRobotInformation();
        }

        if (UIState::instance().showRobotComponentInformation && UIState::instance().selectedComponent != nullptr)
        {
            showRobotComponentInformation();
        }

        drawRobot(UIState::instance().robot, window, UIState::instance().selectedComponent);
        drawEndEffectors(UIState::instance().robot, window);
        
        ImGui::SFML::Render(window);

        window.display();
    }

    ImGui::SFML::Shutdown();
}