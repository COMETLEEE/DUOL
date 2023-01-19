#include <rttr/registration>

#include "DUOLGameEngine/ECS/ObjectBase.h"
#include "DUOLGameEngine/ECS/GameObject.h"
#include "DUOLGameEngine/ECS/Component/Transform.h"

#include "DUOLCommon/MetaDataType.h"

using namespace rttr;

RTTR_PLUGIN_REGISTRATION
{
	rttr::registration::class_<DUOLGameEngine::Transform>("Transform")
	.constructor<const std::weak_ptr<DUOLGameEngine::GameObject>&>()
	.property("_localPosition", &DUOLGameEngine::Transform::GetLocalPosition, &DUOLGameEngine::Transform::SetLocalPosition)
	(
		metadata(DUOLCommon::MetaDataType::Serializable, true)
		, metadata(DUOLCommon::MetaDataType::Draw_Vector3, true)
	)
	.property("_localEulerAngle", &DUOLGameEngine::Transform::GetLocalEulerAngle, &DUOLGameEngine::Transform::SetLocalEulerAngle)
	(
		metadata(DUOLCommon::MetaDataType::Serializable, true)
		, metadata(DUOLCommon::MetaDataType::Draw_Vector3, true)
	)
	.property("_localScale", &DUOLGameEngine::Transform::GetLocalScale, &DUOLGameEngine::Transform::SetLocalScale)
	(
		metadata(DUOLCommon::MetaDataType::Serializable, true)
		, metadata(DUOLCommon::MetaDataType::Draw_Vector3, true)
	);
}