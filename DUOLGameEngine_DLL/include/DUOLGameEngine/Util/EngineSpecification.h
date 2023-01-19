#pragma once
#include "DUOLGameEngine/Export_Engine.h"

#include "DUOLCommon/StringHelper.h"

namespace DUOLGameEngine
{
	struct EditorModeOption
	{
		bool _isPlayingGame = false;
	};

	/**
	 * \brief 엔진 스펙. 엔진의 초기화를 위해 필요한 정보들을 담아서 전달한다.
	 */
	struct DUOL_GAMEENGINE_API EngineSpecification
	{
		DUOLCommon::tstring startSceneName;

		DUOLCommon::tstring projectPath;

		DUOLCommon::tstring projectName;

		HWND hWnd;

		uint32_t screenWidth = 0;

		uint32_t screenHeight = 0;

		/**
		 * \brief 레벨 에디터에서 게임 엔진을 참조할 때 사용하는 Option. (Context 공유한다고 생각하면 됨 ..)
		 */
		EditorModeOption* editorModeOption = nullptr;
	};
}