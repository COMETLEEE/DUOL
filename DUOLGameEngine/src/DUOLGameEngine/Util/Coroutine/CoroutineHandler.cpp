#include "DUOLGameEngine/Util/Coroutine/CoroutineHandler.h"

namespace DUOLGameEngine
{
	CoroutineHandler::CoroutineHandler(CoroutineHandle handle) :
		_coHandle(handle)
	{

	}

	void CoroutineHandler::UpdateCoroutine(float deltaTime) const
	{
		if (_coHandle)
		{
			const std::shared_ptr<YieldInstructionBase> yieldInstruction = _coHandle.promise().GetYieldInstruction();

			if (yieldInstruction != nullptr)
				yieldInstruction->UpdateInstruction(deltaTime);
		}
	}

	bool CoroutineHandler::IsDone() const
	{
		if (_coHandle)
			return _coHandle.done();

		return true;
	}

	void CoroutineHandler::Resume() const
	{
		if (_coHandle)
			_coHandle.resume();
	}

	bool CoroutineHandler::CanResume() const
	{
		if (_coHandle)
		{
			const std::shared_ptr<YieldInstructionBase>& yieldInstruction = _coHandle.promise().GetYieldInstruction();

			// ����� ������ �� ������ �� ���� ���������� �Ѿ�ϴ�.
			if (yieldInstruction != nullptr)
			{
				return _coHandle.promise().GetYieldInstruction()->CanResume();
			}
			// co_yield nullptr;
			else
			{
				if (_coHandle.promise()._isYieldedByNullThisFrame == true)
				{
					_coHandle.promise()._isYieldedByNullThisFrame = false;

					// �ش� �ڷ�ƾ�� ���� �����ӿ� Resume �մϴ�.
					return false;
				}
				else
				{
					return true;
				}
			}
		}

		return false;
	}

	void CoroutineHandler::UnInitialize() const
	{
		if (_coHandle)
			_coHandle.destroy();
	}

	bool CoroutineHandler::CompareCoroutineHandle(const CoroutineHandler& coroutineHandler) const
	{
		return _coHandle == coroutineHandler._coHandle ? true : false;
	}
}