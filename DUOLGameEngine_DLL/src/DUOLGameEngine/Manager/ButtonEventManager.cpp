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

// Scene을 불러오는 이벤트
void DUOLGameEngine::ButtonEventManager::LoadScene(DUOLCommon::tstring& scenename)
{
	std::string path = "Asset/Scene/" + DUOLCommon::StringHelper::ToString(scenename);
	path += ".dscene";

	//auto scene = SceneManager::GetInstance()->GetSceneStatus();
	//std::string sceneName = 
	

	auto scene = DUOLGameEngine::SceneManager::GetInstance()->LoadSceneFileFrom(DUOLCommon::StringHelper::ToTString(path));
}

void DUOLGameEngine::ButtonEventManager::LoadUnityScene(DUOLCommon::tstring& scenename)
{
	std::string path = "Asset/Scene_Unity/" + DUOLCommon::StringHelper::ToString(scenename);
	path += ".unity";

	auto scene = DUOLGameEngine::SceneManager::GetInstance()->LoadUnityScene(DUOLCommon::StringHelper::ToTString(path));
}

void DUOLGameEngine::ButtonEventManager::EndGame()
{
	_exit = true;
}
