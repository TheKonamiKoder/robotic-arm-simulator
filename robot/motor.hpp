#pragma once

#include <string>

#include "coordinate.hpp"
#include "robot_component.hpp"

class Motor: public RobotComponent
{
	private:
		double rotatedAngle = 0;
	protected:
		bool equals(const RobotComponent& other) const override;
	public:
		double maximumRotation;
		double radius;
	
		Motor() = default;
		Motor
		(
			id_t id, 
			std::string name, 
			Coordinate position, 
			double angle,
			double mass, 
			double maximumRotation, 
			double radius			
		);
		void rotate(double angularDisplacement);
		bool canRotateFully() const;
		double getRotatedAngle() const;
};