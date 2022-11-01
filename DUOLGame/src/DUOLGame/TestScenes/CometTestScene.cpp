#include "DUOLGame/TestScenes/CometTestScene.h"

#include "DUOLGameEngine/ECS/GameObject.h"

#include "DUOLGame/TestScripts/CoroutineLogTest.h"

namespace DUOLGame
{
	CometTestScene::CometTestScene() :
		Scene(TEXT("CometTestScene"))
	{
		
	}

	CometTestScene::~CometTestScene()
	{

	}

	void CometTestScene::Awake()
	{
		// 씬 구성, 씬 재활용 등을 위해서라도 Awake에서 처리해야한다.
		std::shared_ptr<DUOLGameEngine::GameObject> testObject = CreateEmpty();

		testObject->AddComponent<CoroutineLogTest>();

		__super::Awake();
	}
}