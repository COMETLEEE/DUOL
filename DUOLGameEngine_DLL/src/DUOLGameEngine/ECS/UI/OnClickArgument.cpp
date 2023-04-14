#include "DUOLGameEngine/ECS/UI//OnClickArgument.h"
#include "DUOLGameEngine/ECS/GameObject.h"
#include "DUOLCommon/MetaDataType.h"

#include <rttr/registration>

using namespace rttr;

RTTR_PLUGIN_REGISTRATION
{
	rttr::registration::class_<DUOLGameEngine::OnClickArgument>("OnClickArgument")
	.constructor()
	(
		rttr::policy::ctor::as_raw_ptr
	)
	.constructor<DUOLGameEngine::GameObject*, const DUOLCommon::tstring&>()
	(
		rttr::policy::ctor::as_raw_ptr
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
