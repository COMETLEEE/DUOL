#pragma once
#include "DUOLGameEngine/ECS/Component/MonoBehaviourBase.h"
#include "DUOLGameEngine/ECS/Component/Rigidbody.h"

namespace DUOLGame
{
	class ThirdPersonCamera : public DUOLGameEngine::MonoBehaviourBase
	{
	public:
		ThirdPersonCamera(const std::weak_ptr<DUOLGameEngine::GameObject>& owner);

		virtual ~ThirdPersonCamera() override;

	private:
		DUOLGameEngine::GameObject* _targetObject;

		float _distance;

		float _moveSpeed;

		float _axisXLimit;

		DUOLMath::Vector2 _prevMousePosition;

	public:
		void SetTargetObject(DUOLGameEngine::GameObject* targetObject);

  /**
      @brief  ���⺤�� �������� �����̼��� �����ش�.
      @param  lookVec - 
      @retval         - 
  **/
		DUOLMath::Vector3 CalcRotation(const DUOLMath::Vector3& lookVec);

	public:
		/**
		 * \brief ���� ������ �� OnStart ���� ������ ȣ��˴ϴ�.
		 * �� ���� �� �켱 ������ ���� ������ ������ �� �ֽ��ϴ�.
		 */
		void OnAwake() override;

		/**
		 * \brief ���� ������ �� OnAwake ���Ŀ� ȣ��˴ϴ�.
		 * �� ���� �� �켱 ������ ���� ������ ������ �� �ֽ��ϴ�.
		 */
		void OnStart() override;

		/**
		 * \brief ������Ʈ�� �ı��� �� ȣ��˴ϴ�.
		 */
		void OnDestroy() override;

		/**
		 * \brief �� �����ӿ� ȣ��˴ϴ�.
		 * \param deltaTime ������ �� �ð� �����Դϴ�.
		 */
		void OnUpdate(float deltaTime) override;

		/**
		 * \brief OnUpdate ����, �� ������ ȣ��˴ϴ�.
		 * \param deltaTime ������ �� �ð� �����Դϴ�.
		 */
		void OnLateUpdate(float deltaTime) override;

	};
}