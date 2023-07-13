#pragma once
#include "DUOLClient/Export_Client.h"
#include "DUOLGameEngine/ECS/Component/MonoBehaviourBase.h"

namespace DUOLClient
{
	/**
	 * \brief 2, 3 �����ε����� ���׸��� �����͸� �����ϱ����� ���� ������Ʈ
	 */
	class DUOL_CLIENT_API MaterialOptionChanger : public DUOLGameEngine::BehaviourBase
	{
	public:
		MaterialOptionChanger(DUOLGameEngine::GameObject* owner = nullptr, const DUOLCommon::tstring& name = TEXT("MaterialOptionChanger"));

		virtual ~MaterialOptionChanger() override;

	public:

		/**
	 * \brief ���� ������ �� OnStart ���� ������ ȣ��˴ϴ�.
	 * �� ���� �� �켱 ������ ���� ������ ������ �� �ֽ��ϴ�.
	 */
		virtual void OnAwake() override;

		virtual void OnStart() override;


		void UpdateCurrentInfo();

		int GetMatIdx() const;

		void SetMatIdx(int matIdx);

		int _matIdx;


		DUOLMath::Vector2 _tilingInfo;

		RTTR_ENABLE(DUOLGameEngine::BehaviourBase)
			RTTR_REGISTRATION_FRIEND
	};
}