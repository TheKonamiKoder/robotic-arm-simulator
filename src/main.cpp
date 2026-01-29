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

sf::Vector2f coordinateToSFMLPosition(const Coordinate& coordinate)
{
    return sf::Vector2f(coordinate.x, -coordinate.y);   // -y because SFML has y axis going down as positive
}

void drawSegment(const Segment& segment, sf::RenderWindow& window) 
{
    std::array<Coordinate, 4> corners;

    corners[0] = Coordinate
    (
        segment.position.x + (segment.height / 2), 
        segment.position.y + (segment.width / 2)
    );
    corners[1] = Coordinate
    (
        segment.position.x - (segment.height / 2),
        segment.position.y + (segment.width / 2)
    );
    corners[2] = Coordinate
    (
        segment.position.x - (segment.height / 2),
        segment.position.y - (segment.width / 2)
    );
    corners[3] = Coordinate
    (
        segment.position.x + (segment.height / 2), 
        segment.position.y - (segment.width / 2)
    );

    sf::ConvexShape convex;
    convex.setPointCount(4);
    convex.setFillColor(sf::Color::Black);

    // Loop through and rotate all the corners so they are in their actual positions
    for (int i = 0; i < 4; i++)
    {
        corners[i].rotateAbout(segment.position, segment.getAngle());
        convex.setPoint(i, coordinateToSFMLPosition(corners[i]));
    }

    window.draw(convex);
}

void drawLinearActuator(const LinearActuator& linearActuator, sf::RenderWindow& window) 
{
    double pistonHeight = linearActuator.height / 20;
    double pistonWidth = linearActuator.width / 5;

    std::array<Coordinate, 4> base;
    std::array<Coordinate, 4> shaft;
    std::array<Coordinate, 4> head;

    double headHeight = linearActuator.height / 20;
    double shaftWidth = linearActuator.width / 10;

    base[0] = Coordinate
    (
        linearActuator.position.x + (linearActuator.height / 2), 
        linearActuator.position.y + (linearActuator.width / 2)
    );
    base[1] = Coordinate
    (
        linearActuator.position.x - (linearActuator.height / 2),
        linearActuator.position.y + (linearActuator.width / 2)
    );
    base[2] = Coordinate
    (
        linearActuator.position.x - (linearActuator.height / 2),
        linearActuator.position.y - (linearActuator.width / 2)
    );
    base[3] = Coordinate
    (
        linearActuator.position.x + (linearActuator.height / 2), 
        linearActuator.position.y - (linearActuator.width / 2)
    );

    shaft[0] = Coordinate
    (
        linearActuator.position.x + (linearActuator.height / 2) + linearActuator.getExtension() - headHeight, 
        linearActuator.position.y + shaftWidth
    );
    shaft[1] = Coordinate
    (
        linearActuator.position.x + (linearActuator.height / 2),
        linearActuator.position.y + shaftWidth
    );
    shaft[2] = Coordinate
    (
        linearActuator.position.x + (linearActuator.height / 2),
        linearActuator.position.y - shaftWidth
    );
    shaft[3] = Coordinate
    (
        linearActuator.position.x + (linearActuator.height / 2) + linearActuator.getExtension() - headHeight, 
        linearActuator.position.y - shaftWidth
    );
    
    head[0] = Coordinate
    (
        linearActuator.position.x + (linearActuator.height / 2) + linearActuator.getExtension(), 
        linearActuator.position.y + (linearActuator.width / 2)
    );
    head[1] = Coordinate
    (
        linearActuator.position.x + (linearActuator.height / 2) + linearActuator.getExtension() - headHeight,
        linearActuator.position.y + (linearActuator.width / 2)
    );
    head[2] = Coordinate
    (
        linearActuator.position.x + (linearActuator.height / 2) + linearActuator.getExtension() - headHeight,
        linearActuator.position.y - (linearActuator.width / 2)
    );
    head[3] = Coordinate
    (
        linearActuator.position.x + (linearActuator.height / 2) + linearActuator.getExtension(), 
        linearActuator.position.y - (linearActuator.width / 2)
    );

    // First draw shaft
    sf::ConvexShape shaftConvex;
    shaftConvex.setPointCount(4);
    shaftConvex.setFillColor(sf::Color::Black);

    for (int i = 0; i < 4; i++)
    {
        shaft[i].rotateAbout(linearActuator.position, linearActuator.getAngle());
        shaftConvex.setPoint(i, coordinateToSFMLPosition(shaft[i]));
    }

    window.draw(shaftConvex);
    
    // Next draw Head
    sf::ConvexShape headConvex;
    headConvex.setPointCount(4);
    headConvex.setFillColor(sf::Color::Black);

    for (int i = 0; i < 4; i++)
    {
        head[i].rotateAbout(linearActuator.position, linearActuator.getAngle());
        headConvex.setPoint(i, coordinateToSFMLPosition(head[i]));
    }

    window.draw(headConvex);

    // Next draw base
    sf::ConvexShape baseConvex;
    baseConvex.setPointCount(4);
    baseConvex.setFillColor(sf::Color::Black);

    for (int i = 0; i < 4; i++)
    {
        base[i].rotateAbout(linearActuator.position, linearActuator.getAngle());
        baseConvex.setPoint(i, coordinateToSFMLPosition(base[i]));
    }

    window.draw(baseConvex);
}

void drawMotor(const Motor& motor, sf::RenderWindow& window) 
{
    sf::CircleShape circle;
    circle.setPosition(sf::Vector2f(motor.position.x - motor.radius, -motor.position.y - motor.radius));
    circle.setRadius(motor.radius);
    circle.setFillColor(sf::Color::White);
    circle.setOutlineThickness(2);
    circle.setOutlineColor(sf::Color::Black);

    window.draw(circle);
}

