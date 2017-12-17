/******************************     LICENSE     *********************************************************
Copyright (c) 2017 Stanislav Uschakow <stanislav.uschakow@gmail.com>

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and
associated documentation files (the "Software"), to deal in the Software without restriction,
including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense,
and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so,
subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial
portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT
NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

*******************************************************************************************************
*/

/*
This class provides basic logging functionality via the log method. You cannot instanciate a
object of the logger yourself since the logger is a singleton. 
If logging to file you can specify a file name to be created if you want, otherwise the logger
will write its logs to application.log The threshold log level is the one you want to print to cout.
Only messages tagged with this particular log will be written to stdout.


Use the TRACE, INFO, DEBUG, WARNING, CRITICAL macros as you wish and expand them if needed.
*/

#ifndef LOGGER__H__
#define LOGGER__H__

#ifdef _WINDOWS
#include <Windows.h>
#endif


#include <iostream>
#include <fstream>
#include <string>
#include <ctime>



namespace attic
{
	class Logger {
	public:
		enum LOG_LEVEL {
			Trace = 1,
			Info = 2,
			Debug = 4,
			Warning = 8,
			Critical = 16
		};

		~Logger() { if (m_toFile) { m_strm.flush(); m_strm.close(); } };

		void setOutputLogFilter(unsigned int lvl) { m_logFilter |= lvl; }

		void setFileLogFilter(unsigned int lvl) { m_fileFilter |= lvl; }

		void resetOutputFilter() { m_logFilter = 0; }

		void resetFileLogFilter() { m_fileFilter = 0; }

		bool toFile() { return m_toFile; };

		bool toFile(std::string fileName)
		{
			if (!m_toFile)
			{
				m_logFileName = fileName;
				m_strm.open(m_logFileName, std::ios::out | std::ios::app);
				m_toFile = m_strm.is_open();
			}
			return m_toFile;
		};

		unsigned int getOutputLogFilter() { return m_logFilter; }

		unsigned int getFileLogFilter() { return m_fileFilter; }

		static Logger& getInstance()
		{
			static Logger log;
			return log;
			//return (l == nullptr) ? l = new Logger() : l;
		}

		void log(LOG_LEVEL lvl, std::string msg)
		{
			m_cnt++;
			//std::string m = "[" + getTime() + "]";
			std::string m;
			switch (lvl)
			{
				case Trace:
					setConsoleColor(GRAY);
					m += "[Trace]";
					break;
				case Info:
					setConsoleColor(NORMAL);
					m = "[Info]";
					break;
				case Debug:
					setConsoleColor(GRAY);
					m += "[Debug]";
					break;
				case Warning:
					setConsoleColor(REDDIM);
					m += "[WARNING]";
					break;
				case Critical:
					setConsoleColor(REDBRIGHT);
					m += "[CRITICAL]";
					break;
			}

			//m = m + msg;
			m += " ";
			if (lvl & m_logFilter)
			{
				std::cout << m;
				resetConsoleColor();
				std::cout << msg << std::endl;

			}
			if (m_toFile && (lvl & m_fileFilter))
			{
				m_strm << m << std::endl;
				if (m_cnt > FLUSHCNT)
				{
					m_strm.flush();
					m_cnt = 0;
				}
			}
		}

	private:
		Logger()
		{
			if (m_toFile)
			{
				m_strm.open(m_logFileName, std::ios::out | std::ios::app);
				m_toFile = m_strm.is_open();
			}
		};

#ifdef _WINDOWS
		const enum CONSOLE_COLOR {
			REDBRIGHT = 0xC,
			REDDIM = 0x4,
			YELLOW = 0xE,
			ORANGE = 0x6,
			GREENBRIGHT = 0xA,
			GREENDIM = 0x2,
			BLUE = 0x3,
			VIOLET = 0x5,
			WHITE = 0xF,
			GRAY = 0x8,
			NORMAL = 0x7
		};

#endif

		Logger(Logger& l);

		Logger& operator= (Logger& rhs);

		std::string getTime()
		{
			time_t t = time(0);
			struct tm* n = localtime(&t);

			std::string dateTime = std::to_string(n->tm_year + 1900)
				+ "_" + ((n->tm_mon < 10) ? "0" + std::to_string(n->tm_mon) : std::to_string(n->tm_mon))
				+ "_" + ((n->tm_mday < 10) ? "0" + std::to_string(n->tm_mday) : std::to_string(n->tm_mday))
				+ "_" + ((n->tm_hour < 10) ? "0" + std::to_string(n->tm_hour) : std::to_string(n->tm_hour))
				+ ":" + (n->tm_min < 10 ? "0" + std::to_string(n->tm_min) : std::to_string(n->tm_min))
				+ ":" + (n->tm_sec < 10 ? "0" + std::to_string(n->tm_sec) : std::to_string(n->tm_sec));
			return dateTime;
		}

		void setConsoleColor(CONSOLE_COLOR color)
		{
#ifdef _WINDOWS
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
#endif
		};
		void resetConsoleColor()
		{
#ifdef _WINDOWS
			std::cout.flush();
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), NORMAL);
#endif
		}

		//static Logger* l;
		bool m_toFile;
		std::string m_logFileName;
		LOG_LEVEL m_lvl = Logger::Info;

		const unsigned int FLUSHCNT = 0;
		unsigned int m_cnt = 0;
		unsigned int m_logFilter = 2;
		unsigned int m_fileFilter = 2;
		std::fstream m_strm;
	};

	//#define INITIALIZELOGGER(toFile, lvl, fileName) attic::Logger* attic::Logger::l = nullptr; bool attic::Logger::m_toFile = toFile; std::string attic::Logger::m_logFileName = std::string(fileName).empty() ? "application.log" : fileName; attic::Logger::LOG_LEVEL attic::Logger::m_lvl = lvl;
#define TRACE(x) attic::Logger::getInstance().log(attic::Logger::LOG_LEVEL::Trace, "[" + std::string(__FUNCTION__) + "@" + std::to_string(__LINE__) + "] " + x);
#define INFO(x) attic::Logger::getInstance().log(attic::Logger::LOG_LEVEL::Info, x);
#define DEBUG(x) attic::Logger::getInstance().log(attic::Logger::LOG_LEVEL::Debug, x);
#define WARNING(x) attic::Logger::getInstance().log(attic::Logger::LOG_LEVEL::Warning, x);
#define CRITICAL(x) attic::Logger::getInstance().log(attic::Logger::LOG_LEVEL::Critical, x);	
#define TO_STR(x) std::to_string(x)

}

#endif