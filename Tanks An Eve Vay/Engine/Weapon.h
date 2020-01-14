#pragma once

#include "Object.h"
#include <math.h>
#define GRAVITY 25.0f
#define PI (atan(1.0f)*4.0f)

class Weapon : public Object
{
private:
	double m_angle;
	double m_velocity;
	double m_time = 0.0f;
	double m_radians;


public:
	Weapon(Vector2f position, Vector2f size, double playerAngle, double playerVelocity);

	Vector2f fly();
	
};

