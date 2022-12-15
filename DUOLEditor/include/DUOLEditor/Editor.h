/**

    @file      Editor.h
    @brief     DUOLEditor core class.
    @details   ~
    @author    COMETLEE
    @date      2.11.2022
    @copyright © COMETLEE, 2022. All right reserved.

**/
#pragma once
#include "DUOLGameEngine/Engine.h"

namespace DUOLEditor
{
	/**
	 * \brief Editor program core class.
	 */
	class Editor
	{
	public:
		Editor(const std::shared_ptr<DUOLGameEngine::Engine>& engine);

		~Editor();

	private:
		// 게임 엔진 객체를 참조하는 포인터
		std::shared_ptr<DUOLGameEngine::Engine> _gameEngine;
		
	public:
		void Update(float deltaTime);
	};
}
