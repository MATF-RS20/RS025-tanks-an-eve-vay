#ifndef TANK_H
#define TANK_H

#include <string>
#include "Object.h"

#pragma once
class Tank : public Object
{
private:
	double m_DrawAngle = 0;
public:
	Tank(Vector2f position, Vector2f size);

	void moveTank(Vector2f dv);
	double getTankDrawAngle() const;
	void setTankDrawAngle(const double newAngle);
}; // CLASS TANK

#endif //END OF TANK_H

