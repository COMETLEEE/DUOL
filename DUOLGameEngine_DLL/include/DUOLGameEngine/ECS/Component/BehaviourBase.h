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
	class DUOL_GAMEENGINE_API BehaviourBase : public DUOLGameEngine::ComponentBase, public std::enable_shared_from_this<BehaviourBase>
	{
	public:
		BehaviourBase(const std::weak_ptr<GameObject>& owner, const DUOLCommon::tstring& name = DUOLCommon::StringHelper::ToTString("Behaviour"));

		virtual ~BehaviourBase() override;

	protected:
		/**
		 * \brief ���� �� Behaviour�� ��� ������ �����ΰ�
		 */
		bool _isEnabled;

		/**
		 * \brief Start �Լ��� ����Ǿ����� ?
		 */
		bool _isStarted;

	public:
		/**
		 * \brief Behaviour�� ��� ���������� �� ȣ��˴ϴ�.
		 */
		virtual void OnEnable() {}

		/**
		 * \brief Behaviour�� ��� �Ұ��������� �� ȣ��˴ϴ�.
		 */
		virtual void OnDisable() {}

		/**
		 * \brief �ش� Behaviour�� On / Off ����մϴ�.
		 * \return ��� ���̸� true, �̻�� ���̸� false�� ��ȯ�մϴ�.
		 */
		inline bool GetIsEnabled() const { return _isEnabled; }

		/**
		 * \brief �ش� Behaviour�� On / Off �ϴµ� ����ϴ� �Լ��Դϴ�.
		 * \param value �����ϰ��� �ϴ� ��� �����Դϴ�.
		 */
		virtual void SetIsEnabled(bool value);

		/**
		 * \brief ���� ������Ʈ�� Active�̸� Behaviour�� Enable���� Ȯ���մϴ�.
		 * \return (GameObject IsActive && Behaviour IsEnable)
		 */
		bool GetIsActiveAndEnabled() const;

		RTTR_ENABLE(ComponentBase)

#pragma region FRIEND_CLASS
		friend class GameObject;
#pragma endregion
	};
}