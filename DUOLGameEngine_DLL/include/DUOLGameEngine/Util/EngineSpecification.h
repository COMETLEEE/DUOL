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
		HWND hWnd;

		DUOLCommon::tstring _startSceneName;

		DUOLCommon::tstring _projectPath;

		DUOLCommon::tstring _projectName;

		uint32_t _screenWidth = 0;

		uint32_t _screenHeight = 0;

		bool _isFullScreen = false;
	};
}