#pragma once
#include <memory>

#include "DUOLGameEngine/ECS/Component/ComponentBase.h"
#include "DUOLGameEngine/Util/enabled_shared_from_base.h"

namespace DUOLGameEngine
{
	class GameObject;

	/**
	 * \brief Ȱ��ȭ, ��Ȱ��ȭ�� ������ �� �ִ� ������Ʈ�Դϴ�.
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
		 * \brief Behaviour�� ��� ���������� �� ȣ��˴ϴ�.
		 */
		virtual void OnEnable() {}

		/**
		 * \brief Behaviour�� ��� �Ұ��������� �� ȣ��˴ϴ�.
		 */
		virtual void OnDisable() {}

		inline bool GetIsEnabled() const { return _isEnabled; }
		
		virtual void SetIsEnabled(bool value);

		/**
		 * \brief ���� ������Ʈ�� Active�̸� Behaviour�� Enable���� Ȯ���մϴ�.
		 * \return (GameObject IsActive && Behaviour IsEnable)
		 */
		inline bool GetIsActiveAndEnabled() const { return _isActivedAndEnabled; }
	};
}