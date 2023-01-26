#include <fcntl.h>
#include <iostream>

#include "DUOLCommon/LogHelper.h"
#include "spdlog/sinks/stdout_color_sinks.h"

namespace DUOLCommon
{
	std::shared_ptr<spdlog::logger> DUOLCommon::LogHelper::_engineLogger = nullptr;

	std::shared_ptr<spdlog::logger> DUOLCommon::LogHelper::_clientLogger = nullptr;

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
	}

	std::shared_ptr<spdlog::logger> LogHelper::GetEngineLogger()
	{
		return _engineLogger;
	}

	std::shared_ptr<spdlog::logger> LogHelper::GetClientLogger()
	{
		return _clientLogger;
	}
}
