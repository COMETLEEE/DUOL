#pragma once
#include "DUOLGameEngine/ECS/Component/MonoBehaviourBase.h"

namespace DUOLClient
{
	/**
	 * \brief Transform �� �� ������Ƽ�� 'Set' �ϴ� ������� ��Ʈ���մϴ�.
	 * ī�޶� ����, ������ ���� � ����� �� �ֽ��ϴ�.
	 */
	class TransformController : public DUOLGameEngine::MonoBehaviourBase
	{
	public:
		TransformController(DUOLGameEngine::GameObject* owner = nullptr, const DUOLCommon::tstring& name = TEXT("TransformController"));

		virtual ~TransformController();

	private:

	public:
		virtual void OnLateUpdate(float deltaTime) override;
	};
}
