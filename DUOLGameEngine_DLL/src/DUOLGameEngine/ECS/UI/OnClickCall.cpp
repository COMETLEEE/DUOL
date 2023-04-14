#include "DUOLGameEngine/ECS/UI//OnClickCall.h"
#include "DUOLGameEngine/ECS/GameObject.h"
#include "DUOLCommon/MetaDataType.h"

#include <rttr/registration>

#include "DUOLGameEngine/ECS/UI/OnClickArgument.h"
#include <string>
#include <functional>

using namespace rttr;

RTTR_PLUGIN_REGISTRATION
{
	rttr::registration::class_<DUOLGameEngine::OnClickCall>("OnClickCall")
	.constructor()
	(
		rttr::policy::ctor::as_raw_ptr
	)
	.constructor<DUOLGameEngine::GameObject*, const DUOLCommon::tstring&>()
	(
		rttr::policy::ctor::as_raw_ptr
	)
	.property("GameObject", &DUOLGameEngine::OnClickCall::_targetGameObject)
	(
		metadata(DUOLCommon::MetaDataType::Serializable, true)
		, metadata(DUOLCommon::MetaDataType::Inspectable, true)
		, metadata(DUOLCommon::MetaDataType::SerializeByUUID, true)
		, metadata(DUOLCommon::MetaDataType::MappingType, DUOLCommon::MappingType::FileUUID)
		, metadata(DUOLCommon::MetaDataType::InspectType, DUOLCommon::InspectType::UIFileName)
	)
	.property("ComponentBase", &DUOLGameEngine::OnClickCall::_targetComponentBase)
	(
		metadata(DUOLCommon::MetaDataType::Serializable, true)
		, metadata(DUOLCommon::MetaDataType::SerializeByUUID, true)
		, metadata(DUOLCommon::MetaDataType::MappingType, DUOLCommon::MappingType::FileUUID)
	);
}

DUOLGameEngine::OnClickCall::OnClickCall() :
	DUOLGameEngine::ObjectBase(TEXT("OnClickCall"), ObjectType::UI)
	, _targetGameObject(nullptr)
	, _componentName(L"No Component")
	, _targetComponentBase(nullptr)
{
	Initialize();
}

DUOLGameEngine::OnClickCall::OnClickCall(DUOLGameEngine::GameObject* owner, const DUOLCommon::tstring& name) :
	DUOLGameEngine::ObjectBase(name, ObjectType::UI)
	, _targetGameObject(nullptr)
	, _componentName(L"No Component")
	, _targetComponentBase(nullptr)
{
	Initialize();
}

DUOLGameEngine::OnClickCall::~OnClickCall()
{
	//_clickArgument.reset();
}

void DUOLGameEngine::OnClickCall::OnUpdate(float deltaTime)
{

	//_clickArgument->OnUpdate(deltaTime);
}

void DUOLGameEngine::OnClickCall::Initialize()
{
	_clickArgument = std::make_shared<OnClickArgument>();
}

void DUOLGameEngine::OnClickCall::OnAwake()
{
	// 함수를 넣어준다.
	const instance obj = *_targetComponentBase;

	// 해당 인스턴스의 가장 아래에 있는 타입 (최종 자식 클래스 타입) 을 가져옵니다.
	const type class_type = obj.get_derived_type();

	rttr::method targetMethod = class_type.get_method(_clickArgument->GetFunctionName());

	auto parameterarry = targetMethod.get_parameter_infos();

	// 모드를 설정해줍니다.

	/*if (parameterarry.begin() == parameterarry.end())
	{
		_clickArgument->_onClickVoidVoid = (&targetMethod);
	}
	else if (_clickArgument->GetParameterName() == "int")
	{
		_clickArgument->_onClickVoidInt = (&targetMethod);
	}
	else if (_clickArgument->GetParameterName() == "bool")
	{
		_clickArgument->_onClickVoidBool = (&targetMethod);
	}
	else if (_clickArgument->GetParameterName() == "string")
	{
		_clickArgument->_onClickVoidString = (&targetMethod);
	}
	else if (_clickArgument->GetParameterName() == "float")
	{
		_clickArgument->_onClickVoidFloat = (&targetMethod);
	}*/
}

void DUOLGameEngine::OnClickCall::SetComponent(DUOLCommon::tstring componentname)
{
	if (_targetGameObject == nullptr)
		return;

	_componentName = componentname;

	for (auto component : _targetGameObject->GetAllComponents())
	{
		if (component->GetName() == componentname)
			_targetComponentBase = component;
	}

	SetArgument();
}

