#pragma once
#include "DUOLGameEngine/ECS/Component/MonoBehaviourBase.h"

namespace DUOLClient
{
	/**
	 * \brief ���� �Ŵ���. �� �̵��� ���� �÷��̾��� ���� ���� ����, �ð�, ȯ�漳�� ���� ����Ѵ�.
	 */
	class GameManager : public DUOLGameEngine::MonoBehaviourBase
	{
	public:
		GameManager(DUOLGameEngine::GameObject* owner = nullptr, const DUOLCommon::tstring& name = TEXT("GameManager"));

		virtual ~GameManager() override;
		
	private:
		float	_testTime;

		bool	_isFirst;

		DUOLCommon::tstring _firstSceneName;

		DUOLCommon::tstring _secondSceneName;

	public:
		virtual void OnAwake() override;

		virtual void OnUpdate(float deltaTime) override;

		RTTR_ENABLE(DUOLGameEngine::MonoBehaviourBase)
	};
}