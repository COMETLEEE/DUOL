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

{
}

DUOLGameEngine::OnClickArgument::OnClickArgument(DUOLGameEngine::GameObject* owner, const DUOLCommon::tstring& name) :
	DUOLGameEngine::ObjectBase(name, ObjectType::UI)
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
