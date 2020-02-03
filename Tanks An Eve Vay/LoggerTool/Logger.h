#pragma once
#include <string>
#include <fstream>
#define LOG(data,tag) Logger::Log(data,__FILE__,__LINE__,__FUNCTION__,tag); 
#define LOG2(data)	Logger::Log(data,__FILE__,__LINE__,__FUNCTION__,"unknown");
#define LOGGERCREATE(filename) Logger::CreateLogger(filename);
template<typename T> class Data
{
};

class  Logger
{
public:
	static bool CreateLogger(std::string filename);
	static bool ShutDownLogger();
	template <typename T> static void Log(T data, const std::string filename, unsigned line ,const std::string function, std::string tag);
private:
	static std::ofstream m_File;
};

template<typename T>
inline void Logger::Log(T data, const std::string filename, unsigned line, const std::string function, std::string tag)
{
	m_File << "[" << tag << "] : " << data << std::endl << "	"
		<< "[FILE]: " << filename << std::endl <<  "	"
		<< "[LINE]: " <<line << std::endl;
}

