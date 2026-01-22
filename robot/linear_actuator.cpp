#include "linear_actuator.hpp"

#include <cmath>

bool LinearActuator::equals(const RobotComponent& other) const
{
	const LinearActuator& linear_actuator = static_cast<const LinearActuator&>(other);
	
	return RobotComponent::equals(other)								&&
		   this->extension == linear_actuator.extension 				&&
		   this->maximumExtension == linear_actuator.maximumExtension 	&&
		   this->height == linear_actuator.height 						&&
		   this->width == linear_actuator.width;
}

LinearActuator::LinearActuator
(
	id_t id, 
	std::string name, 
	Coordinate position, 
	double angle,
	double mass,
	double maximumExtension, 
	double height, 
	double width
) : RobotComponent(id, name, position, angle, mass), maximumExtension(maximumExtension), height(height), width(width) {}

void LinearActuator::translate(double displacement)
{	
	double pastExtension = this->extension;
	double newExtension = this->extension + displacement;

	// Ensures that the extension will not become negative
	if (newExtension < 0)
	{
		this->extension = 0;
	}
	else if (newExtension > maximumExtension)
	{
		this->extension = maximumExtension;
	}
	else
	{
		this->extension = newExtension;
	}
	
	// Changing the characteristics of the linear actuator
	this->height += this->extension - pastExtension;
	
	double dx = (this->extension - pastExtension) * cos(this->angle);
	double dy = (this->extension - pastExtension) * sin(this->angle);
	
	RobotComponent* currentComponent = this->front;
	
	while (currentComponent != nullptr)
	{
		currentComponent->position.x += dx;
		currentComponent->position.y += dy;
		
		currentComponent = currentComponent->front;
	}
}

double LinearActuator::getExtension() const
{
	return this->extension;
}