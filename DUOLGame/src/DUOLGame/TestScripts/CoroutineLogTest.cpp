#include "DUOLGame/TestScripts/CoroutineLogTest.h"

#include "DUOLGameEngine/Util/Coroutine/CoroutineHandler.h"
#include "DUOLGameEngine/Util/Coroutine/WaitForSeconds.h"
#include "DUOLCommon/LogHelper.h"

using namespace DUOLGameEngine;

// 이거 내부 멤버 함수로 못 넣나 ..? => this capture 등의 방법으로 ..
CoroutineHandler LogLog()
{
	for (int i = 0; i < 1000; i++)
	{
		DUOL_TRACE("{0} seconds ago game loop start.", i * 2);

		co_yield std::make_shared<DUOLGameEngine::WaitForSeconds>(2.f);
	}
}

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

	void CoroutineLogTest::OnAwake()
	{
		MonoBehaviourBase::OnAwake();

		// 멤버함수 코루틴 스타트를 위한 문법 ..! (C++ 문법이니까 .. 지켜주셔야 합니다.)
		StartCoroutine(&CoroutineLogTest::CoroutineInCoroutine);
	}

	void CoroutineLogTest::OnUpdate(float deltaTime)
	{
		MonoBehaviourBase::OnUpdate(deltaTime);
	}
}