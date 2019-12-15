#pragma once

#ifndef __VECTOR2F__H
#define __VECTOR2F__H

#define __DEBUG__

#ifdef __DEBUG__
#include <iostream>
#endif

class Vector2f
{
public:
	Vector2f(double x = 0.f, double y = 0.f);

	double GetX() const;
	double GetY() const;
	double Distance(const Vector2f& v1, const Vector2f& v2) const;
	Vector2f operator+(const Vector2f& v) const;
	Vector2f operator-(const Vector2f& v) const;
	Vector2f operator-() const;
	bool operator==(const Vector2f& v) const;
	bool operator!=(const Vector2f& v) const;



	~Vector2f();


private:
	double m_X;
	double m_Y;
#ifdef __DEBUG__
	friend std::ostream& operator<<(std::ostream& out, const Vector2f& v);
#endif
};

#endif