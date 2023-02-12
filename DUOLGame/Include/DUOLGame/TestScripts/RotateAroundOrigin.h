#pragma once
#include "DUOLGameEngine/ECS/Component/MonoBehaviourBase.h"

namespace DUOLGame
{
	/**
	 * \brief ������ �߽����� ȸ���ϴ� ��ü�� ����ϴ�.
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