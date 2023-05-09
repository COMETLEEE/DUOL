#pragma once
#include "DUOLGameEngine/Export_Engine.h"
#include "DUOLCommon/StringHelper.h"

namespace DUOLGameEngine
{
	/**
	 * \brief 엔진 스펙. 엔진의 초기화를 위해 필요한 정보들을 담아서 전달한다.
	 */
	struct DUOL_GAMEENGINE_API EngineSpecification
	{
		HWND hWnd;

		DUOLCommon::tstring _startSceneName;

		DUOLCommon::tstring _projectPath;

		DUOLCommon::tstring _projectName;

		uint32_t _screenWidth = 0;

		uint32_t _screenHeight = 0;

		bool _isFullScreen = false;
	};
}