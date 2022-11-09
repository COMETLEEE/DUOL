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
		// ���� ������Ʈ���� MonoBehaviour�� �´� ������ �ǽý�Ų��.
		// ����Ƽ�� C++���� Protected �� �ظ��ϸ� ���� ����µ� ..
		if (value == _isEnabled)
			return;

		const std::shared_ptr<GameObject>& gameObject = GetGameObject();

		value == true ? gameObject->SetMonoBehaviourEnabled(this->std::enable_shared_from_this<MonoBehaviourBase>::shared_from_this())
			: gameObject->SetMonoBehaviourDisabled(this->std::enable_shared_from_this<MonoBehaviourBase>::shared_from_this());

		// ������Ƽ ���� �ٲߴϴ�.
		_isEnabled = value;
	}

	void MonoBehaviourBase::StopCoroutine(const std::shared_ptr<Coroutine>& coroutine)
	{
		for (auto iter = _coroutineHandlers.begin() ; iter != _coroutineHandlers.end() ; )
		{
			// ���� �ڷ�ƾ �ڵ��̶�� .. �����ϴ�.
			// �ش� �ڷ�ƾ�� �Ҹ��ŵ�ϴ�.
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