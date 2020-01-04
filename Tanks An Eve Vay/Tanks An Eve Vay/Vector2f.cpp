#include "Vector2f.h"

#define __DEBUG__
Vector2f::Vector2f(double x, double y) :m_X(x), m_Y(y)
{
}

double Vector2f::GetX() const
{
	return m_X;
}

double Vector2f::GetY() const
{
	return m_Y;
}

double Vector2f::Distance(const Vector2f & v1, const Vector2f & v2) const
{
	return 0.0;
}

Vector2f Vector2f::operator+(const Vector2f & v) const
{
	return Vector2f(m_X + v.m_X, m_Y + v.m_Y);
}

Vector2f Vector2f::operator-(const Vector2f& v) const
{
	return Vector2f(m_X - v.m_X, m_Y - v.m_Y);
}

Vector2f Vector2f::operator-() const
{
	return Vector2f(-m_X, -m_Y);
}
bool Vector2f::operator==(const Vector2f& v) const
{
	return m_X == v.m_X && m_Y == v.m_Y;
}

bool Vector2f::operator!=(const Vector2f& v) const
{
	return !(*this == v);
}

std::string Vector2f::ToString()
{
	return "(" + std::to_string(m_X) + ", " + std::to_string(m_Y) + ")";
}

Vector2f::~Vector2f()
{
}

#ifdef __DEBUG__
std::ostream& operator<<(std::ostream& out, const Vector2f& v)
{
	return out << "Vector2f (" << v.m_X << ", " << v.m_Y << ");" << std::endl;
}
#endif
