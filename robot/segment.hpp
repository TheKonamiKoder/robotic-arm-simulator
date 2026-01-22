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