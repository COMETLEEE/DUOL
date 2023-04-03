#pragma once

#include "AI_EnemyBase.h"

namespace DUOLClient
{
	class DUOL_CLIENT_API AI_Enemy_Near final : public AI_EnemyBase
	{
	public:
		AI_Enemy_Near(DUOLGameEngine::GameObject* owner = nullptr, const DUOLCommon::tstring& name = TEXT("AI_Enemy_Near"));

		virtual ~AI_Enemy_Near() override = default;

	public:
		virtual void OnAwake() override;

		RTTR_ENABLE(AI_EnemyBase)
			RTTR_REGISTRATION_FRIEND
	};
}

