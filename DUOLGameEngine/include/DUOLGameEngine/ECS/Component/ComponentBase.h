#pragma once
#include "ObjectBase.h"
#include "DUOLGameEngine/Util/Constants.h"

namespace DUOLGameEngine
{
	class GameObject;

	class Transform;

	/**
	 * \brief ���Ϳ� ���� �� �ִ� ��� ������Ʈ���� �θ� Ŭ����
	 */
	class ComponentBase : public ObjectBase
	{
	public:
		/**
		 * \brief ��ӹ��� Ŭ�������� �ݵ�� ȣ��Ǿ�� �մϴ�.
		 * \param actor 
		 */
		ComponentBase(std::shared_ptr<GameObject> owner);

		/**
		 * \brief �������� ������ �Ҹ���
		 */
		virtual ~ComponentBase();

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
		 * \param deltaTime 
		 */
		virtual void OnUpdate(float deltaTime) {}

		/**
		 * \brief �� ���� �����ӿ� ȣ��˴ϴ�.
		 * \param deltaTime 
		 */
		virtual void OnFixedUpdate(float deltaTime) {}

		/**
		 * \brief OnUpdate ����, �� ������ ȣ��˴ϴ�. 
		 * \param deltaTime 
		 */
		virtual void OnLateUpdate(float deltaTime) {}

	private:
		std::shared_ptr<GameObject> _owner;

		std::shared_ptr<Transform> _transform;

		/**
		 * \brief ���� ������Ʈ�� �±��Դϴ�. Tag�� Layer Manager���� ���� �� ����մϴ�.
		 */
		tstring _tag;

	public:

 	};
}