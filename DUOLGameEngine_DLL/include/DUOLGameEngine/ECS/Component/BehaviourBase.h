#pragma once
#include <memory>

#include "DUOLGameEngine/ECS/Component/ComponentBase.h"
#include "DUOLGameEngine/Util/enabled_shared_from_base.h"

namespace DUOLGameEngine
{
	class GameObject;

	/**
	 * \brief 활성화, 비활성화를 선택할 수 있는 컴포넌트입니다.
	 */
	class DUOL_GAMEENGINE_API BehaviourBase : public DUOLGameEngine::ComponentBase, public std::enable_shared_from_this<BehaviourBase>
	{
	public:
		BehaviourBase(const std::weak_ptr<GameObject>& owner, const DUOLCommon::tstring& name = DUOLCommon::StringHelper::ToTString("Behaviour"));

		virtual ~BehaviourBase() override;

	protected:
		/**
		 * \brief 현재 이 Behaviour가 사용 가능한 상태인가
		 */
		bool _isEnabled;

		/**
		 * \brief Start 함수가 실행되었나요 ?
		 */
		bool _isStarted;

	public:
		/**
		 * \brief Behaviour가 사용 가능해졌을 때 호출됩니다.
		 */
		virtual void OnEnable() {}

		/**
		 * \brief Behaviour가 사용 불가능해졌을 때 호출됩니다.
		 */
		virtual void OnDisable() {}

		/**
		 * \brief 해당 Behaviour가 On / Off 사용합니다.
		 * \return 사용 중이면 true, 미사용 중이면 false를 반환합니다.
		 */
		inline bool GetIsEnabled() const { return _isEnabled; }

		/**
		 * \brief 해당 Behaviour를 On / Off 하는데 사용하는 함수입니다.
		 * \param value 지정하고자 하는 사용 상태입니다.
		 */
		virtual void SetIsEnabled(bool value);

		/**
		 * \brief 게임 오브젝트가 Active이며 Behaviour가 Enable인지 확인합니다.
		 * \return (GameObject IsActive && Behaviour IsEnable)
		 */
		bool GetIsActiveAndEnabled() const;

		RTTR_ENABLE(ComponentBase)

#pragma region FRIEND_CLASS
		friend class GameObject;
#pragma endregion
	};
}