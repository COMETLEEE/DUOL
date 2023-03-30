#pragma once
#include "DUOLGameEngine/ECS/Component/BehaviourBase.h"

namespace DUOLGameEngine
{
	class OnClickCall;
}

namespace DUOLGameEngine
{

	class DUOL_GAMEENGINE_API OnClick final : public DUOLGameEngine::BehaviourBase
	{
	public:
		OnClick();

		OnClick(DUOLGameEngine::GameObject* owner, const DUOLCommon::tstring& name = TEXT("OnClick"));

		virtual ~OnClick();

		void OnUpdate(float deltaTime) override;

		void Initialize();

	private:
		std::vector<DUOLGameEngine::OnClickCall*> _persistentCall;

		int _callStateCount;

	public:
		inline std::vector<DUOLGameEngine::OnClickCall*>& GetCalls() { return _persistentCall; }

		void SetCalls(std::vector<DUOLGameEngine::OnClickCall*>& calls) { _persistentCall = calls; }

		inline int& GetCallStateCount() { return _callStateCount; }

#pragma region FRIEND_CLASS
		friend class GameObject;

		RTTR_ENABLE(DUOLGameEngine::BehaviourBase)

		RTTR_REGISTRATION_FRIEND
#pragma endregion

	};
}

