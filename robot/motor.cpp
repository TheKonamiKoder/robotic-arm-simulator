#define _USE_MATH_DEFINES

#include "motor.hpp"

#include <cmath>

bool Motor::equals(const RobotComponent& other) const
{
	const Motor& motor = static_cast<const Motor&>(other);

	return RobotComponent::equals(other) 					&&
		   this->rotatedAngle == motor.rotatedAngle 		&&
		   this->maximumRotation == motor.maximumRotation 	&&
		   this->radius == motor.radius;
}

Motor::Motor
(
	id_t id, 
	std::string name, 
	Coordinate position, 
	double angle,
	double mass,
	double maximumRotation, 
	double radius
) : RobotComponent(id, name, position, angle, mass), maximumRotation(maximumRotation), radius(radius) {}

void Motor::rotate(double angularDisplacement)
{
	double pastAngle = this->rotatedAngle;
	double newAngle = this->rotatedAngle + angularDisplacement;

	// If the maximum rotation is not 2 pi (which means that the angle cannot rotate fully) we first have to check that 
	// the maximum and minimum rotations are not surpassed
	if (!this->canRotateFully())					// Allow a small error of 0.1, due to floating point inaccuracies 
	{
		if (newAngle > this->maximumRotation)
		{
			newAngle = this->maximumRotation;
		}
		else if (newAngle < 0)
		{
			newAngle = 0;
		}		
	}
	else
	{
		newAngle = this->normalise(newAngle);
	}
	
	this->rotatedAngle = newAngle;

	RobotComponent* currentComponent = this->front;

	while (currentComponent != nullptr)
	{
		currentComponent->position.rotateAbout(this->position, this->rotatedAngle - pastAngle);
		currentComponent->rotateAngle(this->rotatedAngle - pastAngle);
		currentComponent = currentComponent->front;
	}
}

bool Motor::canRotateFully() const
{
	// Allow a small error of 0.1, due to floating point inaccuracies
	return (2 * M_PI - this->maximumRotation) < 0.1;
}

double Motor::getRotatedAngle() const
{
	return this->rotatedAngle;
}