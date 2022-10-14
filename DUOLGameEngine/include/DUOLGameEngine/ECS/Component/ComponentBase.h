#pragma once
#include <cassert>

#include "DUOLGameEngine/ECS/GameObject.h"
#include "DUOLGameEngine/Util/Constants.h"
#include "DUOLGameEngine/Util/StringHelper.h"

namespace DUOLGameEngine
{
	class GameObject;

	class Transform;

	/**
	 * \brief ���� ������Ʈ�� ���� �� �ִ� ��� ������Ʈ���� �θ� Ŭ����
	 */
	class ComponentBase : public ObjectBase
	{
	public:
		/**
		 * \brief ��ӹ��� Ŭ�������� �ݵ�� ȣ��Ǿ�� �մϴ�.
		 * \param owner 
		 * \param name 
		 */
		ComponentBase(std::shared_ptr<GameObject> owner, const tstring& name = StringHelper::ToTString("Component"));

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
		 * \brief ������Ʈ�� ��� ���������� �� ȣ��˴ϴ�.
		 */
		virtual void OnEnable() {}

		/**
		 * \brief ������Ʈ�� ��� �Ұ��������� �� ȣ��˴ϴ�.
		 */
		virtual void OnDisable() {}

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

		/**
		 * \brief �ش� ������Ʈ�� Transform Component�Դϴ�.
		 */
		std::shared_ptr<Transform> _transform;
		
	public:
		inline std::shared_ptr<GameObject> GetGameObject() const
		{
			std::shared_ptr<GameObject> owner = _owner.lock();

			assert(owner != nullptr);

			return owner;
		}

		inline std::shared_ptr<Transform> GetTransform() const
		{
			return _transform;
		}

		inline const tstring& GetTag() const
		{
			const std::shared_ptr<GameObject> owner = _owner.lock();

			assert(owner != nullptr);

			return owner->GetTag();
		}

		inline bool CompareTag(const tstring& tag) const
		{
			const std::shared_ptr<GameObject> owner = _owner.lock();

			assert(owner != nullptr);

			return (tag == owner->GetTag());
		}
 	};
}