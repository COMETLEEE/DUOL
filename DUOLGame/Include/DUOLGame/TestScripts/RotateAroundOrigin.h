#pragma once
#include "DUOLGameEngine/ECS/Component/MonoBehaviourBase.h"

namespace DUOLGame
{
	/**
	 * \brief 원점을 중심으로 회전하는 객체로 만듭니다.
	 */
	class RotateAroundOrigin : public DUOLGameEngine::MonoBehaviourBase
	{
	public:
		RotateAroundOrigin(DUOLGameEngine::GameObject* owner);

		virtual ~RotateAroundOrigin() override;

	private:
		DUOLGameEngine::Transform* _myTransform;

		float _rotateSpeed;

	public:
		virtual void OnUpdate(float deltaTime) override;

		float GetRotateSpeed() const;

		void SetRotateSpeed(float rotateSpeed);
	};
}