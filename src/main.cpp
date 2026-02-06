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
#include "robot_editor.hpp"

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
                if (UIState::instance().saved)
                {
                    window.close();
                }
                else
                {
                    UIState::instance().showSaveRobotFilePopup = true;
                }
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

        if (UIState::instance().showSaveRobotFilePopup)
        {
            ImGui::OpenPopup("Save Robot File");
            UIState::instance().showSaveRobotFilePopup = false;
        }
        

        drawRobot(UIState::instance().robot, window, UIState::instance().selectedComponent);
        drawEndEffectors(UIState::instance().robot, window);
        drawCentreOfMass(UIState::instance().robot, window);
        
        ImGui::SFML::Render(window);

        window.display();
    }

    ImGui::SFML::Shutdown();
}