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
		void translate(double displacement);
		double getExtension() const;
};