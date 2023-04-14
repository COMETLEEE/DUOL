#include "DUOLGameEngine/ECS/UI//OnClick.h"
#include "DUOLGameEngine/ECS/GameObject.h"
#include "DUOLCommon/MetaDataType.h"

#include <rttr/registration>

#include "DUOLGameEngine/ECS/UI/OnClickCall.h"


using namespace rttr;

RTTR_PLUGIN_REGISTRATION
{
	rttr::registration::enumeration<DUOLGameEngine::BUTTONCALLBACKSTATE>("ButtonCallBackType")
	(
		value("Off", DUOLGameEngine::BUTTONCALLBACKSTATE::Off)
		, value("EditorAndRunTime", DUOLGameEngine::BUTTONCALLBACKSTATE::EditorAndRunTime)
		, value("RuntimeOnly", DUOLGameEngine::BUTTONCALLBACKSTATE::RuntimeOnly)
	);
	rttr::registration::class_<DUOLGameEngine::OnClick>("OnClick")
	.constructor()
	(
		rttr::policy::ctor::as_raw_ptr
	)
	.constructor<DUOLGameEngine::GameObject*, const DUOLCommon::tstring&>()
	(
		rttr::policy::ctor::as_raw_ptr
	)
	.property("CallBackType", &DUOLGameEngine::OnClick::GetCallBackState,&DUOLGameEngine::OnClick::SetCallBackState)
	(
		metadata(DUOLCommon::MetaDataType::Serializable, true)
		,metadata(DUOLCommon::MetaDataType::Inspectable, true)
		, metadata(DUOLCommon::MetaDataType::InspectType, DUOLCommon::InspectType::Enumeration)
	);
}


DUOLGameEngine::OnClick::OnClick() :
	DUOLGameEngine::ObjectBase(TEXT("OnClick"), ObjectType::UI)
	//, _persistentCall()
	, _callBackState(BUTTONCALLBACKSTATE::RuntimeOnly)
{
	Initialize();
}

DUOLGameEngine::OnClick::OnClick(DUOLGameEngine::GameObject* owner, const DUOLCommon::tstring& name) :
	DUOLGameEngine::ObjectBase(name, ObjectType::UI)
	//, _persistentCall()
	, _callBackState(BUTTONCALLBACKSTATE::RuntimeOnly)
{
	Initialize();

}

DUOLGameEngine::OnClick::~OnClick()
{
	// 해제 해준다.
	_persistentCall.reset();
}

void DUOLGameEngine::OnClick::OnUpdate(float deltaTime)
{
	if (_callBackState == BUTTONCALLBACKSTATE::Off)
		return;


	_persistentCall->OnUpdate(deltaTime);
}

void DUOLGameEngine::OnClick::Initialize()
{
	_persistentCall = std::make_shared<OnClickCall>();

}

void DUOLGameEngine::OnClick::OnAwake()
{
	_persistentCall->OnAwake();
}



void DUOLGameEngine::OnClick::Invoke()
{
	_persistentCall->Invoke();
}
