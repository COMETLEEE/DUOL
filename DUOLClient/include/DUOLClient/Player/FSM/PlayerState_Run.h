#pragma once
#include "PlayerStateBase.h"

namespace DUOLClient
{
	/**
	 * \brief 질주 상태. 해당 상태에서는 몬스터를 밀거나, 엎어트려서 데미지를 입히는 등의 특수 컨디션이 들어갈 수 있으니
	 * 따로 스테이트를 빼놓습니다.
	 */
	class PlayerState_Run : public DUOLClient::PlayerStateBase
	{
	public:
		PlayerState_Run();

		virtual ~PlayerState_Run() override;

	public:
		virtual void OnStateEnter(float deltaTime) override;

		virtual void OnStateStay(float deltaTime) override;

		virtual void OnStateExit(float deltaTime) override;
	};
}