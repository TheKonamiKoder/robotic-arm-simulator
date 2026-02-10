#include "robot_graphics.hpp"

#include <array>
#include <cmath>

#include <SFML/Graphics.hpp>
#include <imgui-SFML.h>
#include <imgui.h>

#include "coordinate.hpp"
#include "segment.hpp"
#include "linear_actuator.hpp"
#include "motor.hpp"
#include "robot.hpp"

sf::Vector2f coordinateToSFMLPosition(const Coordinate& coordinate)
{
    return sf::Vector2f(coordinate.x, -coordinate.y);   // -y because SFML has y axis going down as positive
}

void drawSegment(const Segment& segment, sf::RenderWindow& window, bool selected) 
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

    if (selected)
    {
        convex.setOutlineColor(sf::Color::Blue);
        convex.setOutlineThickness(2);
    }
    
    window.draw(convex);
}

void drawLinearActuator(const LinearActuator& linearActuator, sf::RenderWindow& window, bool selected) 
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

    if (selected)
    {
        sf::ConvexShape selectConvex;
        selectConvex.setPointCount(4);
        selectConvex.setFillColor(sf::Color::Transparent);
        selectConvex.setOutlineColor(sf::Color::Blue);
        selectConvex.setOutlineThickness(2);
    }

    window.draw(baseConvex);
}

void drawMotor(const Motor& motor, sf::RenderWindow& window, bool selected) 
{
    sf::CircleShape circle;
    circle.setPosition(sf::Vector2f(motor.position.x - motor.radius, -motor.position.y - motor.radius));
    circle.setRadius(motor.radius);
    circle.setFillColor(sf::Color::White);
    circle.setOutlineThickness(2);

    if (selected)
    {
        circle.setOutlineColor(sf::Color::Blue);
    }
    else
    {
        circle.setOutlineColor(sf::Color::Black);
    }

    window.draw(circle);
}

void drawRobot(const Robot& robot, sf::RenderWindow& window, const RobotComponent* selectedComponent)
{
    RobotComponent* currentComponent = robot.getBase();
    
    while (currentComponent != nullptr)
    {
        if (auto segment = dynamic_cast<Segment*>(currentComponent))
        {
            drawSegment(*segment, window, selectedComponent == segment);
        }
        else if (auto linear_actuator = dynamic_cast<LinearActuator*>(currentComponent))
        {
            drawLinearActuator(*linear_actuator, window, selectedComponent == linear_actuator);
        }
        else if (auto motor = dynamic_cast<Motor*>(currentComponent))
        {
            drawMotor(*motor, window, selectedComponent == motor);
        }

        currentComponent = currentComponent->front;
    }
}

void drawEndEffectors(const Robot& robot, sf::RenderWindow& window)
{
    sf::CircleShape circle;
    circle.setFillColor(sf::Color::Red);
    circle.setRadius(2);

    RobotComponent* currentComponent = robot.getBase();

    while (currentComponent != nullptr)
    {
        Coordinate endEffectorPosition = robot.getEndEffectorPosition(currentComponent);   
        circle.setPosition(sf::Vector2f(endEffectorPosition.x - 2, -endEffectorPosition.y - 2));
        window.draw(circle);
        currentComponent = currentComponent->front;
    }
}

void drawCentreOfMass(const Robot& robot, sf::RenderWindow& window)
{
    if (robot.getMass() != 0)
    {
        Coordinate centreOfMass = robot.getCentreOfMass();
        
        sf::CircleShape circle;
        circle.setFillColor(sf::Color::Blue);
        circle.setRadius(5);

        circle.setPosition(sf::Vector2f(centreOfMass.x - 5, -centreOfMass.y - 5));

        window.draw(circle);
    }
}
