#include "Tank.h"


Tank::Tank(Vector2f position, Vector2f size) : m_PositionParameters(position, size)
{
}

Object Tank::getPositionParameters()
{
	return m_PositionParameters;
}
