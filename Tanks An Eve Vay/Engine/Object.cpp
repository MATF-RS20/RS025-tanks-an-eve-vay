#include "Object.h"

Object::Object()
{
}

Object::Object(Vector2f position, Vector2f size)
	:m_Position(position), m_Size(size)
{
}

Vector2f Object::GetPosition() const
{
	return m_Position;
}

Vector2f Object::GetSize() const
{
	return m_Size;
}


void Object::Move(const Vector2f & dv)
{
	m_Position = m_Position + dv;
}

std::string Object::ToString()
{
	return "Position of object: " + m_Position.ToString() + " Size: " + m_Size.ToString();
}

bool Object::DetectCoalision(const Object * other)
{
	return false;
}

Object::~Object()
{
}