void DUOLGameEngine::OnClickCall::SetFunction(DUOLCommon::tstring function)
{
	// 이름을 설정해준다.
	SetFunctionName(function);

	auto totalname = DUOLCommon::StringHelper::ToString(function);

	std::size_t pos = totalname.find("(");

	std::string functionname = totalname.substr(0, pos);

	std::string parametername = totalname.substr(pos + 1);

	std::size_t endpos = parametername.find(")");

	parametername = parametername.substr(0, endpos);

	_clickArgument->_functionName = DUOLCommon::StringHelper::ToTString(functionname);

	_clickArgument->_paramterName = DUOLCommon::StringHelper::ToTString(parametername);

	// mode를 설정해준다. 
	if (parametername == "void")
	{
		_clickArgument->SetMode(OnClickEventFunctionMode::VoidVoid);
	}
	else if (parametername == "int")
	{
		_clickArgument->SetMode(OnClickEventFunctionMode::VoidInt);
	}
	else if (parametername == "bool")
	{
		_clickArgument->SetMode(OnClickEventFunctionMode::VoidBool);
	}
	else if (parametername == "string")
	{
		_clickArgument->SetMode(OnClickEventFunctionMode::VoidString);
	}
	else if (parametername == "float")
	{
		_clickArgument->SetMode(OnClickEventFunctionMode::VoidFloat);
	}
}

void DUOLGameEngine::OnClickCall::SetArgument()
{
	_clickArgument->ResetFunction();

	if (_targetComponentBase == nullptr)
		return;

	instance obj = *_targetComponentBase;

	_clickArgument->_targetComponentBase = _targetComponentBase;

	// 해당 인스턴스의 가장 아래에 있는 타입 (최종 자식 클래스 타입) 을 가져옵니다.
	const type class_type = obj.get_derived_type();

	auto methods = class_type.get_methods();

	for (const auto& method : methods)
	{
		// 인자가 1개만 들어온다고 가정한다. (여러개 넣지마)
		auto temp = method.get_parameter_infos();

		auto functionName = method.get_name();

		auto stringFunctionName = functionName.to_string();

		auto parameter = temp.begin();

		std::string parametername;

		if (parameter != temp.end())
		{
			auto parametertype = parameter->get_type();

			auto parameterData = parametertype.get_name();

			parametername = parameterData.to_string();

			std::size_t pos = parametername.find(":");

			if (pos != 0)
				parametername = parametername.substr(pos + 2, parametername.size());

		}
		else
			parametername = "void";

		_clickArgument->PushBackFunction(stringFunctionName, parametername);
	}

	SetNowFunctionNames();
}

void DUOLGameEngine::OnClickCall::SetNowFunctionNames()
{
	_nowFunctionNames.clear();

	auto argument = _clickArgument->GetFunction();

	for (auto function : argument)
	{
		auto outputName = function.first + "(" + function.second + ")";

		_nowFunctionNames.emplace_back(DUOLCommon::StringHelper::ToTString(outputName));
	}
}

void DUOLGameEngine::OnClickCall::SetParameter(const DUOLCommon::tstring& input)
{
	auto inputstring = DUOLCommon::StringHelper::ToString(input);

	switch (_clickArgument->GetMode())
	{
	case OnClickEventFunctionMode::VoidVoid:
		break;
	case OnClickEventFunctionMode::VoidInt:
	{
		int inputfloat = std::stoi(inputstring);
		_clickArgument->_intParameter = inputfloat;
		break;
	}
	case OnClickEventFunctionMode::VoidBool:
	{
		if (inputstring == "true" || inputstring == "True" || inputstring == "TRUE" || inputstring == "1")
			_clickArgument->_boolParameter = true;
		else if (inputstring == "false" || inputstring == "False" || inputstring == "FALSE" || inputstring == "0")
			_clickArgument->_boolParameter = false;

		break;
	}
	case OnClickEventFunctionMode::VoidString:
	{
		_clickArgument->_stringParameter = inputstring;
		break;
	}
	case OnClickEventFunctionMode::VoidFloat:
	{
		float inputfloat = std::stof(inputstring);
		_clickArgument->_floatParameter = inputfloat;
		break;
	}
	}
}

void DUOLGameEngine::OnClickCall::ResetComponent()
{
	_componentName = L"";
	_targetComponentBase = nullptr;
}

void DUOLGameEngine::OnClickCall::Invoke()
{
	if (_targetComponentBase == nullptr)
		return;

	const instance obj = *_targetComponentBase;

	// 해당 인스턴스의 가장 아래에 있는 타입 (최종 자식 클래스 타입) 을 가져옵니다.
	const type class_type = obj.get_derived_type();

	rttr::method targetMethod = class_type.get_method(_clickArgument->GetFunctionName());

	auto parameterarry = targetMethod.get_parameter_infos();

	if (parameterarry.begin() == parameterarry.end())
	{
		targetMethod.invoke(obj);
	}
	else if (_clickArgument->GetParameterName() == "int")
	{
		targetMethod.invoke(obj,_clickArgument->_intParameter);
	}
	else if (_clickArgument->GetParameterName() == "bool")
	{
		targetMethod.invoke(obj, _clickArgument->_boolParameter);
	}
	else if (_clickArgument->GetParameterName() == "string")
	{
		targetMethod.invoke(obj, _clickArgument->_stringParameter);
	}
	else if (_clickArgument->GetParameterName() == "float")
	{
		targetMethod.invoke(obj, _clickArgument->_floatParameter);
	}
	
	_clickArgument->Invoke();
}
