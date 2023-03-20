#pragma once
#include "DUOLGameEngine/ECS/Component/MonoBehaviourBase.h"

namespace DUOLClient
{
	/**
	 * \brief �� �𵨰� �ִϸ��̼� ���� �������� �޶� ����� ������ .. ���� �����κ��� Ż ������ �� ������ �ٲ��ִ� ���� ����
	 */
	class Weapon_Scaler : public DUOLGameEngine::MonoBehaviourBase
	{
	public:
		Weapon_Scaler(DUOLGameEngine::GameObject* owner = nullptr, const DUOLCommon::tstring& name = TEXT("Weapon_Scaler"));

		virtual ~Weapon_Scaler() override;

	private:
		bool _scaled;

	public:
		virtual void OnUpdate(float deltaTime) override;

		RTTR_ENABLE(DUOLGameEngine::MonoBehaviourBase)
	};
}