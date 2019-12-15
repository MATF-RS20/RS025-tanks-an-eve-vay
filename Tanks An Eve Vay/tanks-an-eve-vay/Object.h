#pragma once

#include "Vector2f.h"

class Object
{
public:
	Object();
	~Object();

private:
	Vector2f m_Position;
	Vector2f m_Size;
};

