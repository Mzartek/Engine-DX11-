#pragma once

#include "Object.h"

namespace Engine
{
	class DLLAPI Logger : public Object
	{
	private:
		std::wofstream _loggerFile;
		char _timeBuffer[80];

	public:
		static Logger &Instance(void);

	private:
		Logger(void);
		~Logger(void);

		void updateTimeBuffer(void);

	public:
		void config(const std::wstring &file);

		void info(const std::wstring &category, const std::wstring &text);
		void debug(const std::wstring &category, const std::wstring &text);
		void error(const std::wstring &category, const std::wstring &text);
		void error(const std::wstring &category, const std::exception &exception);
		void error(const std::wstring &category, const std::wstring &text, const std::exception &exception);
	};
}

