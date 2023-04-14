#pragma once
#include "DUOLGameEngine/ECS/Component/BehaviourBase.h"
#include <string>
#include <rttr/method.h>

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
}

namespace DUOLGameEngine
{
	class DUOL_GAMEENGINE_API OnClickArgument final :public DUOLGameEngine::ObjectBase, public std::enable_shared_from_this<OnClickArgument>
	{
	public:
		OnClickArgument();

		OnClickArgument(DUOLGameEngine::GameObject* owner, const DUOLCommon::tstring& name = TEXT("OnClickArgument"));

		virtual ~OnClickArgument();

		void OnUpdate(float deltaTime);

		void Initialize();

	public:
		/*std::function<void(void)> _onClickVoidVoid;

		std::function<void(int)> _onClickVoidInt;

		std::function<void(bool)> _onClickVoidBool;

		std::function<void(std::string)> _onClickVoidString;

		std::function<void(float)> _onClickVoidFloat;*/
		ComponentBase* _targetComponentBase;

		int _intParameter;

		bool _boolParameter;

		std::string _stringParameter;

		float _floatParameter;

		DUOLCommon::tstring _functionParameterName;

		DUOLCommon::tstring _functionName;

		DUOLCommon::tstring _paramterName;

	public:
		DUOLGameEngine::OnClickEventFunctionMode& GetMode() { return _methodID; }

		std::string GetFunctionName() { return DUOLCommon::StringHelper::ToString(_functionName); }

		std::string GetParameterName() { return DUOLCommon::StringHelper::ToString(_paramterName); }

		DUOLCommon::tstring& GetFunctionParameterName() { return _functionParameterName; }

		void SetFunctionName(DUOLCommon::tstring& nowfunctionname) { _functionParameterName = nowfunctionname; }

		void SetMode(DUOLGameEngine::OnClickEventFunctionMode mode) { _methodID = mode; }

		void PushBackFunction(std::string& functionname, std::string& parameter);

		std::vector<std::pair<std::string, std::string>>& GetFunction() { return _functions; }

		void ResetFunction();

		void Invoke();

	private:
		// 어떤 Mode인지
		DUOLGameEngine::OnClickEventFunctionMode _methodID;

		std::vector<std::pair<std::string,std::string>> _functions;

#pragma region FRIEND_CLASS
		friend class GameObject;

		RTTR_ENABLE(DUOLGameEngine::ObjectBase)

		RTTR_REGISTRATION_FRIEND
#pragma endregion
	};
}


