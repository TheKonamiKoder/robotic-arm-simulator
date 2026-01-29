#pragma once

#include <string>

#include "coordinate.hpp"

using id_t = int;	// Use type alias in case the internal datatype for id changes

class RobotComponent
{
	protected:
		id_t id;
		double angle;	// This angle will remain normalised so that it is in range [0, 2 pi)

		// This function is responsible for ensuring that the angle is within range.
		double normalise(double angle) const;

		virtual bool equals(const RobotComponent& other) const;
	public:
		std::string name;
		Coordinate position;
		double mass;
		
		// Front and back pointers to create a linked list
		RobotComponent* front = nullptr;
		RobotComponent* back = nullptr;				
	
		RobotComponent() = default;
		RobotComponent(id_t id, std::string name, Coordinate position, double angle, double mass);

		// Setter will also normalise the angle
		void setAngle(double angle);

		// This will change the angle by the angular displacement (and ensure it is normalised)
		void rotateAngle(double angularDisplacement);

		// Will return the position of the centre of mass of the robot
		virtual Coordinate getCentreOfMass() const;
		id_t getId() const;
		double getAngle() const;
		
		friend bool operator==(const RobotComponent& lhs, const RobotComponent& rhs);
};