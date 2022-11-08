#include "DUOLGame/TestScenes/CometTestScene.h"

#include "DUOLGameEngine/ECS/GameObject.h"

#include "DUOLGame/TestScripts/EnableTest.h"
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

		/*std::shared_ptr<DUOLGameEngine::GameObject> testObject1 = CreateEmpty();

		std::shared_ptr<EnableTest> enableTest = testObject1->AddComponent<EnableTest>();*/

		// enableTest->_setComponent = testObject->GetComponent<CoroutineLogTest>();

		// 이거 어떻게 밖으로 빼낼지 고려해야합니다 ..
		__super::Awake();
	}
}