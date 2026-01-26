#include <SFML/Graphics.hpp>
#include <imgui-SFML.h>
#include <imgui.h>

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

void drawMotor(const Motor& motor, sf::CircleShape& circleShape) 
{
    circleShape.setPosition(sf::Vector2f(motor.position.x - motor.radius, -motor.position.y - motor.radius));
    circleShape.setRadius(motor.radius);
    circleShape.setFillColor(sf::Color::White);
    circleShape.setOutlineThickness(2);
    circleShape.setOutlineColor(sf::Color::Black);
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
        ImGui::OpenPopup("OpenRobotFilePopup");
    }

    if (saveRobotFilePopup)
    {
        ImGui::OpenPopup("SaveRobotFilePopup");
    }
}

void showOpenRobotFilePopup(Robot& robot)
{
    static char filenameBuffer[256] = "";

    if (ImGui::BeginPopupModal("OpenRobotFilePopup", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
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

    if (ImGui::BeginPopupModal("SaveRobotFilePopup", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
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
    sf::RenderWindow window = sf::RenderWindow(sf::VideoMode({1920u, 1080u}), "Robot Simulator");
    window.setFramerateLimit(144);
    if (!ImGui::SFML::Init(window))
    {
        return -1;
    }

    ImGuiIO& io = ImGui::GetIO();
    io.FontGlobalScale = 2.0f;   // Increase size of all text, so it is more readable

    Robot robot;

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
        }

        ImGui::SFML::Update(window, clock.restart());
        

        window.clear(sf::Color::White);

        drawMenu(robot);
        showOpenRobotFilePopup(robot);
        showSaveRobotFilePopup(robot);
        
        if (UIFlags::instance().showRobotInformation)
        {
            showRobotInformation(robot);
        }
        
        ImGui::SFML::Render(window);

        window.display();
    }

    ImGui::SFML::Shutdown();
}