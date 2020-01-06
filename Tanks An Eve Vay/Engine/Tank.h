#ifndef TANK_H
#define TANK_H

#include <string>
#include "Object.h"

#pragma once
class Tank : public Object
{
private:

public:
	Tank(Vector2f position, Vector2f size);

	void moveTank(Vector2f dv);
}; // CLASS TANK

#endif //END OF TANK_H

