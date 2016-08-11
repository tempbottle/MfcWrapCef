#ifndef UTIL_LOGGING_H_INCLUDED
#define UTIL_LOGGING_H_INCLUDED

#include <string>
#include <cstdint>

namespace Logging
{
	bool Init(const std::string& log_file_name = "app");
	void Log(const std::wstring& message);
} // Logging

// Here this macro will add "FILE" "FUNCTION" and "LINE" infomation.
#define LOCATION	L"{FILE:" + std::wstring(__FILEW__) + \
					L" LINE:" + std::to_wstring(__LINE__) + \
					L" FUNC:" + std::wstring(__FUNCTIONW__) + \
					L"} "

#if 1
#define LOG_TRACE(Messages)	   Logging::Log(std::wstring(L"Trace   ") + Messages)
#define LOG_DEBUG(Messages)	   Logging::Log(std::wstring(L"Debug   ") + Messages)
#define LOG_INFO(Messages)	   Logging::Log(std::wstring(L"Info    ") + Messages)
#define LOG_WARNING(Messages)  Logging::Log(std::wstring(L"Warning ") + Messages)
#define LOG_ERROR(Messages)	   Logging::Log(std::wstring(L"Error   ") + Messages)
#define LOG_FATAL(Messages)	   Logging::Log(std::wstring(L"Fatal   ") + Messages)
#else
#define LOG_TRACE(Messages)		do{}while(false)
#define LOG_DEBUG(Messages)		do{}while(false)
#define LOG_INFO(Messages)		do{}while(false)
#define LOG_WARNING(Messages)	do{}while(false)
#define LOG_ERROR(Messages)		do{}while(false)
#define LOG_FATAL(Messages)		do{}while(false)
#endif


#endif // UTIL_LOGGING_H_INCLUDED
