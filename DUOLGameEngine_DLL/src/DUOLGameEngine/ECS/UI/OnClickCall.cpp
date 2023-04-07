#include "DUOLGameEngine/ECS/UI//OnClickCall.h"
#include "DUOLGameEngine/ECS/GameObject.h"
#include "DUOLCommon/MetaDataType.h"

#include <rttr/registration>

#include "DUOLGameEngine/ECS/UI/OnClickArgument.h"

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
	.property("GameObject", &DUOLGameEngine::OnClickCall::GetTargetObject,&DUOLGameEngine::OnClickCall::SetTargetGameObject)
	(
		metadata(DUOLCommon::MetaDataType::Serializable, true)
		, metadata(DUOLCommon::MetaDataType::Inspectable, true)
		, metadata(DUOLCommon::MetaDataType::SerializeByUUID, true)
		, metadata(DUOLCommon::MetaDataType::InspectType, DUOLCommon::InspectType::UIFileName)
	);
}

DUOLGameEngine::OnClickCall::OnClickCall() :
	DUOLGameEngine::ObjectBase(TEXT("OnClickCall"), ObjectType::UI)
	, _owner(nullptr)
	, _targetGameObject(nullptr)
{
}

DUOLGameEngine::OnClickCall::OnClickCall(DUOLGameEngine::GameObject* owner, const DUOLCommon::tstring& name) :
	DUOLGameEngine::ObjectBase(name, ObjectType::UI)
	, _owner(owner)
	, _targetGameObject(nullptr)
{
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
}
