#include "DUOLGameEngine/ECS/UI//OnClickArgument.h"
#include "DUOLGameEngine/ECS/GameObject.h"
#include "DUOLCommon/MetaDataType.h"

#include <rttr/registration>

using namespace rttr;

RTTR_PLUGIN_REGISTRATION
{
	rttr::registration::enumeration<DUOLGameEngine::OnClickEventFunctionMode>("OnClickEventFunctionMode")
	(
		value("VoidVoid", DUOLGameEngine::OnClickEventFunctionMode::VoidVoid)
		, value("VoidBool", DUOLGameEngine::OnClickEventFunctionMode::VoidBool)
		, value("VoidInt", DUOLGameEngine::OnClickEventFunctionMode::VoidInt)
		, value("VoidString", DUOLGameEngine::OnClickEventFunctionMode::VoidString)
		, value("VoidFloat", DUOLGameEngine::OnClickEventFunctionMode::VoidFloat)
	);
	rttr::registration::class_<DUOLGameEngine::OnClickArgument>("OnClickArgument")
	.constructor()
	(
		rttr::policy::ctor::as_raw_ptr
	)
	.constructor<DUOLGameEngine::GameObject*, const DUOLCommon::tstring&>()
	(
		rttr::policy::ctor::as_raw_ptr
	)
	.property("FunctionParameterName", &DUOLGameEngine::OnClickArgument::GetFunctionParameterName,&DUOLGameEngine::OnClickArgument::SetFunctionName)
	(
		metadata(DUOLCommon::MetaDataType::Serializable, true)
	)
	.property("IntParameter", &DUOLGameEngine::OnClickArgument::GetIntParameter,&DUOLGameEngine::OnClickArgument::SetIntParameter)
	(
		metadata(DUOLCommon::MetaDataType::Serializable, true)
	)
	.property("BoolParameter", &DUOLGameEngine::OnClickArgument::GetBoolParameter,&DUOLGameEngine::OnClickArgument::SetBoolParameter)
	(
		metadata(DUOLCommon::MetaDataType::Serializable, true)
	)
	.property("StringParameter", &DUOLGameEngine::OnClickArgument::GetStringParameter,&DUOLGameEngine::OnClickArgument::SetStringParameter)
	(
		metadata(DUOLCommon::MetaDataType::Serializable, true)
	)
	.property("FloatParameter", &DUOLGameEngine::OnClickArgument::GetFloatParameter,&DUOLGameEngine::OnClickArgument::SetFloatParameter)
	(
		metadata(DUOLCommon::MetaDataType::Serializable, true)
	)
	.property("FunctionName", &DUOLGameEngine::OnClickArgument::_functionName)
	(
		metadata(DUOLCommon::MetaDataType::Serializable, true)
	)
	.property("ParameterName", &DUOLGameEngine::OnClickArgument::_paramterName)
	(
		metadata(DUOLCommon::MetaDataType::Serializable, true)
	)
	.property("InputParameter", &DUOLGameEngine::OnClickArgument::GetInputParameter,&DUOLGameEngine::OnClickArgument::SetInputParameter)
	(
		metadata(DUOLCommon::MetaDataType::Serializable, true)
	)
	.property("FunctionMode", &DUOLGameEngine::OnClickArgument::_methodID)
	(
		metadata(DUOLCommon::MetaDataType::Serializable, true)
	);
}

DUOLGameEngine::OnClickArgument::OnClickArgument() :
	DUOLGameEngine::ObjectBase(TEXT("OnClickArgument"), ObjectType::UI)
	, _functionParameterName(L"No Function")
{
}


// OnClick이 실행될때 불리는 함수
void DUOLGameEngine::OnClickArgument::Invoke()
{
	//if (_targetComponentBase == nullptr)
	//	return;

	//instance obj = *_targetComponentBase;

	//const type class_type = obj.get_derived_type();

	//switch (_methodID)
	//{
	//case DUOLGameEngine::OnClickEventFunctionMode::VoidVoid:
	//	{
	//	_onClickVoidVoid->invoke(obj);
	//		break;
	//	}
	//case DUOLGameEngine::OnClickEventFunctionMode::VoidInt:
	//{

	//	_onClickVoidInt->invoke(obj,_intParameter);
	//	break;
	//}
	//case DUOLGameEngine::OnClickEventFunctionMode::VoidBool:
	//{
	//	_onClickVoidBool->invoke(obj, _boolParameter);
	//	break;
	//}
	//case DUOLGameEngine::OnClickEventFunctionMode::VoidString:
	//{
	//	_onClickVoidString->invoke(obj, "start");
	//	break;
	//}
	//case DUOLGameEngine::OnClickEventFunctionMode::VoidFloat:
	//{
	//	_onClickVoidFloat->invoke(obj, _floatParameter);
	//	break;
	//}
	//default:
	//	{
	//	//_onClickVoidVoid->invoke(obj);
	//	break;
	//	}
	//	break;
	//}
}

DUOLGameEngine::OnClickArgument::OnClickArgument(DUOLGameEngine::GameObject* owner, const DUOLCommon::tstring& name) :
	DUOLGameEngine::ObjectBase(name, ObjectType::UI)
	, _functionParameterName(L"No Function")
{
}

DUOLGameEngine::OnClickArgument::~OnClickArgument()
{
}

void DUOLGameEngine::OnClickArgument::OnUpdate(float deltaTime)
{

}

void DUOLGameEngine::OnClickArgument::Initialize()
{
}

void DUOLGameEngine::OnClickArgument::PushBackFunction(std::string& functionname,
	std::string& parameter)
{
	_functions.emplace_back(std::make_pair(functionname, parameter));

}

void DUOLGameEngine::OnClickArgument::ResetFunction()
{
	_functions.clear();
}
