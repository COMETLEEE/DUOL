#include "DUOLGame/TestScripts/CoroutineLogTest.h"

#include "DUOLGameEngine/Util/Coroutine/CoroutineHandler.h"
#include "DUOLGameEngine/Util/Coroutine/WaitForSeconds.h"
#include "DUOLCommon/LogHelper.h"

using namespace DUOLGameEngine;

namespace DUOLGame
{
	CoroutineLogTest::CoroutineLogTest(const std::weak_ptr<DUOLGameEngine::GameObject>& owner) :
		DUOLGameEngine::MonoBehaviourBase(owner, TEXT("CoroutineLogTest"))
	{

	}

	DUOLGameEngine::CoroutineHandler CoroutineLogTest::LogPerTwoSeconds()
	{
		for (int i = 0 ; i < 3 ; i++)
		{
			DUOL_TRACE("{0} seconds ago game loop start.", i * 2);

			co_yield std::make_shared<DUOLGameEngine::WaitForSeconds>(2.f);
		}
	}

	DUOLGameEngine::CoroutineHandler CoroutineLogTest::CoroutineInCoroutine()
	{
		for (int i = 0 ; i < 100 ; i++)
		{
			co_yield StartCoroutine(&CoroutineLogTest::LogPerTwoSeconds);

			DUOL_WARN("LogPerTwoSecond End ..! But, Re-Start LogPerTwoSeconds after 3 seconds.");

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
		StartCoroutine(&CoroutineLogTest::CoroutineInCoroutine);

		StartCoroutine(&CoroutineLogTest::InvokeInCoroutine);
	}

	void CoroutineLogTest::OnUpdate(float deltaTime)
	{

	}
}