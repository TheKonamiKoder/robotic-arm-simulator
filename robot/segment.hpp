#pragma once

#include <string>
#include "coordinate.hpp"
#include "robot_component.hpp"

class Segment: public RobotComponent
{
	protected:
		bool equals(const RobotComponent& other) const override;
	public:
		double height;
		double width;
	
		Segment() = default;
		Segment(id_t id, std::string name, Coordinate position, double angle, double mass, double height, double width);
};

bool Segment::equals(const RobotComponent& other) const
{
	const Segment& segment = static_cast<const Segment&>(other);

	return RobotComponent::equals(other) 	&&
		   this->height == segment.height 	&&
		   this->width == segment.width;
}

Segment::Segment(id_t id, std::string name, Coordinate position, double angle, double mass, double height, double width)
	: RobotComponent(id, name, position, angle, mass), height(height), width(width) {}