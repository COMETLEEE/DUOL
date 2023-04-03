#include "DUOLClient/ECS/Component/CoroutineLogTest.h"

#include "DUOLGameEngine/ECS/GameObject.h"

#include "DUOLGameEngine/Util/Coroutine/CoroutineHandler.h"
#include "DUOLGameEngine/Util/Coroutine/WaitForSeconds.h"
#include "DUOLCommon/Log/LogHelper.h"

#include <rttr/registration>
#include "DUOLCommon/MetaDataType.h"

using namespace rttr;

RTTR_REGISTRATION
{
	rttr::registration::class_<DUOLClient::CoroutineLogTest>("CoroutineLogTest")
	.constructor<>()
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
	CoroutineLogTest::CoroutineLogTest(DUOLGameEngine::GameObject* owner, const DUOLCommon::tstring& name) :
		DUOLGameEngine::MonoBehaviourBase(owner, name)
	{

	}

	DUOLGameEngine::CoroutineHandler CoroutineLogTest::LogPerTwoSeconds()
	{
		for (int i = 0 ; i < 3 ; i++)
		{
			DUOL_TRACE(DUOL_CONSOLE, "{0} seconds ago game loop start.", i * 2);

			co_yield std::make_shared<DUOLGameEngine::WaitForSeconds>(2.f);
		}
	}

	DUOLGameEngine::CoroutineHandler CoroutineLogTest::YieldNull()
	{
		co_yield nullptr;

		DUOL_TRACE(DUOL_CONSOLE, "으어아 다음 프레임이다 ..");

		co_yield std::make_shared<DUOLGameEngine::WaitForSeconds>(5.f);

		DUOL_TRACE(DUOL_CONSOLE, "으어아 프레임 지낫다.");
	}

	DUOLGameEngine::CoroutineHandler CoroutineLogTest::CoroutineInCoroutine()
	{
		for (int i = 0 ; i < 100 ; i++)
		{
			co_yield StartCoroutine(&CoroutineLogTest::LogPerTwoSeconds);

			DUOL_WARN(DUOL_CONSOLE, "LogPerTwoSecond End ..! But, Re-Start LogPerTwoSeconds after 3 seconds.");

			co_yield std::make_shared <DUOLGameEngine::WaitForSeconds>(3.f);
		}
	}

	DUOLGameEngine::CoroutineHandler CoroutineLogTest::InvokeInCoroutine()
	{
		for (int i = 0; i < 100; i++)
		{
			Invoke(&CoroutineLogTest::InvokeTestFunc, 5.f);

			co_yield std::make_shared<DUOLGameEngine::WaitForSeconds>(5.f);
		}
	}

	void CoroutineLogTest::InvokeTestFunc()
	{
		DUOL_CRITICAL("Invoke Test repeate 5 Second !");
	}

	void CoroutineLogTest::OnAwake()
	{
		// StartCoroutine(&CoroutineLogTest::CoroutineInCoroutine);

		// StartCoroutine(&CoroutineLogTest::InvokeInCoroutine);
	}

	void CoroutineLogTest::OnUpdate(float deltaTime)
	{
	}
}