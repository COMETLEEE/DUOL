/**

    @file      MonoBehaviourBase.h
    @brief     MonoBehaviour is a base class that many DUOL Scripts derive from.
    @details   ~
    @author    COMETLEE
    @date      25.10.2022
    @copyright © COMETLEE, 2022. All right reserved.

**/
#pragma once
#include "DUOLGameEngine/ECS/Component/BehaviourBase.h"
#include "DUOLGameEngine/Util/Coroutine/Coroutine.h"

namespace DUOLGameEngine
{
	struct Collision;

	class ColliderBase;

	/**
	 * \brief 스크립트 (== 커스텀 컴포넌트) 의 기본 클래스입니다.
	 * 스크립트는 해당 게임 오브젝트의 생애 동안 조작을 담당합니다.
	 */
	class MonoBehaviourBase : public BehaviourBase
	{
	public:
		MonoBehaviourBase(const std::weak_ptr<DUOLGameEngine::GameObject>& owner, const DUOLCommon::tstring& name = DUOLCommon::StringHelper::ToTString("MonoBehaviour"));

		virtual ~MonoBehaviourBase() override;

		DEFINE_DEFAULT_COPY_MOVE(MonoBehaviourBase)

		virtual void OnCollisionEnter(std::shared_ptr<Collision> collision) { }

		virtual void OnCollisionStay(std::shared_ptr<Collision> collision) { }

		virtual void OnCollisionExit(std::shared_ptr<Collision> collision) { }

		virtual void OnTriggerEnter(std::shared_ptr<ColliderBase> collision) { }

		virtual void OnTriggerStay(std::shared_ptr<ColliderBase> other) { }

		virtual void OnTriggerExit(std::shared_ptr<ColliderBase> other) { }

#pragma region COROUTINE
		// 아직 흐름으로 남아있는 코루틴 함수들의 리스트
		std::list<DUOLGameEngine::CoroutineHandler> _coroutineHandlers;

		template <typename ...Types>
		std::shared_ptr<Coroutine> StartCoroutine(DUOLGameEngine::CoroutineHandler(*routine)(Types...), Types... args);

		void StopCoroutine(const std::shared_ptr<DUOLGameEngine::Coroutine>& coroutine);

		// 함수의 비교는 아직 힘들다 .. => 리플렉션 ..? 컴파일할 때 코루틴 함수들을 전역 테이블에 등록 ..?
		/*template <typename ...Types>
		void StopCoroutine(CoroutineHandler(*routine)(Types...));*/

		void StopAllCoroutines();

	private:
		void UpdateAllCoroutines(float deltaTime);
#pragma endregion

#pragma region INVOKE
	public:
		// void Invoke(std::function<void(void)> function, float time);

		// void CancleInvoke();
#pragma endregion

#pragma region FRIEND_CLASS
		friend class GameObject;
#pragma endregion
	};

	template <typename ...Types>
	std::shared_ptr<Coroutine> MonoBehaviourBase::StartCoroutine(DUOLGameEngine::CoroutineHandler(*routine)(Types...), Types... args)
	{
		// 루틴 시작하면서 반환되는 코루틴 핸들러의 복사본을 리스트에 넣는다.
		_coroutineHandlers.push_back(routine(args...));

		// 해당 코루틴 핸들러를 Wrapping 하는 코루틴 객체를 만들어서 반환한다. (For Coroutine Chain)
		return std::make_shared<Coroutine>(_coroutineHandlers.back());
	}  
}