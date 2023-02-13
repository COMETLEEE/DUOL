/**

	@file      LogHelper.h
	@brief     SPD C++ Log Library wrapper.
	@details   ~
	@author    COMETLEE
	@date      31.10.2022
	@copyright © COMETLEE, 2022. All right reserved.

**/
#pragma once
#include "spdlog/spdlog.h"
#include "spdlog/fmt/ostr.h"

namespace DUOLCommon
{
	constexpr WORD MAX_CONSOLE_LINES = 500;

	/**
	 * \brief 로그 남기는 것을 도와주는 헬퍼 클래스입니다.
	 */
	class LogHelper
	{
	public:
		static void Initialize();

		static void UnInitialize();

		static void HideConsole();

		static void ShowConsole();

		static void CreateLogFile();

	private:
		static std::shared_ptr<spdlog::logger> _engineLogger;

		static std::shared_ptr<spdlog::logger> _clientLogger;

		static std::shared_ptr<spdlog::logger> _fileLogger;

		static void RedirectIOToConsole();

	public:
		static std::shared_ptr<spdlog::logger> GetEngineLogger();

		static std::shared_ptr<spdlog::logger> GetClientLogger();

		static std::shared_ptr<spdlog::logger> GetFileLogger();

	};
}
	
#define DUOL_FILE				true
#define DUOL_CONSOLE			false

// Core Log Macros
#define DUOL_ENGINE_TRACE(x,...)	{::DUOLCommon::LogHelper::GetEngineLogger()->trace(__VA_ARGS__);if(x){SPDLOG_LOGGER_TRACE(::DUOLCommon::LogHelper::GetFileLogger(),__VA_ARGS__);};}
#define DUOL_ENGINE_INFO(x,...)		{::DUOLCommon::LogHelper::GetEngineLogger()->info(__VA_ARGS__);if(x){ SPDLOG_LOGGER_INFO(::DUOLCommon::LogHelper::GetFileLogger(),__VA_ARGS__);};}
#define DUOL_ENGINE_WARN(x,...)		{::DUOLCommon::LogHelper::GetEngineLogger()->warn(__VA_ARGS__);if(x){SPDLOG_LOGGER_INFO(::DUOLCommon::LogHelper::GetFileLogger(),__VA_ARGS__);};}
#define DUOL_ENGINE_ERROR(...)		{::DUOLCommon::LogHelper::GetEngineLogger()->error(__VA_ARGS__);SPDLOG_LOGGER_ERROR(::DUOLCommon::LogHelper::GetFileLogger(),__VA_ARGS__);}
#define DUOL_ENGINE_CRITICAL(...)	{::DUOLCommon::LogHelper::GetEngineLogger()->critical(__VA_ARGS__);SPDLOG_LOGGER_CRITICAL(::DUOLCommon::LogHelper::GetFileLogger(),__VA_ARGS__);}
#define DUOL_ENGINE_ASSERT(x,msg)	if ((x)) {} else {DUOL_ENGINE_CRITICAL("ASSERT - {}\n\t{}\n\tin file: {}\n\ton line: {}",#x,msg,__FILE__,__LINE__)};

// Client Log Macros
#define DUOL_TRACE(x,...)				{::DUOLCommon::LogHelper::GetClientLogger()->trace(__VA_ARGS__);if(x){SPDLOG_LOGGER_TRACE(::DUOLCommon::LogHelper::GetFileLogger(),__VA_ARGS__);};}
#define DUOL_INFO(x,...)				{::DUOLCommon::LogHelper::GetClientLogger()->info(__VA_ARGS__);if(x){ SPDLOG_LOGGER_INFO(::DUOLCommon::LogHelper::GetFileLogger(),__VA_ARGS__);};}
#define DUOL_WARN(x,...)				{::DUOLCommon::LogHelper::GetClientLogger()->warn(__VA_ARGS__);if(x){SPDLOG_LOGGER_INFO(::DUOLCommon::LogHelper::GetFileLogger(),__VA_ARGS__);};}
#define DUOL_ERROR(...)					{::DUOLCommon::LogHelper::GetClientLogger()->error(__VA_ARGS__);SPDLOG_LOGGER_ERROR(::DUOLCommon::LogHelper::GetFileLogger(),__VA_ARGS__);}
#define DUOL_CRITICAL(...)				{::DUOLCommon::LogHelper::GetClientLogger()->critical(__VA_ARGS__);SPDLOG_LOGGER_CRITICAL(::DUOLCommon::LogHelper::GetFileLogger(),__VA_ARGS__);}
#define DUOL_ASSERT(x,msg)				if ((x)) {} else {DUOL_CRITICAL("ASSERT - {}\n\t{}\n\tin file: {}\n\ton line: {}",#x,msg,__FILE__,__LINE__);

// File Log Macros
#define  DUOL_FILE_TRACE(...)				SPDLOG_LOGGER_TRACE(::DUOLCommon::LogHelper::GetFileLogger(),__VA_ARGS__)
#define  DUOL_FILE_WARN(...)				SPDLOG_LOGGER_WARN(::DUOLCommon::LogHelper::GetFileLogger(),__VA_ARGS__)
#define  DUOL_FILE_INFO(...)				SPDLOG_LOGGER_INFO(::DUOLCommon::LogHelper::GetFileLogger(),__VA_ARGS__)
#define  DUOL_FILE_ERROR(...)				SPDLOG_LOGGER_ERROR(::DUOLCommon::LogHelper::GetFileLogger(),__VA_ARGS__)
#define  DUOL_FILE_CRITICAL(...)			SPDLOG_LOGGER_CRITICAL(::DUOLCommon::LogHelper::GetFileLogger(),__VA_ARGS__)