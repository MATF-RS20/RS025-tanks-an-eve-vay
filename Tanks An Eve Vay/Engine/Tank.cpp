#include "Tank.h"


Tank::Tank(Vector2f position, Vector2f size) : m_PositionParameters(position, size)
{
}

Object Tank::getPositionParameters()
{
	return m_PositionParameters;
}

Vector2f Tank::GetPosition()
{
	return (*this).getPositionParameters().GetPosition();
}

void Tank::moveTank(Vector2f dv)
{
	m_PositionParameters.Move(dv);
}
