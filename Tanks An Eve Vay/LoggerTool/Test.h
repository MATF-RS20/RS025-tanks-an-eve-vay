#pragma once
#include "Logger.h"
#include <string>
class TestObject
{
public:
	FRIENDLOG(x, y, name)
private:

	int x = 0;
	double y = 4.5554;
	std::string name = "perapera";
};
class TestObject2
{
public:
	TestObject* o = new TestObject();
	FRIENDLOG(o)
	POINTERSLOG(o)
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
		TestObject2 o;
		o.LogPointers();
		Logger::ShutDownLogger();
	}
};