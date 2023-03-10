#include "DUOLClient/ECS/Component/EnableTest.h"


#include "DUOLGameEngine/Util/Coroutine/CoroutineHandler.h"
#include "DUOLGameEngine/Util/Coroutine/WaitForSeconds.h"

#include "DUOLCommon/Log/LogHelper.h"

namespace DUOLClient
{
	EnableTest::EnableTest(DUOLGameEngine::GameObject* owner) :
		DUOLGameEngine::MonoBehaviourBase(owner, TEXT("EnableTest"))
		, _setComponent(nullptr)
	{
	}

	EnableTest::~EnableTest()
	{
	}
	
	void EnableTest::OnUpdate(float deltaTime)
	{
		
	}

	DUOLGameEngine::CoroutineHandler EnableTest::ChangeEnablePer6Sec()
	{
		static bool value = true;

		for (int i = 0; i < 100; i++)
		{
			if (_setComponent != nullptr)
			{
				_setComponent->SetIsEnabled(value);

				value = !value;

				DUOL_ENGINE_INFO(DUOL_CONSOLE, "Time Stamp : Coroutine log comopnent enable changed.");
			}

			co_yield std::make_shared<DUOLGameEngine::WaitForSeconds>(7.f);
		}
	}

	void EnableTest::OnAwake()
	{
		// ����Լ� �ڷ�ƾ ��ŸƮ !
		StartCoroutine(&EnableTest::ChangeEnablePer6Sec);
	}
}
