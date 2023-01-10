#pragma once
#include "DUOLCommon/StringHelper.h"

namespace DUOLGameEngine
{
	struct EditorModeOption
	{
		bool _isPlayingGame = false;
	};

	/**
	 * \brief ���� ����. ������ �ʱ�ȭ�� ���� �ʿ��� �������� ��Ƽ� �����Ѵ�.
	 */
	struct EngineSpecification
	{
		DUOLCommon::tstring startSceneName;

		DUOLCommon::tstring projectPath;

		DUOLCommon::tstring projectName;

		HWND hWnd;

		uint32_t screenWidth = 0;

		uint32_t screenHeight = 0;

		/**
		 * \brief ���� �����Ϳ��� ���� ������ ������ �� ����ϴ� Option. (Context �����Ѵٰ� �����ϸ� �� ..)
		 */
		EditorModeOption* editorModeOption = nullptr;
	};
}