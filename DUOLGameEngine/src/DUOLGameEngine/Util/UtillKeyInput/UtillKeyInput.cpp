#include "DUOLGameEngine/Manager/InputManager.h"
#include "DUOLGameEngine/Util/UtillKeyInput/UtillKeyInput.h"

#include "DUOLCommon/LogHelper.h"

namespace DUOLGameEngine
{
	UtillKeyInput::UtillKeyInput()
	{
	}

	UtillKeyInput::~UtillKeyInput()
	{
	}

	void UtillKeyInput::Initialize()
	{
		_isConsole = false;
	}

	void UtillKeyInput::UnInitialize()
	{
	}

	void UtillKeyInput::Update()
	{
		if (DUOLGameEngine::InputManager::GetInstance()->GetKeyDown(DUOLGameEngine::KeyCode::F1))
			_isConsole = !_isConsole;

		if(_isConsole)
			DUOLCommon::LogHelper::ShowConsole();
		else
			DUOLCommon::LogHelper::HideConsole();

	}
}