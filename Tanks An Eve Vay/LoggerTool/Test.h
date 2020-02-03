#pragma once
#include "Logger.h"
#include <string>
class TestObject
{
	friend std::ostream& operator<<(std::ostream& out, const TestObject& o)
	{
		return out << o.x << " " << o.y << " " << o.name;
	}
private:
	int x = 0;
	double y = 4.5554;
	std::string name = "perapera";
};

class Test
{
public:
	Test()
	{
		//WRITE YOUR TEST WITH LOGGER
		int x = 123445;
		LOGGERCREATE("Logs/a.txt");
		LOG(123, "123");
		LOG(111111111111111111, "broj");
		LOG("Pera je gey", "tag");
		LOG(x, "x");
		TestObject o;
		LOG(o, "objekat");
		Logger::ShutDownLogger();
	}
};