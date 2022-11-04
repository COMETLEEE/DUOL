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
		// �� ����, �� ��Ȱ�� ���� ���ؼ��� Awake���� ó���ؾ��Ѵ�.
		std::shared_ptr<DUOLGameEngine::GameObject> testObject = CreateEmpty();

		testObject->AddComponent<CoroutineLogTest>();

		__super::Awake();
	}
}