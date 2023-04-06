#pragma once
#include "DUOLGameEngine/ECS/Component/MonoBehaviourBase.h"

namespace DUOLClient
{
	/**
	 * \brief 검 모델과 애니메이션 모델의 스케일이 달라서 생기는 문제다 .. 무기 본으로부터 탈 부착할 때 스케일 바꿔주는 역할 수행
	 */
	class Weapon_Scaler : public DUOLGameEngine::MonoBehaviourBase
	{
	public:
		Weapon_Scaler(DUOLGameEngine::GameObject* owner = nullptr, const DUOLCommon::tstring& name = TEXT("Weapon_Scaler"));

		virtual ~Weapon_Scaler() override;

	public:
		virtual void OnAwake() override;

		RTTR_ENABLE(DUOLGameEngine::MonoBehaviourBase)
	};
}