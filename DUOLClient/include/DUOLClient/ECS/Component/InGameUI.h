#pragma once
#include "DUOLClient/Export_Client.h"
#include "DUOLGameEngine/ECS/Component/MonoBehaviourBase.h"

namespace DUOLClient
{ 
	/**
	 * \brief �ΰ����� UI�� �����͸� ���ӿ����� UIManager�� �Ѱ��ְ� �̸� �������� �����ϰ� ����ϴ�.
	 */
	class DUOL_CLIENT_API InGameUI final :public DUOLGameEngine::MonoBehaviourBase
	{
	public:
		InGameUI(DUOLGameEngine::GameObject* owner=nullptr, const DUOLCommon::tstring& name = DUOLCommon::StringHelper::ToTString("InGameUI"));

		virtual ~InGameUI() override;

		virtual void OnUpdate(float deltaTime) override;

		// UIMode�� �ٲ��ݴϴ�.
		void UIMode();

		// BulletTimeAllMode�� �ٲ��ݴϴ�.
		// �� ������
		void BulletTimeAllMode();

		// BulletTimePlayerMode
		// �÷��̾�� �� ������
		void BulletTimePlayerMode();

		// �⺻ ���� ���
		void OutInGameUIMode();

		// Ŭ�����ϰ� ���� ��
		void ClearAndGoMain();

		// ������ ���� MonoBehaviourBase���� Ŭ���� �������� Reflection �� �� �ֵ��� �������ݴϴ�.
		RTTR_ENABLE(DUOLGameEngine::MonoBehaviourBase)

		RTTR_REGISTRATION_FRIEND

	};

}