#include "Weapon.h"

Weapon::Weapon(Vector2f position, Vector2f size, double playerAngle, double playerVelocity) :
	Object(position, size) ,m_angle(playerAngle), m_velocity(playerVelocity), m_radians(playerAngle)
{
	(*this).Move(Vector2f(0.01f, 0.05f));	
}

Vector2f Weapon::fly()
{
	m_time += 0.003;
	
	double x = 1.6*m_velocity * cos(m_radians) * m_time;
	double y = (1.6*m_velocity * sin(m_radians) * m_time) - GRAVITY*m_time*m_time*0.5;
	Vector2f v = (*this).GetPosition();
	(*this).Move(Vector2f(x, y));

	return (*this).GetPosition();
}


