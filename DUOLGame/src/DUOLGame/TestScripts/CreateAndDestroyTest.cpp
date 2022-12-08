#include "DUOLGame/TestScripts/CreateAndDestroyTest.h"

#include "DUOLCommon/LogHelper.h"

#include "DUOLGameEngine/Manager/SceneManagement/SceneManager.h"

namespace DUOLGame
{
	CreateAndDestroyTest::CreateAndDestroyTest(const std::weak_ptr<DUOLGameEngine::GameObject>& owner) :
		DUOLGameEngine::MonoBehaviourBase(owner, TEXT("CREATE_DESTROY_TEST"))
		, _createTick(0.f)
		, _destroyTick(0.f)
	{
	}

	CreateAndDestroyTest::~CreateAndDestroyTest()
	{
	}

	void CreateAndDestroyTest::OnStart()
	{

	}

	void CreateAndDestroyTest::OnUpdate(float deltaTime)
	{
		_createTick += deltaTime;

		_destroyTick += deltaTime;

		if (_createTick >= 3.f)
		{
			DUOL_ERROR("Create New Object !");

			std::shared_ptr<DUOLGameEngine::GameObject> gameObject =
				DUOLGameEngine::SceneManager::GetInstance()->GetCurrentScene()->CreateEmpty();
		}

		// 4개 만들고 이 녀석을 삭제합시다.
		if (_destroyTick >= 12.5f)
		{
			DUOL_WARN("Destroy Seed Object !");

			// Destroy(std::static_pointer_cast<DUOLGameEngine::ObjectBase>(GetGameObject()));
		}
	}
}
