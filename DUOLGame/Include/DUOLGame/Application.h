/**

    @file      Application.h
    @brief     DUOLGame.exe 에서 게임 엔진을 통해 WinMain Game Loop를 담당하는 클래스
    @details   ~
    @author    COMETLEE
    @date      18.10.2022
    @copyright © COMETLEE, 2022. All right reserved.

**/

#pragma once
#include <cassert>
#include <memory>

#include "DUOLGameEngine/Engine.h"
#include "DUOLCommon/StringHelper.h"

namespace DUOLGame
{
	/**
	 * \brief 커맨드 라인 변수
	 */
	struct ApplicationCommandLineArgs
	{
		int count = 0;

		char** args = nullptr;

		const char* operator[](int index) const
		{
			assert(index < count);

			return args[index];
		}
	};

	/**
	 * \brief 어플리케이션 스펙. 추후 .inl 파일과 같은 설정 파일을 파싱하여 객체 생성할 예정입니다.
	 */
	struct ApplicationSpecification
	{
		DUOLGameEngine::tstring name = DUOLCommon::StringHelper::ToTString("DUOL GAME");

		DUOLGameEngine::tstring workingDirectory;

		ApplicationCommandLineArgs commandLineArgs;
	};

	class Application
	{
	public:
		Application() :
			_gameEngine(nullptr)
		{
			Initialize();
		}

		~Application() { UnInitialize(); }

		Application(const Application&) = delete;

		Application operator=(const Application&) = delete;

		Application(Application&&) = delete;

		Application operator=(Application&&) = delete;

	private:
		void Initialize();

		void UnInitialize();

	private:
		std::unique_ptr<DUOLGameEngine::Engine> _gameEngine;
		
	public:
		void Run() const;
	};
}