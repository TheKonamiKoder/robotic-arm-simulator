#pragma once

#define _USE_MATH_DEFINES

#include <string>
#include <unordered_map>

#include "coordinate.hpp"
#include "robot_component.hpp"

class Robot
{
private:
	id_t id;
	double mass = 0;
	std::unordered_map<id_t, RobotComponent*> robotComponentsMap;
	RobotComponent* base = nullptr;
	RobotComponent* end = nullptr;
public:
	std::string name = "";

	Robot() = default;
	Robot(id_t id);
	Robot(id_t id, std::string name);
	~Robot();
	id_t generateId() const;
	void attachToEnd(RobotComponent* robotComponent);
	void attachTo(RobotComponent* to, RobotComponent* next);
	void attachBaseTo(RobotComponent* robotComponent);
	void remove(RobotComponent* robotComponent);
	void saveToFile(const std::string& filename) const;
	void loadFromFile(const std::string& filename);
	Coordinate getCentreOfMass() const;
	id_t getId() const;
	double getMass() const;
	RobotComponent* getComponentFromId(id_t id);
	RobotComponent* getBase() const;
	RobotComponent* getEnd() const;
};