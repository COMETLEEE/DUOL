#pragma once
#include "DUOLGameEngine/ECS/Component/BehaviourBase.h"

namespace DUOLGameEngine
{
	enum class OnClickEventFunctionMode
	{
		VoidVoid
		, VoidBool
		, VoidInt
		, VoidString
		, VoidFloat
	};
}


namespace DUOLGameEngine
{
	class GameObject;
	class OnClickArgument;

}

namespace DUOLGameEngine
{

	class DUOL_GAMEENGINE_API OnClickCall final:public DUOLGameEngine::ObjectBase, public std::enable_shared_from_this<OnClickCall>
	{
	public:
		OnClickCall();

		OnClickCall(DUOLGameEngine::GameObject* owner, const DUOLCommon::tstring& name = TEXT("OnClickCall"));

		virtual ~OnClickCall();

		void OnUpdate(float deltaTime);

		void Initialize();

	private:
		DUOLGameEngine::GameObject* _owner;

		// �̰� ������ ���� ������Ʈ(���⼱ ��ư�� ������ UI) ���̵�
		DUOLGameEngine::GameObject* _targetGameObject;

		// � Mode����
		DUOLGameEngine::OnClickEventFunctionMode _methodID;

		//std::shared_ptr<DUOLGameEngine::OnClickArgument> _clickArgument;

	public:
		DUOLGameEngine::GameObject* GetTargetObject() { return _targetGameObject; }

		DUOLGameEngine::OnClickEventFunctionMode& GetMode() { return _methodID; }

		void SetTargetGameObject(DUOLGameEngine::GameObject* target) { _targetGameObject = target; }

		void SetMode(DUOLGameEngine::OnClickEventFunctionMode& mode) { _methodID = mode; }

#pragma region FRIEND_CLASS
		friend class GameObject;

		RTTR_ENABLE(DUOLGameEngine::ObjectBase)

		RTTR_REGISTRATION_FRIEND
#pragma endregion

	};
}
