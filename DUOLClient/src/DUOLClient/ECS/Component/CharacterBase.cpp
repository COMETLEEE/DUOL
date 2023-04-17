#include "DUOLClient/ECS/Component/CharacterBase.h"
#include "DUOLGameEngine/ECS/GameObject.h"
#include <rttr/registration>
#include "DUOLCommon/MetaDataType.h"

using namespace rttr;

RTTR_REGISTRATION
{
	rttr::registration::class_<DUOLClient::CharacterBase>("CharacterBase")
	.property("_hp",&DUOLClient::CharacterBase::_hp)
	(
			metadata(DUOLCommon::MetaDataType::Serializable, true)
		, metadata(DUOLCommon::MetaDataType::Inspectable, true)
		, metadata(DUOLCommon::MetaDataType::InspectType, DUOLCommon::InspectType::Float))
	.property("_damage",&DUOLClient::CharacterBase::_damage)
	(
			metadata(DUOLCommon::MetaDataType::Serializable, true)
		, metadata(DUOLCommon::MetaDataType::Inspectable, true)
		, metadata(DUOLCommon::MetaDataType::InspectType, DUOLCommon::InspectType::Float))
	.property("_currentSpeed",&DUOLClient::CharacterBase::_currentSpeed)
	(
			metadata(DUOLCommon::MetaDataType::Serializable, true)
		, metadata(DUOLCommon::MetaDataType::Inspectable, true)
		, metadata(DUOLCommon::MetaDataType::InspectType, DUOLCommon::InspectType::Float));


}

DUOLClient::CharacterBase::CharacterBase(DUOLGameEngine::GameObject* owner, const DUOLCommon::tstring& name) :
	DUOLGameEngine::MonoBehaviourBase(owner, name), _hp(0), _damage(0), _currentSpeed(0)
{}