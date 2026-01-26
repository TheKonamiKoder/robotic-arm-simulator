#include <array>

#include <SFML/Graphics.hpp>
#include <imgui-SFML.h>
#include <imgui.h>

#include "segment.hpp"
#include "linear_actuator.hpp"
#include "motor.hpp"
#include "robot.hpp"

struct UIFlags
{
    bool showRobotInformation = false;

    static UIFlags& instance()
    {
        static UIFlags inst;
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
        corners[i].rotateAbout(segment.position, segment.angle);
        convex.setPoint(i, coordinateToSFMLPosition(corners[i]));
    }

    window.draw(convex);
}

void drawLinearActuator(const LinearActuator& linearActuator, sf::RenderWindow& window) 
{
    double pistonHeight = linearActuator.height / 10;
    double pistonWidth = linearActuator.width / 5;

    std::array<Coordinate, 12> corners;

    corners[0] = Coordinate
    (
        linearActuator.position.x + (linearActuator.height / 2) + linearActuator.getExtension(), 
        linearActuator.position.y + (linearActuator.width / 2)
    );
    corners[1] = Coordinate
    (
        linearActuator.position.x + (linearActuator.height / 2) + linearActuator.getExtension() - pistonHeight, 
        linearActuator.position.y + (linearActuator.width / 2)
    );
    corners[2] = Coordinate
    (
        linearActuator.position.x + (linearActuator.height / 2) + linearActuator.getExtension() - pistonHeight, 
        linearActuator.position.y + (linearActuator.width / 2) - pistonWidth
    );
    corners[3] = Coordinate
    (
        linearActuator.position.x + (linearActuator.height / 2), 
        linearActuator.position.y + (linearActuator.width / 2) - pistonWidth
    );
    corners[4] = Coordinate
    (
        linearActuator.position.x + (linearActuator.height / 2), 
        linearActuator.position.y + (linearActuator.width / 2)
    );
    corners[5] = Coordinate
    (
        linearActuator.position.x - (linearActuator.height / 2), 
        linearActuator.position.y + (linearActuator.width / 2)
    );
    corners[6] = Coordinate
    (
        linearActuator.position.x - (linearActuator.height / 2), 
        linearActuator.position.y - (linearActuator.width / 2)
    );
    corners[7] = Coordinate
    (
        linearActuator.position.x + (linearActuator.height / 2), 
        linearActuator.position.y - (linearActuator.width / 2)
    );
    corners[8] = Coordinate
    (
        linearActuator.position.x + (linearActuator.height / 2), 
        linearActuator.position.y - (linearActuator.width / 2) + pistonWidth
    );
    corners[9] = Coordinate
    (
        linearActuator.position.x + (linearActuator.height / 2) + linearActuator.getExtension() - pistonHeight, 
        linearActuator.position.y - (linearActuator.width / 2) + pistonWidth
    );
    corners[10] = Coordinate
    (
        linearActuator.position.x + (linearActuator.height / 2) + linearActuator.getExtension() - pistonHeight, 
        linearActuator.position.y - (linearActuator.width / 2)
    );
    corners[11] = Coordinate
    (
        linearActuator.position.x + (linearActuator.height / 2) + linearActuator.getExtension(), 
        linearActuator.position.y - (linearActuator.width / 2)
    );

    sf::ConvexShape convex;
    convex.setPointCount(12);
    convex.setFillColor(sf::Color::Black);

    // Loop through and rotate all the corners so they are in their actual positions
    for (int i = 0; i < 12; i++)
    {
        corners[i].rotateAbout(linearActuator.position, linearActuator.angle);
        convex.setPoint(i, coordinateToSFMLPosition(corners[i]));
    }

    window.draw(convex);
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

void drawMenu(Robot& robot)
{
    bool openRobotFilePopup = false;
    bool saveRobotFilePopup = false;

    if (ImGui::BeginMainMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            if (ImGui::MenuItem("New")) 
            {
                robot.reset();
                UIFlags::instance().showRobotInformation = true;
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
                UIFlags::instance().showRobotInformation = true;
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
        ImGui::OpenPopup("Save Robot File Popup");
    }
}

void showOpenRobotFilePopup(Robot& robot)
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

            robot.loadFromFile(filename);

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

void showSaveRobotFilePopup(Robot& robot)
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

            robot.saveToFile(filename);

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

void showRobotInformation(Robot& robot)
{
    static char robotNameBuffer[128];
    std::strncpy(robotNameBuffer, robot.name.c_str(), sizeof(robotNameBuffer));

    ImGui::Begin("Robot Information", &UIFlags::instance().showRobotInformation);
        ImGui::Text("Robot Name:");
        ImGui::SameLine();
        if (ImGui::InputText("##RobotNameTextInput", robotNameBuffer, sizeof(robotNameBuffer)))
        {
            robot.name = robotNameBuffer;
        }
        
        ImGui::Text("Robot Mass:");
        ImGui::SameLine();
        ImGui::Text(std::to_string(robot.getMass()).c_str());

        if (robot.getBase() != nullptr)
        {
            ImGui::Text("Robot Components:");

            RobotComponent* currentComponent = robot.getBase();

            while (currentComponent != nullptr)
            {
                ImGui::Text(currentComponent->name.c_str());

                currentComponent = currentComponent->front;
            }   
        }
        
    ImGui::End();
}

int main()
{
    Robot robot;

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
        }

        ImGui::SFML::Update(window, clock.restart());
        
        window.clear(sf::Color::White);

        if (robot.getBase() != nullptr)
        {
            robot_view.setCenter(coordinateToSFMLPosition(robot.getBase()->position));
        }

        drawMenu(robot);
        showOpenRobotFilePopup(robot);
        showSaveRobotFilePopup(robot);
        
        if (UIFlags::instance().showRobotInformation)
        {
            showRobotInformation(robot);
        }

        drawRobot(robot, window);
        
        ImGui::SFML::Render(window);

        window.display();
    }

    ImGui::SFML::Shutdown();
}