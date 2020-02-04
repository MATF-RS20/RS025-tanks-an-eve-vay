#pragma once
#include <ostream>
template<typename T> class LoggerStringBuilder
{
public:
	LoggerStringBuilder(T data) : m_Data(data) {};
	T m_Data;
	friend std::ostream& operator<<(std::ostream& out, const LoggerStringBuilder& o) { return out << o.m_Data; }
};