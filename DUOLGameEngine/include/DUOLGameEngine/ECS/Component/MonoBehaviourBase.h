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

namespace DUOLGameEngine
{
	struct Collision;

	class ColliderBase;

	class Coroutine
	{
		
	};



	/**
	 * \brief 스크립트 (== 커스텀 컴포넌트) 의 기본 클래스입니다.
	 * 스크립트는 해당 게임 오브젝트의 생애 동안 조작을 담당합니다.
	 */
	class MonoBehaviourBase : public BehaviourBase
	{
	public:
		MonoBehaviourBase(std::shared_ptr<DUOLGameEngine::GameObject> owner, const DUOLCommon::tstring& name = DUOLCommon::StringHelper::ToTString("MonoBehaviour"));

		virtual ~MonoBehaviourBase() override;

		DEFINE_DEFAULT_COPY_MOVE(MonoBehaviourBase)

		virtual void OnCollisionEnter(std::shared_ptr<Collision> collision) { }

		virtual void OnCollisionStay(std::shared_ptr<Collision> collision) { }

		virtual void OnCollisionExit(std::shared_ptr<Collision> collision) { }

		virtual void OnTriggerEnter(std::shared_ptr<ColliderBase> collision) { }

		virtual void OnTriggerStay(std::shared_ptr<ColliderBase> other) { }

		virtual void OnTriggerExit(std::shared_ptr<ColliderBase> other) { }

		// 코루틴 매니저, 인보크 매니저 등 있어야합니다 ..
		// MonoBehaviour에서 그 역할을 수행하고 Update와 관련되어, MonoBehaviour에 관련되어
		// 따로 게임 오브젝트 내에서 AddComponent 할 때 캐싱하고, 이벤트 루프 함수들을
		// 이 개체에서만 수행할 수 있도록 지정한다. (컴포넌트 리스트에서 찾는 일이 없도록 한다는 것)

		// Coroutine StartCoroutine(IEnumerator routine);

		// void Invoke(std::function<void(void)> function, float time);

		// void CancleInvoke();

		// void StopAllCoroutines();

		// void StopCoroutine(Coroutine routine);

		// void StopCoroutine(IEnumerator routine);
	};
}