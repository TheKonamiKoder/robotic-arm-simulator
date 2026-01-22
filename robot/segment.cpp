#include "segment.hpp"

bool Segment::equals(const RobotComponent& other) const
{
	const Segment& segment = static_cast<const Segment&>(other);

	return RobotComponent::equals(other) 	&&
		   this->height == segment.height 	&&
		   this->width == segment.width;
}

Segment::Segment(id_t id, std::string name, Coordinate position, double angle, double mass, double height, double width)
	: RobotComponent(id, name, position, angle, mass), height(height), width(width) {}