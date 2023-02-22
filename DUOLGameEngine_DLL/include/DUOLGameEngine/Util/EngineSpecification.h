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
		DUOLCommon::tstring startSceneName;

		DUOLCommon::tstring projectPath;

		DUOLCommon::tstring projectName;

		HWND hWnd;

		uint32_t screenWidth = 0;

		uint32_t screenHeight = 0;
	};
}