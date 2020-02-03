#include "Logger.h"
#include <iostream>

bool Logger::CreateLogger(std::string filename)
{
	m_File = std::ofstream(filename);
	if (!m_File.is_open())
	{
		std::cout << "ERROR: Logger was not initialized properly!" << std::endl;
		exit(1);
	}
	return true;
}
bool Logger::ShutDownLogger()
{
	if (m_File.is_open())
	{
		m_File.close();
	}
	return true;
}


std::ofstream Logger::m_File = std::ofstream("pera");