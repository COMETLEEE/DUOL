#include "DUOLGameEngine/ECS/Component/MonoBehaviourBase.h"

#include "DUOLGameEngine/ECS/GameObject.h"

namespace DUOLGameEngine
{
	MonoBehaviourBase::MonoBehaviourBase(const std::weak_ptr<DUOLGameEngine::GameObject>& owner, const DUOLCommon::tstring& name) :
		BehaviourBase(owner, name)
		, _coroutineHandlers(std::list<CoroutineHandler>())
	{

	}

	MonoBehaviourBase::~MonoBehaviourBase()
	{

	}

	void MonoBehaviourBase::SetIsEnabled(bool value)
	{
		// 게임 오브젝트에게 MonoBehaviour에 맞는 동작을 실시시킨다.
		// 이펙티브 C++에서 Protected 도 왠만하면 쓰지 말라는데 ..
		if (value == _isEnabled)
			return;

		const std::shared_ptr<GameObject>& gameObject = GetGameObject();

		value == true ? gameObject->SetMonoBehaviourEnabled(this->std::enable_shared_from_this<MonoBehaviourBase>::shared_from_this())
			: gameObject->SetMonoBehaviourDisabled(this->std::enable_shared_from_this<MonoBehaviourBase>::shared_from_this());

		// 프로퍼티 값을 바꿉니다.
		_isEnabled = value;
	}

	void MonoBehaviourBase::StopCoroutine(const std::shared_ptr<Coroutine>& coroutine)
	{
		for (auto iter = _coroutineHandlers.begin() ; iter != _coroutineHandlers.end() ; )
		{
			// 같은 코루틴 핸들이라면 .. 같습니다.
			// 해당 코루틴을 소멸시킵니다.
			if (iter->CompareCoroutineHandle(coroutine->GetCoroutineHandler()))
				_coroutineHandlers.erase(iter++);
			else
				iter++;
		}
	}

	void MonoBehaviourBase::StopAllCoroutines()
	{
		for (auto iter = _coroutineHandlers.begin() ; iter != _coroutineHandlers.end() ; iter++)
		{
			iter->UnInitialize();
		}

		_coroutineHandlers.clear();
	}

	void MonoBehaviourBase::UpdateAllCoroutines(float deltaTime)
	{
		for (auto iter = _coroutineHandlers.begin() ; iter != _coroutineHandlers.end() ; )
		{
			iter->UpdateCoroutine(deltaTime);

			if (iter->IsDone())
			{
				iter->UnInitialize();

				_coroutineHandlers.erase(iter++);

				continue;
			}

			if (iter->CanResume())
				iter->Resume();

			iter++;
		}
	}
}