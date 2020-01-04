#ifndef TANK_H
#define TANK_H

#include <string>
#include "Object.h"

#pragma once
class Tank : public Object
{
private:
	Object m_PositionParameters;

public:
	Tank(Vector2f position, Vector2f size);

	Object getPositionParameters();
}; // CLASS TANK

#endif //END OF TANK_H

