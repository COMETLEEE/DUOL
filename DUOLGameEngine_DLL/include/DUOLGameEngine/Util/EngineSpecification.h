#pragma once
#include "DUOLGameEngine/Export_Engine.h"

#include "DUOLCommon/StringHelper.h"

namespace DUOLGameEngine
{
	/**
	 * \brief ���� ����. ������ �ʱ�ȭ�� ���� �ʿ��� �������� ��Ƽ� �����Ѵ�.
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