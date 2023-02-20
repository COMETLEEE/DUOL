#include <fcntl.h>
#include <iostream>

#include "DUOLCommon/Log/LogHelper.h"

#include "DUOLCommon/Log/DUOLSink.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/sinks/basic_file_sink.h"
#include "spdlog/sinks/daily_file_sink.h"
#include "spdlog/sinks/rotating_file_sink.h"
#include "spdlog/logger.h"

namespace DUOLCommon
{
	std::shared_ptr<spdlog::logger> DUOLCommon::LogHelper::_engineLogger = nullptr;

	std::shared_ptr<spdlog::logger> DUOLCommon::LogHelper::_clientLogger = nullptr;

	std::shared_ptr<spdlog::logger> DUOLCommon::LogHelper::_fileLogger = nullptr;

	void LogHelper::Initialize()
	{
		RedirectIOToConsole();

		// Time Stamp / Name of the logger / 
		spdlog::set_pattern("%^[%T] %n: %v%$");

		//콘솔 로거 생성
		_engineLogger = spdlog::stdout_color_mt("DUOLEngine");

		// 가장 낮은 레벨로 모든것을 추적한다.
		_engineLogger->set_level(spdlog::level::trace);

		_clientLogger = spdlog::stdout_color_mt("DUOLClient");

		_clientLogger->set_level(spdlog::level::trace);

		CreateLogFile();

		DUOL_INFO(true, "Log Initialize Success !");

	}

	void LogHelper::UnInitialize()
	{
		spdlog::shutdown();
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

	void LogHelper::CreateLogFile()
	{
		try {
			//// Create a logger that uses the file sink
			// _fileLogger = std::make_shared<spdlog::logger>("FileLogger", file_sink);

			// 1024*1024(KB)

			auto customsink = std::make_shared<duollog::sinks::duol_file_sink_st>("logs/DUOLLog.txt", (1024 * 1024) * 10, 5);

			/*auto rotatesink = std::make_shared<spdlog::sinks::rotating_file_sink_st>("logs/DUOL.txt", 1024 * 10, 3);

			auto dailysink = std::make_shared<spdlog::sinks::daily_file_sink_st>("logs/DUOLs.txt", 2, 30, false, 4);*/

			_fileLogger = std::make_shared<spdlog::logger>("FileLogger", customsink);

			spdlog::register_logger(_fileLogger);

			_fileLogger->set_level(spdlog::level::trace);
		}
		catch (const spdlog::spdlog_ex& ex)
		{
			std::cout << ex.what() << std::endl;
		}
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

	std::shared_ptr<spdlog::logger> LogHelper::GetFileLogger()
	{
		return _fileLogger;
	}
}
