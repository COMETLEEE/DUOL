#pragma once
#include "DUOLClient/Export_Client.h"
#include "DUOLGameEngine/ECS/Component/MonoBehaviourBase.h"

namespace DUOLClient
{
	/**
	 * \brief ������ �߽����� ȸ���ϴ� ��ü�� ����ϴ�.
	 */
	class DUOL_CLIENT_API RotateAroundOrigin : public DUOLGameEngine::MonoBehaviourBase
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