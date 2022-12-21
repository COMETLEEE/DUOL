#include "GameProcess.h"
#include "IntroScene.h"
#include "IGameEngine.h"
#include "LogSystem.h"
#include "SceneManager.h"
#include "ResourceManager.h"
std::shared_ptr<GameProcess> GameProcess::_gameProcess = nullptr;

GameProcess::~GameProcess()
{
	m_GameEngine->Finalize();
	m_GameEngine.reset();
	ResourceManager::Get().Finalize();
}

std::shared_ptr<GameProcess> GameProcess::Get()
{
	if (_gameProcess == nullptr)
		_gameProcess = std::make_shared<GameProcess>();

	return _gameProcess;
}

HWND GameProcess::GetHWND()
{
	return _hwnd;
}

void GameProcess::Initialize(HWND _Hwnd, int WindowSizeX, int WindowSizeY)
{

	_hwnd = _Hwnd;
	m_GameEngine = Muscle::IGameEngine::Get();
	m_GameEngine->Initialize(_Hwnd, WindowSizeX, WindowSizeY);


	// 씬 생성 및 게임 엔진에 등록
	std::shared_ptr<Muscle::IScene> introScene = std::make_shared<IntroScene>();

	m_GameEngine->GetSceneManager()->RegistScene(introScene);

	m_GameEngine->GetSceneManager()->LoadScene("IntroScene");

	ResourceManager::Get().Initialize();
}

void GameProcess::Finalize()
{
	_gameProcess.reset();
}

void GameProcess::Update()
{
	m_GameEngine->Update();
}

void GameProcess::Render()
{
	m_GameEngine->Render();
}

