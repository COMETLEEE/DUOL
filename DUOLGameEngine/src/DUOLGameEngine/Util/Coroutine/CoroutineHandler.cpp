#include "DUOLGameEngine/Util/Coroutine/CoroutineHandler.h"

namespace DUOLGameEngine
{
	CoroutineHandler::CoroutineHandler(CoroutineHandle handle) :
		_coHandle(handle)
	{

	}

	void CoroutineHandler::UpdateCoroutine(double deltaTime)
	{
		if (_coHandle)
			_coHandle.promise().GetYieldInstruction()->UpdateInstruction(deltaTime);
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
			return _coHandle.promise().GetYieldInstruction()->CanResume() ? true : false;

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