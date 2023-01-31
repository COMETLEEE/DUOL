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
      @brief  방향벡터 기준으로 로테이션을 돌려준다.
      @param  lookVec - 
      @retval         - 
  **/
		DUOLMath::Vector3 CalcRotation(const DUOLMath::Vector3& lookVec);

	public:
		/**
		 * \brief 씬이 시작할 때 OnStart 보다 이전에 호출됩니다.
		 * 씬 시작 시 우선 순위의 게임 로직을 적용할 수 있습니다.
		 */
		void OnAwake() override;

		/**
		 * \brief 씬이 시작할 때 OnAwake 이후에 호출됩니다.
		 * 씬 시작 시 우선 순위의 게임 로직을 적용할 수 있습니다.
		 */
		void OnStart() override;

		/**
		 * \brief 컴포넌트가 파괴될 때 호출됩니다.
		 */
		void OnDestroy() override;

		/**
		 * \brief 매 프레임에 호출됩니다.
		 * \param deltaTime 프레임 간 시간 간격입니다.
		 */
		void OnUpdate(float deltaTime) override;

		/**
		 * \brief OnUpdate 이후, 매 프레임 호출됩니다.
		 * \param deltaTime 프레임 간 시간 간격입니다.
		 */
		void OnLateUpdate(float deltaTime) override;

	};
}