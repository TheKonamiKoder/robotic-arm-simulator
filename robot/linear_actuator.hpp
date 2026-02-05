#pragma once

#include <string>

#include "coordinate.hpp"
#include "robot_component.hpp"

class LinearActuator: public RobotComponent
{
	private:
		double extension = 0;
	protected:
		bool equals(const RobotComponent& other) const override;
	public:
		double maximumExtension;
		double height;
		double width;
		
		LinearActuator() = default;
		LinearActuator
		(
			id_t id, 
			std::string name, 
			Coordinate position, 
			double angle,
			double mass, 
			double maximumExtension, 
			double height, 
			double width
		);

		// This will translate the components in the robot
		void translate(double displacement);

		// The centre of mass for the linear actuator is slightly different to that of the other components (as the
		// extension also needs to be taken into account), so the method needs to be overridden.
		Coordinate getCentreOfMass() const override;
		double getExtension() const;
};