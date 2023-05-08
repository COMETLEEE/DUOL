#include "DUOLGameEngine/Manager//ButtonEventManager.h"

#include "DUOLGameEngine/Manager/EventManager.h"
#include "DUOLGameEngine/Manager/SceneManagement/SceneManager.h"

DUOLGameEngine::ButtonEventManager::ButtonEventManager() :
	_exit(false)
{
}

DUOLGameEngine::ButtonEventManager::~ButtonEventManager()
{
}

void DUOLGameEngine::ButtonEventManager::Initialize()
{
}

void DUOLGameEngine::ButtonEventManager::UnInitialize()
{
}

void DUOLGameEngine::ButtonEventManager::Update(float deltaTime)
{
}

// Scene�� �ҷ����� �̺�Ʈ
void DUOLGameEngine::ButtonEventManager::LoadScene(DUOLCommon::tstring& scenename)
{
	std::string path = "Asset/Scene/" + DUOLCommon::StringHelper::ToString(scenename);
	path += ".dscene";

	auto scene = DUOLGameEngine::SceneManager::GetInstance()->LoadSceneFileFrom(DUOLCommon::StringHelper::ToTString(path));
}

void DUOLGameEngine::ButtonEventManager::EndGame()
{
	_exit = true;
}
