#include "DUOLGame/TestScripts/CoroutineLogTest.h"

#include "DUOLGameEngine/Util/Coroutine/CoroutineHandler.h"
#include "DUOLGameEngine/Util/Coroutine/WaitForSeconds.h"
#include "DUOLCommon/LogHelper.h"

using namespace DUOLGameEngine;

// �̰� ���� ��� �Լ��� �� �ֳ� ..? => this capture ���� ������� ..
CoroutineHandler LogLog()
{
	for (int i = 0; i < 1000; i++)
	{
		DUOL_CRITICAL("{0} seconds ago game loop start.", i * 2);

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
		for (int i = 0 ; i < 100 ; i++)
		{
			DUOL_ENGINE_CRITICAL("{0} seconds ago game loop start.", i * 2);

			co_yield std::make_shared<DUOLGameEngine::WaitForSeconds>(2.f);
		}
	}

	void CoroutineLogTest::OnAwake()
	{
		MonoBehaviourBase::OnAwake();

		// ����Լ� �ڷ�ƾ ��ŸƮ !
		StartCoroutine(*this, &CoroutineLogTest::LogPerTwoSeconds);
	}

	void CoroutineLogTest::OnUpdate(float deltaTime)
	{
		MonoBehaviourBase::OnUpdate(deltaTime);
	}
}