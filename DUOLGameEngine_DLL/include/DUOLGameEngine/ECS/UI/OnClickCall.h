#pragma once
#include "OnClickArgument.h"
#include "DUOLGameEngine/ECS/Component/BehaviourBase.h"

namespace DUOLGameEngine
{
	enum class OnClickEventFunctionMode;
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

		void OnAwake();

	private:
		// 이걸 가지는 게임 오브젝트(여기선 버튼을 가지는 UI) 아이디
		DUOLGameEngine::GameObject* _targetGameObject;

		ComponentBase* _targetComponentBase;

		std::shared_ptr<DUOLGameEngine::OnClickArgument> _clickArgument;

		DUOLCommon::tstring _componentName;

		std::vector<DUOLCommon::tstring> _nowFunctionNames;

	public:

		DUOLGameEngine::GameObject* GetTargetObject() { return _targetGameObject; }

		DUOLGameEngine::OnClickEventFunctionMode& GetMode() { return _clickArgument->GetMode(); }

		DUOLCommon::tstring& GetComponentName();

		ComponentBase* GetComponentBase() { return _targetComponentBase; }

		std::vector<DUOLCommon::tstring>& GetFunctionNames() { return _nowFunctionNames; }

		DUOLCommon::tstring& GetFunctionName() { return _clickArgument->GetFunctionParameterName(); }

		DUOLCommon::tstring& GetParameter() { return _clickArgument->GetInputParameter(); }

		void SetComponentName(DUOLCommon::tstring& componentname) { _componentName = componentname; }

		void SetFunctionName(DUOLCommon::tstring nowfunctionname) { _clickArgument->SetFunctionName(nowfunctionname); }

		void SetComponent(DUOLCommon::tstring componentname);

		void SetTargetGameObject(DUOLGameEngine::GameObject* target) { _targetGameObject = target; }

		void SetMode(DUOLGameEngine::OnClickEventFunctionMode& mode) { _clickArgument->SetMode(mode); }

		void SetFunction(DUOLCommon::tstring function);

		void SetArgument();

		void SetNowFunctionNames();

		void SetParameterName(const DUOLCommon::tstring& input);

		void SetParameter();

		void ResetComponent();

		void Invoke();
#pragma region FRIEND_CLASS
		friend class GameObject;

		RTTR_ENABLE(DUOLGameEngine::ObjectBase)

		RTTR_REGISTRATION_FRIEND
#pragma endregion

	};
}
