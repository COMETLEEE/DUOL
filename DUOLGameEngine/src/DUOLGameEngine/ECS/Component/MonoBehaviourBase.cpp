#include "DUOLGameEngine/ECS/Component/MonoBehaviourBase.h"

#include <iostream>

#include "DUOLGameEngine/ECS/GameObject.h"

namespace DUOLGameEngine
{
	MonoBehaviourBase::MonoBehaviourBase(const std::weak_ptr<DUOLGameEngine::GameObject>& owner, const DUOLCommon::tstring& name) :
		BehaviourBase(owner, name)
		, enable_shared_from_base<DUOLGameEngine::MonoBehaviourBase, DUOLGameEngine::BehaviourBase>()
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

		/*value == true ? gameObject->SetMonoBehaviourEnabled(shared_from_this())
			: gameObject->SetMonoBehaviourDisabled(shared_from_this());*/

		value == true ? gameObject->SetMonoBehaviourEnabled(shared_from_base())
			: gameObject->SetMonoBehaviourDisabled(shared_from_base());

		// ���� �ٲߴϴ�.
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

	void MonoBehaviourBase::Invoke(void(* func)(), float time)
	{
		_invokeReservedFunctions.push_back({ func, time });
	}

	void MonoBehaviourBase::CancleAllInvokes()
	{
		_invokeThisFrameFunctions.clear();

		_invokeReservedFunctions.clear();
	}

	void MonoBehaviourBase::CancleInvoke(void(* func)())
	{
		std::function<void()> functor = func;

		std::erase_if(_invokeReservedFunctions, [func](const std::pair<std::function<void()>, float>& elem)
			{
				return *elem.first.target<void(*)(void)>() == func
					? true : false;
			});

		std::erase_if(_invokeThisFrameFunctions, [func](const std::function<void()>& elem)
			{
				return *elem.target<void(*)(void)>() == func
					? true : false;
			});
	}

	void MonoBehaviourBase::UpdateAllInvokes(float deltaTime)
	{
		// ����Ǿ��� �Լ��� �� ���� �ð��� ��� ���� �Լ����� ȣ���մϴ�.
		for (auto iter = _invokeThisFrameFunctions.begin() ; iter != _invokeThisFrameFunctions.end() ; )
		{
			// Invoke !
			(*(iter++))();
		}

		_invokeThisFrameFunctions.clear();

		// Invoke Reserved Time Update
		for (auto iter = _invokeReservedFunctions.begin() ; iter != _invokeReservedFunctions.end() ; )
		{
			iter->second -= deltaTime;

			// �ð��� �� �Ǿ����� ���� �����ӿ� ȣ���ŵ�ϴ�.
			if (iter->second < 0.f)
			{
				_invokeThisFrameFunctions.push_back(iter->first);

				_invokeReservedFunctions.erase(iter++);

				continue;
			}
			else
				iter++;
		}
	}
}