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

#include <windows.h>
#include <cstdio>
#include <fcntl.h>
#include <io.h>
#include <iostream>
#include <fstream>

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
	 * \brief 어플리케이션 스펙. 추후 .inl 파일과 같은 설정 파일을 파싱하여 객체 생성 후 생성자에 넣어줄까 ..?
	 */
	struct ApplicationSpecification
	{
		DUOLCommon::tstring name = DUOLCommon::StringHelper::ToTString("DUOL GAME");

		DUOLCommon::tstring workingDirectory;

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

		~Application() {  }

		Application(const Application&) = delete;

		Application operator=(const Application&) = delete;

		Application(Application&&) = delete;

		Application operator=(Application&&) = delete;

	private:
		void Initialize();

	public:
		void UnInitialize();
		
		bool ProcWrapper(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	public:
		std::shared_ptr<DUOLGameEngine::Engine> _gameEngine;

	public:
		void Run() const;
	};
}