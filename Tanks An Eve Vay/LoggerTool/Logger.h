#pragma once
#include <string>
#include <fstream>
#include "LoggerStringBuilder.h"
#define LOG(data,tag) Logger::Log(data,__FILE__,__LINE__,__FUNCTION__,tag); 
#define LOG2(data)	Logger::Log(data,__FILE__,__LINE__,__FUNCTION__,"unknown");
#define LOGGERCREATE(filename) Logger::CreateLogger(filename);
#define DEFAULT (__FILE__, __LINE__, __FUNCTION__, "")

#define FRIENDLOG(...) void Log() { Logger::Log(__VA_ARGS__, __FILE__, __LINE__, __FUNCTION__, ""); }
#define STARTLOGGINGPOINTERS(...) void LogPointers() {LOGPOINTER(__VA_ARGS__);
#define LOGPOINTER(p,...) p.Log(); LOGPOINTER(__VA__ARGS__);
#define LOGPOINTER(p) p->Log();
#define ENDLOGGINGPOINTERS }
#define POINTERSLOG(...) STARTLOGGINGPOINTERS(__VA_ARGS__) ENDLOGGINGPOINTERS

class  Logger
{
public:
	static bool CreateLogger(std::string filename);
	static bool ShutDownLogger();
	template<typename T, typename A, typename B, typename C, typename D  >static void Log(T data, A filename, B line, C function, D tag);
	template <typename T,typename... Args> static void Log(T data, Args... args);
	template <typename T> void LogPointer(T pointer);
private:
	static std::ofstream m_File;
};

template<typename T,typename A,typename B, typename C, typename D  >
inline void Logger::Log(T data, A filename, B line, C function, D tag)
{
	LoggerStringBuilder<T> printableData(data);
	m_File << "[" << tag << "] : " << printableData << std::endl << "	"
		<< "[FILE]: " << filename << std::endl <<  "	"
		<< "[LINE]: " <<line << std::endl;
}

template<typename T, typename ...Args>
inline void Logger::Log(T data, Args ...args)
{
	LoggerStringBuilder<T> printableData(data);
	m_File << printableData << std::endl;
	return Log(args...);
}

template<typename T>
inline void Logger::LogPointer(T pointer)
{
	pointer.Log();
}
