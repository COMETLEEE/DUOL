#include "DUOLClient/ECS/Component/Enemy/EnemyParentObjectObserver.h"
#include <rttr/registration>
#include "DUOLCommon/MetaDataType.h"
#include "DUOLGameEngine/ECS/GameObject.h"

using namespace rttr;

RTTR_REGISTRATION
{
rttr::registration::class_<DUOLClient::EnemyParentObjectObserver>("EnemyParentObjectObserver")
	.constructor()
	(
		rttr::policy::ctor::as_raw_ptr
	)
	.constructor<DUOLGameEngine::GameObject*, const DUOLCommon::tstring&>()
	(
		rttr::policy::ctor::as_raw_ptr
	);

}

namespace DUOLClient
{
	EnemyParentObjectObserver::EnemyParentObjectObserver(DUOLGameEngine::GameObject* owner,
		const DUOLCommon::tstring& name) : DUOLGameEngine::MonoBehaviourBase(owner, name),
		_enemyOwner(nullptr)
	{
	}
}
