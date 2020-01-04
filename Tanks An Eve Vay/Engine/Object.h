#pragma once

#include "Vector2f.h"

class Object
{
public:
	Object();
	Object(Vector2f position, Vector2f size);

	Object(const Object& o) = default;
	Object& operator =(const Object& o) = default;

	Vector2f GetPosition() const;
	Vector2f GetSize() const;

	virtual void Move(const Vector2f& dv);
	virtual std::string ToString();
	bool DetectCoalision(const Object* other);
	~Object();

private:
	Vector2f m_Position;
	Vector2f m_Size;
};


