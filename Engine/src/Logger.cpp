#include <Engine\Logger.h>

Engine::Logger &Engine::Logger::Instance(void)
{
	static Logger instance;
	return instance;
}

Engine::Logger::Logger(void)
{
}

Engine::Logger::~Logger(void)
{
	if (_loggerFile.is_open()) _loggerFile.close();
}

void Engine::Logger::updateTimeBuffer(void)
{
	time_t rawTime;
	tm *timeInfo;

	time(&rawTime);
	timeInfo = localtime(&rawTime);

	strftime(_timeBuffer, 80, "(%R:%S)", timeInfo);
}

void Engine::Logger::config(const std::wstring &file)
{
	if (_loggerFile.is_open()) _loggerFile.close();
	_loggerFile.open(file, std::wofstream::out | std::wofstream::binary);
	_loggerFile.imbue(std::locale(_loggerFile.getloc(), new std::codecvt_utf16<WCHAR, 0x10ffff, std::generate_header>));
}

void Engine::Logger::info(const std::wstring &category, const std::wstring &text)
{
	if (!_loggerFile.is_open()) return;
	this->updateTimeBuffer();
	_loggerFile << _timeBuffer << L" INFO::" << category << L" -> " << text << std::endl;
}

void Engine::Logger::debug(const std::wstring &category, const std::wstring &text)
{
	if (!_loggerFile.is_open()) return;
	this->updateTimeBuffer();
	_loggerFile << _timeBuffer << L" DEBUG::" << category << L" -> " << text << std::endl;
}

void Engine::Logger::error(const std::wstring &category, const std::wstring &text)
{
	if (!_loggerFile.is_open()) return;
	this->updateTimeBuffer();
	_loggerFile << _timeBuffer << L" ERROR::" << category << L" -> " << text << std::endl;
}

void Engine::Logger::error(const std::wstring &category, const std::exception &exception)
{
	if (!_loggerFile.is_open()) return;
	this->updateTimeBuffer();
	_loggerFile << _timeBuffer << L" ERROR::" << category << L" -> " << exception.what() << std::endl;
}

void Engine::Logger::error(const std::wstring &category, const std::wstring &text, const std::exception &exception)
{
	this->error(category, text);
	this->error(category, exception);
}
