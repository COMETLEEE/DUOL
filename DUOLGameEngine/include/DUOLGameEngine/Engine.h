#pragma once
#include <memory>
#include "DUOLGameEngine/Util/Defines.h"

namespace DUOLGameEngine
{
	class InputManager;

	class TimeManager;

	class GraphicsManager;

	/**
	 * \brief 엔진 스펙. 엔진의 초기화를 위해 필요한 정보들을 담아서 전달한다.
	 */
	struct EngineSpecification
	{
		DUOLCommon::tstring startSceneName;

		HWND hWnd;

		uint32_t screenWidth;

		uint32_t screenHeight;
	};

	/**
	 * \brief 게임 엔진. .exe로 노출되어 초기 설정 파일을 통해 초기화를 하고 Game Loop를 실시한다.
	 */
	class Engine
	{
	public:
		Engine(const EngineSpecification& gameSpecification);

		~Engine();

		DELETE_COPY_MOVE(Engine)

	public:
		void Initialize();

		void UnInitialize();

	private:
		EngineSpecification _engineSpec;

	public:
		/**
		 * \brief 게임 엔진 생애 주기에 따른 루프를 실시합니다.
		 */
		void Update();
	};
}