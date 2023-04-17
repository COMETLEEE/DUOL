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
		ComponentBase* _targetComponentBase;

		int _intParameter;

		bool _boolParameter;

		std::string _stringParameter;

		float _floatParameter;

		DUOLCommon::tstring _functionParameterName;

		DUOLCommon::tstring _functionName;

		DUOLCommon::tstring _paramterName;

		DUOLCommon::tstring _inputParameter;

	public:
		DUOLGameEngine::OnClickEventFunctionMode& GetMode() { return _methodID; }

		std::string GetFunctionName()  { return DUOLCommon::StringHelper::ToString(_functionName); }

		std::string GetParameterName()  { return DUOLCommon::StringHelper::ToString(_paramterName); }

		DUOLCommon::tstring& GetFunctionParameterName() { return _functionParameterName; }

		// parameter
		int& GetIntParameter() { return _intParameter; }

		bool& GetBoolParameter() { return _boolParameter; }

		std::string& GetStringParameter() { return _stringParameter; }

		float& GetFloatParameter() { return _floatParameter; }

		 DUOLCommon::tstring& GetInputParameter() { return _inputParameter; }

		void SetInputParameter(DUOLCommon::tstring& inputparameter) { _inputParameter = inputparameter; }

		void SetIntParameter(int& intparameter) { _intParameter = intparameter; }

		void SetBoolParameter(bool& boolparameter) { _boolParameter = boolparameter; }

		void SetStringParameter(std::string& stringparameter) { _stringParameter = stringparameter; }

		void SetFloatParameter(float& floatparameter) { _floatParameter = floatparameter; }

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


