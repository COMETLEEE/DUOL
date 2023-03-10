#include "DUOLClient/ECS/Component/DebugUI.h"

#include "DUOLGameEngine/Manager/GraphicsManager.h"
#include "DUOLGameEngine/Manager/TimeManager.h"
#include "DUOLGraphicsLibrary/RendererFlags.h"

DUOLClient::DebugUI::DebugUI(DUOLGameEngine::GameObject* owner) :
	MonoBehaviourBase(owner)
{
}

DUOLClient::DebugUI::~DebugUI()
{
}

void DUOLClient::DebugUI::OnStart()
{
}

void DUOLClient::DebugUI::OnUpdate(float deltaTime)
{
	std::wstring str = L"FPS : ";
	str += std::to_wstring(DUOLGameEngine::TimeManager::GetInstance()->GetFPS());
	_fpstext->GetTextBox()._text = str;

	DUOLGraphicsLibrary::QueryInfo queryInfo;

	if (DUOLGameEngine::GraphicsManager::GetInstance()->GetGraphicsRenderingResultInfo(queryInfo))
	{
		str = L"Drawed Surface : ";
		str += std::to_wstring(queryInfo._IAPrimitives);
		_etc->GetTextBox()._text = str;
	}

}
