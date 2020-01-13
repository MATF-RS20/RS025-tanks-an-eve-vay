#include "Tank.h"


Tank::Tank(Vector2f position, Vector2f size) : Object(position, size)
{
}

void Tank::moveTank(Vector2f dv)
{

	if (this->GetPosition().GetX() + dv.GetX() - 0.05 <= -1.0f)
	{
		this->Move(Vector2f(0.01, 0));
	}
	else if (this->GetPosition().GetX() + dv.GetX() + 0.05 >= +1.0f)
	{
		this->Move(Vector2f(-0.01, 0));
	}
	else
	{
		(*this).Move(dv);
	}
	
}

double Tank::getTankDrawAngle() const
{
	return m_DrawAngle;
}

void Tank::setTankDrawAngle(const double newAngle)
{
	m_DrawAngle = newAngle;
}


