#include "Tank.h"


Tank::Tank(Vector2f position, Vector2f size) : Object(position, size)
{
}

void Tank::moveTank(Vector2f dv)
{
	(*this).Move(dv);
}
