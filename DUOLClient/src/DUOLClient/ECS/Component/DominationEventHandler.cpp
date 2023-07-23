#include "DUOLClient/ECS/Component/DominationEventHandler.h"

#include "DUOLGameEngine/ECS/GameObject.h"
#include "DUOLGameEngine/ECS/Component/Transform.h"
#include <rttr/registration>

#include "DUOLClient/Manager/SystemManager.h"
#include "DUOLCommon/MetaDataType.h"
#include "DUOLGameEngine/Manager/SceneManagement/SceneManager.h"

using namespace rttr;

RTTR_REGISTRATION
{
	rttr::registration::class_<DUOLClient::DominationEventHandler>("DominationEventHandler")
	.constructor()
	(
		rttr::policy::ctor::as_raw_ptr
	)
	.constructor<DUOLGameEngine::GameObject*, const DUOLCommon::tstring&>()
	(
		rttr::policy::ctor::as_raw_ptr
	)
	.property("_isDominateAllArea", &DUOLClient::DominationEventHandler::_isDominateAllArea)
	(
		metadata(DUOLCommon::MetaDataType::Inspectable, true)
		, metadata(DUOLCommon::MetaDataType::InspectType, DUOLCommon::InspectType::Bool)
		, metadata(DUOLCommon::MetaDataType::Serializable, true)
	);
}


namespace DUOLClient
{
	DUOLClient::DominationEventHandler::DominationEventHandler(DUOLGameEngine::GameObject* owner,
		const DUOLCommon::tstring& name) :
		BehaviourBase(owner, name),
		_isDominateAllArea(false)
	{
	}

	DUOLClient::DominationEventHandler::~DominationEventHandler()
	{
	}

	void DUOLClient::DominationEventHandler::OnAwake()
	{
		auto children = GetTransform()->GetChildGameObjects();
		_dominationAreas.reserve(children.size());

		for (auto& child : children)
		{
			auto dominationArea = child->GetComponent<DominationArea>();
			_dominationAreas.push_back(dominationArea);
		}



	}

	void DominationEventHandler::OnUpdate(float deltaf)
	{
		//굳이 매프레임 체크할 필요는 없을 것 같다.
		if (_isDominateAllArea == false)
		{
			//체크시작
			for (auto& dominationArea : _dominationAreas)
			{
				_isDominateAllArea = dominationArea->IsCleared();

				if (!_isDominateAllArea)
				{
					_isDominateAllArea = false;
					break;
				}
			}

			//성공했을때, zone의 파괴율상승을 멈춘다.
			if(_isDominateAllArea == true)
			{
				for (auto& dominationArea : _dominationAreas)
				{
					dominationArea->SetTotallyClear(true);
				}
				//성공이벤트 호출!

				SystemManager::SetAstageClear(true);
				SystemManager::GetInstance()->MiniMapChange(L"StageAMiniMapPortal.png");
				SystemManager::GetInstance()->CreatePortal(TEXT("AClearPortal"), TEXT("Middle"), A_CLEAR_PORTAL_TO_MIDDLE_POSITION);
			}
		}
		else
		{
			//성공이벤트 지속호출! 
		}

	}

}
