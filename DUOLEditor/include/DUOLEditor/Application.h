/**

    @file      Application.h
    @brief     Entry point of DUOLEditor
    @details   ~
    @author    COMETLEE
    @date      2.11.2022
    @copyright © COMETLEE, 2022. All right reserved.

**/
#pragma once
#include <memory>
#include "DUOLCommon/StringHelper.h"

#include "DUOLGameEngine/Engine.h"
#include "DUOLEditor/Editor.h"

namespace DUOLEditor
{
	/**
	 * \brief Entry point of DUOLEditor
	 */
	class Application
	{
	public:
		Application() :
			_gameEngine(nullptr)
		{
			Initialize();
		}

		~Application()
		{
			UnInitialize();
		}

	private:
		void Initialize();

		void UnInitialize();

	private:
		std::shared_ptr<DUOLGameEngine::Engine> _gameEngine;

		std::shared_ptr<DUOLEditor::Editor> _editor;

	public:
		void Run() const;
	};
}