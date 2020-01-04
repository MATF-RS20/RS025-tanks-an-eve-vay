#include "Weapon.h"

Weapon::Weapon(Vector2f position, Vector2f size, double playerAngle, double playerVelocity) :
	Object(position, size) ,m_angle(playerAngle), m_velocity(playerVelocity) 
{}

void Weapon::fly()
{
	m_time += 0.01;
	float y = (*this).GetPosition().GetY() + 0.01;
	(*this).Move(Vector2f(0.0f, y));
}


