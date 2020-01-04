#pragma once

#include "Object.h"

class Weapon : public Object
{
private:
	double m_angle;
	double m_velocity;
	double m_time = 0.0f;
	double m_gravity = 9.81;




public:
	Weapon(Vector2f position, Vector2f size, double playerAngle, double playerVelocity);

	void fly();
};

