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

	private:
		static std::shared_ptr<spdlog::logger> _engineLogger;

		static std::shared_ptr<spdlog::logger> _clientLogger;

		static void RedirectIOToConsole();

	public:
		inline static std::shared_ptr<spdlog::logger>& GetEngineLogger() { return _engineLogger; }

		inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return _clientLogger; }
	};
}

// Core Log Macros
#define DUOL_ENGINE_TRACE(...)		::DUOLCommon::LogHelper::GetEngineLogger()->trace(__VA_ARGS__)
#define DUOL_ENGINE_INFO(...)		::DUOLCommon::LogHelper::GetEngineLogger()->info(__VA_ARGS__)
#define DUOL_ENGINE_WARN(...)		::DUOLCommon::LogHelper::GetEngineLogger()->warn(__VA_ARGS__)
#define DUOL_ENGINE_ERROR(...)		::DUOLCommon::LogHelper::GetEngineLogger()->error(__VA_ARGS__)
#define DUOL_ENGINE_CRITICAL(...)	::DUOLCommon::LogHelper::GetEngineLogger()->critical(__VA_ARGS__)

// Client Log Macros
#define DUOL_TRACE(...)				::DUOLCommon::LogHelper::GetClientLogger()->trace(__VA_ARGS__)
#define DUOL_INFO(...)				::DUOLCommon::LogHelper::GetClientLogger()->info(__VA_ARGS__)
#define DUOL_WARN(...)				::DUOLCommon::LogHelper::GetClientLogger()->warn(__VA_ARGS__)
#define DUOL_ERROR(...)				::DUOLCommon::LogHelper::GetClientLogger()->error(__VA_ARGS__)
#define DUOL_CRITICAL(...)			::DUOLCommon::LogHelper::GetClientLogger()->critical(__VA_ARGS__)