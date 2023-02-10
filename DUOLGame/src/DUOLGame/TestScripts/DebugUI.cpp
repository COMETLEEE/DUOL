#include "DUOLGame/TestScripts/DebugUI.h"
#include "DUOLGameEngine/Manager/TimeManager.h"

DUOLGame::DebugUI::DebugUI(const std::weak_ptr<DUOLGameEngine::GameObject>& owner):
	MonoBehaviourBase(owner)
{
}

DUOLGame::DebugUI::~DebugUI()
{
}

void DUOLGame::DebugUI::OnStart()
{
}

void DUOLGame::DebugUI::OnUpdate(float deltaTime)
{
	std::wstring str = L"FPS : ";
	str += std::to_wstring(DUOLGameEngine::TimeManager::GetInstance()->GetFPS());
	_fpstext->GetTextBox()._text = str;
}
