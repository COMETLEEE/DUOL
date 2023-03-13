#include "DUOLClient/ECS/Component/CreateAndDestroyTest.h"

#include "DUOLCommon/Log/LogHelper.h"

#include "DUOLGameEngine/Manager/SceneManagement/SceneManager.h"

#include "DUOLGameEngine/ECS/GameObject.h"

#include <rttr/registration>
#include "DUOLCommon/MetaDataType.h"

using namespace rttr;

RTTR_REGISTRATION
{
	rttr::registration::class_<DUOLClient::CreateAndDestroyTest>("CreateAndDestroyTest")
	.constructor()
	(
		rttr::policy::ctor::as_raw_ptr
	)
	.constructor<DUOLGameEngine::GameObject*, const DUOLCommon::tstring&>()
	(
		rttr::policy::ctor::as_raw_ptr
	);
}

namespace DUOLClient
{
	CreateAndDestroyTest::CreateAndDestroyTest(DUOLGameEngine::GameObject* owner, const DUOLCommon::tstring& name) :
		DUOLGameEngine::MonoBehaviourBase(owner, name)
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
			DUOLGameEngine::GameObject* gameObject =
				DUOLGameEngine::SceneManager::GetInstance()->GetCurrentScene()->CreateEmpty();
		}
	}
}
