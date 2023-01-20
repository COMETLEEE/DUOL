#pragma once
#include <cassert>

#include "DUOLGameEngine/ECS/ObjectBase.h"
#include "DUOLCommon/StringHelper.h"

#include "rttr/registration_friend.h"
#include "rttr/rttr_enable.h"

namespace DUOLGameEngine
{
	class GameObject;

	class Transform;

	/**
	 * \brief ���� ������Ʈ�� ���� �� �ִ� ��� ������Ʈ���� �θ� Ŭ����
	 */
	class DUOL_GAMEENGINE_API ComponentBase : public ObjectBase
	{
	public:
		/**
		 * \brief ��ӹ��� Ŭ�������� �ݵ�� ȣ��Ǿ�� �մϴ�.
		 * \param owner �ش� ������Ʈ�� ������ ���� ������Ʈ
		 * \param name �ش� ������Ʈ�� �̸�
		 */
		ComponentBase(const std::weak_ptr<GameObject>& owner, const DUOLCommon::tstring& name = DUOLCommon::StringHelper::ToTString("Component"));

		/**
		 * \brief �������� ������ �Ҹ���
		 */
		virtual ~ComponentBase() override;

		DEFINE_DEFAULT_COPY_MOVE(ComponentBase)

		/**
		 * \brief ���� ������ �� OnStart ���� ������ ȣ��˴ϴ�.
		 * �� ���� �� �켱 ������ ���� ������ ������ �� �ֽ��ϴ�.
		 */
		virtual void OnAwake() {}

		/**
		 * \brief ���� ������ �� OnAwake ���Ŀ� ȣ��˴ϴ�.
		 * �� ���� �� �켱 ������ ���� ������ ������ �� �ֽ��ϴ�.
		 */
		virtual void OnStart() {}

		/**
		 * \brief ������Ʈ�� �ı��� �� ȣ��˴ϴ�.
		 */
		virtual void OnDestroy() {}

		/**
		 * \brief �� �����ӿ� ȣ��˴ϴ�.
		 * \param deltaTime ������ �� �ð� �����Դϴ�.
		 */
		virtual void OnUpdate(float deltaTime) {}

		/**
		 * \brief �� ���� �����ӿ� ȣ��˴ϴ�.
		 * \param deltaTime ������ �� �ð� �����Դϴ�.
		 */
		virtual void OnFixedUpdate(float deltaTime) {}

		/**
		 * \brief OnUpdate ����, �� ������ ȣ��˴ϴ�. 
		 * \param deltaTime ������ �� �ð� �����Դϴ�.
		 */
		virtual void OnLateUpdate(float deltaTime) {}

	private:
		/**
		 * \brief �ش� ������Ʈ�� �����ϰ� �ִ� ���� ������Ʈ�Դϴ�.
		 * ��ȯ ������ ���� ���� std::weak_ptr�� �����մϴ�.
		 */
		std::weak_ptr<GameObject> _owner;

	public:
		inline GameObject* GetGameObject() const
		{
			std::shared_ptr<GameObject> owner = _owner.lock();

			assert(owner != nullptr);

			return owner.get();
		}

		Transform* GetTransform() const;

		const DUOLCommon::tstring& GetTag() const;

		bool CompareTag(const DUOLCommon::tstring& tag) const;

#pragma region
		RTTR_ENABLE()

		RTTR_REGISTRATION_FRIEND
#pragma endregion
 	};
}