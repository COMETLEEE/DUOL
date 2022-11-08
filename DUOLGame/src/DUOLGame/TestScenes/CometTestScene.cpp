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
		// �� ����, �� ��Ȱ�� ���� ���ؼ��� Awake���� ó���ؾ��Ѵ�.
		std::shared_ptr<DUOLGameEngine::GameObject> testObject = CreateEmpty();

		testObject->AddComponent<CoroutineLogTest>();

		/*std::shared_ptr<DUOLGameEngine::GameObject> testObject1 = CreateEmpty();

		std::shared_ptr<EnableTest> enableTest = testObject1->AddComponent<EnableTest>();*/

		// enableTest->_setComponent = testObject->GetComponent<CoroutineLogTest>();

		// �̰� ��� ������ ������ ����ؾ��մϴ� ..
		__super::Awake();
	}
}