void drawRobot(const Robot& robot, sf::RenderWindow& window)
{
    RobotComponent* currentComponent = robot.getBase();
    
    while (currentComponent != nullptr)
    {
        if (auto segment = dynamic_cast<Segment*>(currentComponent))
        {
            drawSegment(*segment, window);
        }
        else if (auto linear_actuator = dynamic_cast<LinearActuator*>(currentComponent))
        {
            drawLinearActuator(*linear_actuator, window);
        }
        else if (auto motor = dynamic_cast<Motor*>(currentComponent))
        {
            drawMotor(*motor, window);
        }

        currentComponent = currentComponent->front;
    }
    
}

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
        std::strncpy(robotComponentNameBuffer, UIState::instance().selectedComponent->name.c_str(), sizeof(robotComponentNameBuffer));

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
                        Coordinate position = UIState::instance().robot.getEndEffectorPosition(UIState::instance().selectedComponent->back);
                        //std::cout << position.x << " " << position.y << '\n';

                        //std::cout << UIState::instance().selectedComponent->back << '\n';

                        std::cout << UIState::instance().selectedComponent->back->position.x << " " 
                                  << UIState::instance().selectedComponent->back->position.y << '\n';

                        std::cout << UIState::instance().selectedComponent->back->getAngle() << '\n';

                        Motor phantom
                        (
                            0, 
                            "", 
                            UIState::instance().robot.getEndEffectorPosition(UIState::instance().selectedComponent->back), 
                            0, 
                            0, 
                            2 * M_PI, 
                            0
                        );
                        phantom.front = UIState::instance().selectedComponent;
                        phantom.rotate(rotatedAngle);
                    }
                }
                
            }

        ImGui::End();
    }  
}

void addSegment()
{
    Coordinate position(0, 0);
    
    // Calculate position of robot
    if (UIState::instance().robot.getBase() != nullptr)
    {
        position.x = UIState::instance().robot.getEndEffectorPosition().x + UIState::instance().height / 2;
        position.y = UIState::instance().robot.getEndEffectorPosition().y;

        position.rotateAbout
        (
            UIState::instance().robot.getEndEffectorPosition(), UIState::instance().robot.getEnd()->getAngle()
        );
    }

    double angle = 0;
    if (UIState::instance().robot.getBase() != nullptr)
    {
        angle = UIState::instance().robot.getEnd()->getAngle();
    }
    
    UIState::instance().robot.attachToEnd
    (
        new Segment
        (
            UIState::instance().robot.generateId(), 
            "Segment", 
            position, 
            angle, 
            UIState::instance().density * UIState::instance().height * UIState::instance().width, 
            UIState::instance().height, 
            UIState::instance().width
        )
    );
}

void addLinearActuator()
{
    Coordinate position(0, 0);
    
    // Calculate position of robot
    if (UIState::instance().robot.getBase() != nullptr)
    {
        position.x = UIState::instance().robot.getEndEffectorPosition().x + UIState::instance().height / 2;
        position.y = UIState::instance().robot.getEndEffectorPosition().y;

        position.rotateAbout
        (
            UIState::instance().robot.getEndEffectorPosition(), UIState::instance().robot.getEnd()->getAngle()
        );
    }

    double angle = 0;
    if (UIState::instance().robot.getBase() != nullptr)
    {
        angle = UIState::instance().robot.getEnd()->getAngle();
    }

    UIState::instance().robot.attachToEnd
    (
        new LinearActuator
        (
            UIState::instance().robot.generateId(), 
            "Linear Actuator", 
            position, 
            angle, 
            UIState::instance().density * UIState::instance().height * UIState::instance().width * 2,
            UIState::instance().height, 
            UIState::instance().height, 
            UIState::instance().width
        )
    );
}

void addMotor()
{
    Coordinate position(0, 0);
    
    // Calculate position of robot
    if (UIState::instance().robot.getBase() != nullptr)
    {
        position.x = UIState::instance().robot.getEndEffectorPosition().x + UIState::instance().width / 2;
        position.y = UIState::instance().robot.getEndEffectorPosition().y;

        position.rotateAbout(UIState::instance().robot.getEndEffectorPosition(), UIState::instance().robot.getEnd()->getAngle());
    }

    double angle = 0;
    if (UIState::instance().robot.getBase() != nullptr)
    {
        angle = UIState::instance().robot.getEnd()->getAngle();
    }

    UIState::instance().robot.attachToEnd
    (
        new Motor
        (
            UIState::instance().robot.generateId(), 
            "Linear Actuator", 
            position, 
            angle, 
            UIState::instance().density * M_PI * (UIState::instance().width / 2) * (UIState::instance().width / 2),
            2 * M_PI,
            UIState::instance().width / 2
        )
    );
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
                        addSegment();
                    }
                    
                    if (keyPressed->scancode == sf::Keyboard::Scancode::L)
                    {
                        addLinearActuator();
                    }

                    if (keyPressed->scancode == sf::Keyboard::Scancode::M)
                    {
                        addMotor();
                    }

                    UIState::instance().selectedComponent = UIState::instance().robot.getEnd();
                    UIState::instance().showRobotComponentInformation = true;
                }
            }
        }

        ImGui::SFML::Update(window, clock.restart());
        
        window.clear(sf::Color::White);

        if (UIState::instance().robot.getEnd() != nullptr)
        {
            robot_view.setCenter(coordinateToSFMLPosition(UIState::instance().robot.getEnd()->position));
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

        drawRobot(UIState::instance().robot, window);
        
        ImGui::SFML::Render(window);

        window.display();
    }

    ImGui::SFML::Shutdown();
}