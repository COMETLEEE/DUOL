#include "DUOLGameEngine/Util/Coroutine/Coroutine.h"

namespace DUOLGameEngine
{
	Coroutine::Coroutine(const CoroutineHandler& coroutineHandler) :
		YieldInstructionBase(YieldInstructionType::COROUTINE)
		, _coroutineHandler(coroutineHandler)
	{

	}

	Coroutine::~Coroutine()
	{

	}

	void Coroutine::UpdateInstruction(float deltaTime)
	{
		// ���� ���⼭�� �� ���� ���� �׳� �ش� �ڷ�ƾ ��ü�� ���� �ִ� �ڷ�ƾ �ڵ鷯��
		// �� �������� Ȯ���ϰ� ���� �帧���� �Ѿ�� �˴ϴ�.
	}

	bool Coroutine::CanResume()
	{
		return _coroutineHandler.IsDone();
	}
}