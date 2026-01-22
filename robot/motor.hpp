#pragma once

#define _USE_MATH_DEFINES

#include <string>
#include <cmath>
#include "coordinate.hpp"
#include "robot_component.hpp"

class Motor: public RobotComponent
{
	private:
		double rotatedAngle;
	protected:
		bool equals(const RobotComponent& other) const override;
	public:
		double maximumRotation;
		double radius;
	
		Motor();
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
		double getRotatedAngle();
};

bool Motor::equals(const RobotComponent& other) const
{
	const Motor& motor = static_cast<const Motor&>(other);

	return RobotComponent::equals(other) 					&&
		   this->rotatedAngle == motor.rotatedAngle 		&&
		   this->maximumRotation == motor.maximumRotation 	&&
		   this->radius == motor.radius;
}

Motor::Motor() {}

Motor::Motor
(
	id_t id, 
	std::string name, 
	Coordinate position, 
	double angle,
	double mass,
	double maximumRotation, 
	double radius
)
{
	// Initialising base class attributes
	this->id = id;
	this->name = name;
	this->position = position;
	this->angle = angle;
	this->mass = mass;
	this->front = nullptr;
	
	// Initialising child class attributes
	this->rotatedAngle = 0;    // Setting starting rotated angle to 0 at the beginning
	this->maximumRotation = maximumRotation;
	this->radius = radius;
}

void Motor::rotate(double angularDisplacement)
{
	double pastAngle = this->rotatedAngle;
	double newAngle = this->rotatedAngle + angularDisplacement;

	// If the maximum rotation is not 2 pi (which means that the angle cannot rotate fully) we first have to check that 
	// the maximum and minimum rotations are not surpassed
	if ((2 * M_PI - this->maximumRotation) > 0.1)	// Allow a small error of 0.1, due to floating point inaccuracies 
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
		// Ensure that the newAngle is between 0 and 2 pi radians
		// This is to be able to check if the motor has rotated past its maximum rotation
		// A while loop is necessary in case the magnitude of the angular displacement is greater than 2 pi
		while (!((newAngle >= 0) && (newAngle < 2 * M_PI)))
		{
			if (newAngle < 0)
			{
				newAngle += 2 * M_PI;	// This will add 2 pi to angle until it comes into range
			}
			if (newAngle >= 2 * M_PI)
			{
				newAngle -= 2 * M_PI;	// This willl remove 2 pi from angle until it comes into range
			}
		}
	}
	
	this->rotatedAngle = newAngle;

	RobotComponent* currentComponent = this->front;

	while (currentComponent != nullptr)
	{
		currentComponent->position.rotateAbout(this->position, this->rotatedAngle - pastAngle);
		currentComponent->angle += (this->rotatedAngle - pastAngle);
		currentComponent = currentComponent->front;
	}
}

double Motor::getRotatedAngle()
{
	return this->rotatedAngle;
}