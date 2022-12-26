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
	 * \brief Entry point of DUOLEditor. 근데 지금 생각난건데 Engine core code에서 제공해도 될 것 같기도 하다 ..
	 * 일종의 Base Class라도 분명 엔진이 바라는 컨트롤 방향이 있을 것이니까
	 */
	class Application
	{
	public:
		Application() :
			_gameEngine(nullptr)
			, _editorModeOption({})
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

	public:
		DUOLGameEngine::EditorModeOption _editorModeOption;

		std::shared_ptr<DUOLGameEngine::Engine> _gameEngine;

		std::shared_ptr<DUOLEditor::Editor> _editor;

	public:
		void Run() const;
	};
}