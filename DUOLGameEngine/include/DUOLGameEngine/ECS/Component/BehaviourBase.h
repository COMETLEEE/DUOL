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
	class BehaviourBase : public DUOLGameEngine::ComponentBase, public std::enable_shared_from_this<BehaviourBase>
	{
	public:
		BehaviourBase(const std::weak_ptr<GameObject>& owner, const DUOLCommon::tstring& name = DUOLCommon::StringHelper::ToTString("Behaviour"));

		virtual ~BehaviourBase() override;

	protected:
		bool _isEnabled;

		bool _isActivedAndEnabled;

	public:
		/**
		 * \brief Behaviour가 사용 가능해졌을 때 호출됩니다.
		 */
		virtual void OnEnable() {}

		/**
		 * \brief Behaviour가 사용 불가능해졌을 때 호출됩니다.
		 */
		virtual void OnDisable() {}

		inline bool GetIsEnabled() const { return _isEnabled; }
		
		virtual void SetIsEnabled(bool value);

		/**
		 * \brief 게임 오브젝트가 Active이며 Behaviour가 Enable인지 확인합니다.
		 * \return (GameObject IsActive && Behaviour IsEnable)
		 */
		inline bool GetIsActiveAndEnabled() const { return _isActivedAndEnabled; }
	};
}