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

	void LogHelper::HideConsole()
	{
		HWND hWndConsole = GetConsoleWindow();
		ShowWindow(hWndConsole, SW_HIDE);
	}

	void LogHelper::ShowConsole()
	{
		HWND hWndConsole = GetConsoleWindow();
		ShowWindow(hWndConsole, SW_SHOW);
	}


	void LogHelper::RedirectIOToConsole()
	{
		//CONSOLE_SCREEN_BUFFER_INFO coninfo;

		// 프로세스에 이미 콘솔이 연결되어 있는지 확인한다. 

		if (AllocConsole() == 0) 
		{
			// 현재 프로세스의 부모 콘솔을 사용한다.
			AttachConsole(ATTACH_PARENT_PROCESS);
			
		}

		//// set the screen buffer to be big enough to let us scroll text
		//GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &coninfo);

		//coninfo.dwSize.Y = MAX_CONSOLE_LINES;

		//SetConsoleScreenBufferSize(GetStdHandle(STD_OUTPUT_HANDLE), coninfo.dwSize);

		//// redirect unbuffered STDOUT to the console

		//void* lStdHandle = GetStdHandle(STD_OUTPUT_HANDLE);
		//int hConHandle = _open_osfhandle(reinterpret_cast<intptr_t>(lStdHandle), _O_TEXT);

		//FILE* fp = _fdopen(hConHandle, "w");
		//*stdout = *fp;
		//setvbuf(stdout, NULL, _IONBF, 0);

		//// redirect unbuffered STDIN to the console
		//lStdHandle = GetStdHandle(STD_INPUT_HANDLE);
		//hConHandle = _open_osfhandle(reinterpret_cast<intptr_t>(lStdHandle), _O_TEXT);

		//fp = _fdopen(hConHandle, "r");

		//*stdin = *fp;
		//setvbuf(stdin, NULL, _IONBF, 0);

		//// redirect unbuffered STDERR to the console
		//lStdHandle = GetStdHandle(STD_ERROR_HANDLE);
		//hConHandle = _open_osfhandle(reinterpret_cast<intptr_t>(lStdHandle), _O_TEXT);
		//fp = _fdopen(hConHandle, "w");
		//*stderr = *fp;
		//setvbuf(stderr, NULL, _IONBF, 0);

		//// make cout, wcout, cin, wcin, wcerr, cerr, wclog and clog 
		//// point to console as well
		//std::ios::sync_with_stdio(false);

		//std::cin.tie(NULL);

		//std::cout.tie(NULL);
	}
}