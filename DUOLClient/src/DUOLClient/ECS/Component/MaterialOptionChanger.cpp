#include "DUOLClient/ECS/Component/MaterialOptionChanger.h"


#include <rttr/registration>

#include "DUOLCommon/MetaDataType.h"
#include "DUOLGameEngine/ECS/GameObject.h"
#include "DUOLGameEngine/ECS/Component/MeshRenderer.h"
#include "DUOLGameEngine/ECS/Object/Material.h"

using namespace rttr;

RTTR_REGISTRATION
{
	rttr::registration::class_<DUOLClient::MaterialOptionChanger>("MaterialOptionChanger")
	.constructor()
	(
		rttr::policy::ctor::as_raw_ptr
	)
	.constructor<DUOLGameEngine::GameObject*, const DUOLCommon::tstring&>()
	(
		rttr::policy::ctor::as_raw_ptr
	)
		.constructor<DUOLGameEngine::GameObject*, const DUOLCommon::tstring&>()
	(
		rttr::policy::ctor::as_raw_ptr
	)
	.property("materialIdx", &DUOLClient::MaterialOptionChanger::GetMatIdx, &DUOLClient::MaterialOptionChanger::SetMatIdx)
	(
		metadata(DUOLCommon::MetaDataType::Inspectable, true)
		, metadata(DUOLCommon::MetaDataType::InspectType, DUOLCommon::InspectType::Int)
		, metadata(DUOLCommon::MetaDataType::Serializable, true)
	)
	.property("tilingInfo", &DUOLClient::MaterialOptionChanger::_tilingInfo)
	(
		metadata(DUOLCommon::MetaDataType::Inspectable, true)
		, metadata(DUOLCommon::MetaDataType::InspectType, DUOLCommon::InspectType::Float2)
		, metadata(DUOLCommon::MetaDataType::Serializable, true)
	);
}

namespace DUOLClient
{
	MaterialOptionChanger::MaterialOptionChanger(DUOLGameEngine::GameObject* owner, const DUOLCommon::tstring& name) :
		BehaviourBase(owner, name)
		, _matIdx(0)
		, _tilingInfo(0, 0)
	{

	}

	MaterialOptionChanger::~MaterialOptionChanger()
	{
	}

	void MaterialOptionChanger::OnAwake()
	{
	}

	void MaterialOptionChanger::OnStart()
	{
		UpdateCurrentInfo();
	}

	void MaterialOptionChanger::UpdateCurrentInfo()
	{
		auto go = GetGameObject();

		if (go != nullptr)
		{
			auto meshRenderer = go->GetComponent<DUOLGameEngine::MeshRenderer>();
			if (meshRenderer != nullptr)
			{
				auto mats = meshRenderer->GetMaterials();

				for (int idx = 0; idx < mats.size(); idx++)
				{
					if (idx == _matIdx)
					{
						mats[idx]->SetTiling(_tilingInfo);
					}
				}
			}
		}
	}

	int MaterialOptionChanger::GetMatIdx() const
	{
		return _matIdx;
	}

	void MaterialOptionChanger::SetMatIdx(int matIdx)
	{
		_matIdx = matIdx;
	}
}
