#pragma once
#include "DUOLGameEngine/ECS/Component/BehaviourBase.h"

namespace DUOLGameEngine
{
	enum class BUTTONCALLBACKSTATE
	{
		// 이벤트가 발생하지 않습니다. 항상 업데이트가 꺼져있음
		Off,
		// 에디터와 런타임일때 돌아감 (에디터에서 플레이를 누르지 않아도 돌아감)
		EditorAndRunTime,
		// 런타임일때만 돌아감 (에디터에서 플레이를 눌러야만 돌아감)
		RuntimeOnly
	};
}

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

		BUTTONCALLBACKSTATE _callBackState;

	public:
		inline std::vector<DUOLGameEngine::OnClickCall*>& GetCalls() { return _persistentCall; }

		void SetCalls(std::vector<DUOLGameEngine::OnClickCall*>& calls) { _persistentCall = calls; }

		inline BUTTONCALLBACKSTATE& GetCallStateCount() { return _callBackState; }

#pragma region FRIEND_CLASS
		friend class GameObject;

		RTTR_ENABLE(DUOLGameEngine::BehaviourBase)

		RTTR_REGISTRATION_FRIEND
#pragma endregion

	};
}

