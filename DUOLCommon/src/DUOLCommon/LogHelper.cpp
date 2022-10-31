#include <fcntl.h>
#include <iostream>

#include "DUOLCommon/LogHelper.h"
#include "spdlog/sinks/stdout_color_sinks.h"


namespace DUOLCommon
{
	std::shared_ptr<spdlog::logger> LogHelper::_engineLogger = nullptr;

	std::shared_ptr <spdlog::logger> LogHelper::_clientLogger = nullptr;

	void LogHelper::Initialize()
	{
		RedirectIOToConsole();

		// Time Stamp / Name of the logger / 
		spdlog::set_pattern("%^[%T] %n: %v%$");

		_engineLogger = spdlog::stdout_color_mt("DUOLEngine");

		_engineLogger->set_level(spdlog::level::trace);

		_clientLogger = spdlog::stdout_color_mt("DUOLClient");

		_clientLogger->set_level(spdlog::level::trace);
	}

	void LogHelper::UnInitialize()
	{
		::FreeConsole();
	}

	void LogHelper::RedirectIOToConsole()
	{
		CONSOLE_SCREEN_BUFFER_INFO coninfo;

		// allocate a console for this app
		AllocConsole();

		// set the screen buffer to be big enough to let us scroll text
		GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &coninfo);

		coninfo.dwSize.Y = MAX_CONSOLE_LINES;

		SetConsoleScreenBufferSize(GetStdHandle(STD_OUTPUT_HANDLE), coninfo.dwSize);

		// redirect unbuffered STDOUT to the console

		void* lStdHandle = GetStdHandle(STD_OUTPUT_HANDLE);
		int hConHandle = _open_osfhandle(reinterpret_cast<intptr_t>(lStdHandle), _O_TEXT);

		FILE* fp = _fdopen(hConHandle, "w");
		*stdout = *fp;
		setvbuf(stdout, NULL, _IONBF, 0);

		// redirect unbuffered STDIN to the console
		lStdHandle = GetStdHandle(STD_INPUT_HANDLE);
		hConHandle = _open_osfhandle(reinterpret_cast<intptr_t>(lStdHandle), _O_TEXT);

		fp = _fdopen(hConHandle, "r");

		*stdin = *fp;
		setvbuf(stdin, NULL, _IONBF, 0);

		// redirect unbuffered STDERR to the console
		lStdHandle = GetStdHandle(STD_ERROR_HANDLE);
		hConHandle = _open_osfhandle(reinterpret_cast<intptr_t>(lStdHandle), _O_TEXT);
		fp = _fdopen(hConHandle, "w");
		*stderr = *fp;
		setvbuf(stderr, NULL, _IONBF, 0);

		// make cout, wcout, cin, wcin, wcerr, cerr, wclog and clog 
		// point to console as well
		std::ios::sync_with_stdio();
	}
}