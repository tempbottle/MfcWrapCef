#include "stdafx.h"

#include "util/logging.h"
#include "util/string_converter.h"

#include <fstream>
#include <ctime>
#include <memory>
#include <mutex>

namespace
{
	// 年月日时分秒
	char* ymdhms_time(const struct tm *timeptr)
	{
		static char y_m_d_h_m_s[20] = {0};
		sprintf(y_m_d_h_m_s, "%.4d%.2d%.2d %.2d:%.2d:%.2d",
			1900 + timeptr->tm_year, 
			1 + timeptr->tm_mon,
			timeptr->tm_mday, timeptr->tm_hour,
			timeptr->tm_min, timeptr->tm_sec
			);
		return y_m_d_h_m_s;
	}
	// 年月日
	char* ymd_time(const struct tm *timeptr)
	{
		static char y_m_d[9] = { 0 };
		sprintf(y_m_d, "%.4d%.2d%.2d",
			1900 + timeptr->tm_year,
			1 + timeptr->tm_mon,
			timeptr->tm_mday);
		return y_m_d;
	}


	class Logger
	{
	public:
		Logger(const std::string& log_file_name) : log_file_stream(log_file_name, std::ios_base::out | std::ios_base::app)
		{
			log_file_stream << 
				"\n"
				"*************************************************************\n"
				"*************************************************************\n"
				<< std::endl;
		}
		bool Initialized()
		{
			return log_file_stream.is_open();
		}
		~Logger()
		{
			log_file_stream.close();
		}
		void Log(const std::wstring& message)
		{

			time_t rawtime;
			struct tm * timeinfo;

			time(&rawtime);// 获取当前系统时间
			timeinfo = localtime(&rawtime);

			log_file_stream << ymdhms_time(timeinfo) << "  " << wstr_2_str(message) << std::endl;
			log_file_stream.flush();
		}
	private:
		std::ofstream log_file_stream;
	};
	std::unique_ptr<Logger> logger;
}

namespace Logging
{
	std::mutex lg_mutex;

	bool Init(const std::string& log_file_name)
	{
		std::unique_lock<std::mutex> lk(lg_mutex);
		if (nullptr == logger)
		{ 
			time_t rawtime;
			struct tm * timeinfo;

			time(&rawtime);// 获取当前系统时间
			timeinfo = localtime(&rawtime);
			std::string str_time = "_" + std::string(ymd_time(timeinfo));
			logger.reset(new Logger(log_file_name + str_time + ".log"));
		}
		return logger->Initialized();
	}

	void Log(const std::wstring& message)
	{
		std::unique_lock<std::mutex> lk(lg_mutex);
		if (nullptr == logger)
			return;
		logger->Log(message);
	}
}