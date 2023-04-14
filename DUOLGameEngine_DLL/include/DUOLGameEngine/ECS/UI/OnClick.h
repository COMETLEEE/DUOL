#pragma once
#include "OnClickCall.h"
#include "DUOLGameEngine/ECS/Component/BehaviourBase.h"
#include "DUOLGameEngine/ECS/GameObject.h"
#include "rttr/registration_friend.h"

namespace DUOLGameEngine
{
	enum class BUTTONCALLBACKSTATE
	{
		// �̺�Ʈ�� �߻����� �ʽ��ϴ�. �׻� ������Ʈ�� ��������
		Off,
		// �����Ϳ� ��Ÿ���϶� ���ư� (�����Ϳ��� �÷��̸� ������ �ʾƵ� ���ư�)
		EditorAndRunTime,
		// ��Ÿ���϶��� ���ư� (�����Ϳ��� �÷��̸� �����߸� ���ư�)
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
		// GameObject�� �� �Լ��� ������. 
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

