#include "robot_component.hpp"

bool RobotComponent::equals(const RobotComponent& other) const
{
	return this->id == other.id 			&&
		   this->name == other.name 		&&
		   this->position == other.position &&
		   this->angle == other.angle 		&&
		   this->mass == other.mass;
}

RobotComponent::RobotComponent(id_t id, std::string name, Coordinate position, double angle, double mass)
	: id(id), name(name), position(position), angle(angle), mass(mass) {}

Coordinate RobotComponent::getCentreOfMass() const
{
	return this->position;
}

id_t RobotComponent::getId() const
{
	return this->id;
}

bool operator==(const RobotComponent& lhs, const RobotComponent& rhs)
{
	return lhs.equals(rhs);
}