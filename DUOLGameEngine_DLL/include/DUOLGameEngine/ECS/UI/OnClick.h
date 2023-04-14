#pragma once
#include "OnClickCall.h"
#include "DUOLGameEngine/ECS/Component/BehaviourBase.h"
#include "DUOLGameEngine/ECS/GameObject.h"
#include "rttr/registration_friend.h"

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
	class DUOL_GAMEENGINE_API OnClick final :public DUOLGameEngine::ObjectBase, public std::enable_shared_from_this<OnClick>
	{
	public:
		OnClick();

		OnClick(DUOLGameEngine::GameObject* owner, const DUOLCommon::tstring& name = TEXT("OnClick"));

		virtual ~OnClick();

		void OnUpdate(float deltaTime);

		void Initialize();

		void OnAwake();
	private:
		// GameObject와 그 함수를 가진다. 
		std::shared_ptr<DUOLGameEngine::OnClickCall> _persistentCall;

		BUTTONCALLBACKSTATE _callBackState;

	public:
		inline DUOLGameEngine::OnClickCall* GetCalls() { return _persistentCall.get(); }

		inline DUOLGameEngine::OnClickEventFunctionMode& GetMode() { return _persistentCall->GetMode(); }

		void SetCalls(std::shared_ptr<DUOLGameEngine::OnClickCall> calls) { _persistentCall = calls; }

		inline BUTTONCALLBACKSTATE& GetCallBackState() { return _callBackState; }

		inline void SetCallBackState(BUTTONCALLBACKSTATE& callbackstate) { _callBackState = callbackstate; }


		void Invoke();
#pragma region FRIEND_CLASS
		friend class GameObject;

		RTTR_ENABLE(DUOLGameEngine::ObjectBase)

		RTTR_REGISTRATION_FRIEND
#pragma endregion

	};
}

