#pragma once

#include <string>

#include "coordinate.hpp"

using id_t = int;	// Use type alias in case the internal datatype for id changes

class RobotComponent
{
	protected:
		id_t id;

		virtual bool equals(const RobotComponent& other) const;
	public:
		std::string name;
		Coordinate position;
		double angle;
		double mass;
		
		// Front and back pointers to create a linked list
		RobotComponent* front = nullptr;
		RobotComponent* back = nullptr;				
	
		RobotComponent() = default;
		RobotComponent(id_t id, std::string name, Coordinate position, double angle, double mass);

		virtual Coordinate getCentreOfMass() const;
		id_t getId() const;
		
		friend bool operator==(const RobotComponent& lhs, const RobotComponent& rhs);
};