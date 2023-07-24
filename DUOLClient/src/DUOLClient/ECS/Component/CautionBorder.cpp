#include "DUOLClient/ECS/Component/CautionBorder.h"

#include "DUOLGameEngine/ECS/GameObject.h"
#include "DUOLGameEngine/ECS/Component/MeshRenderer.h"
#include "DUOLGameEngine/ECS/Component/Transform.h"
#include "DUOLGameEngine/ECS/Object/Material.h"

#include <rttr/registration>


using namespace rttr;

RTTR_REGISTRATION
{
	rttr::registration::class_<DUOLClient::CautionBorder>("CautionBorder")
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
	);

}


namespace DUOLGameEngine
{
	class MeshRenderer;
}

namespace DUOLClient
{
	CautionBorder::CautionBorder(DUOLGameEngine::GameObject* owner, const DUOLCommon::tstring& name):
		BehaviourBase(owner, name)
	{
	}

	CautionBorder::~CautionBorder()
	{
	}

	void CautionBorder::OnUpdate(float deltaT)
	{
		auto offset = _lowerline->GetOffset() + DUOLMath::Vector2{ -1 * deltaT, 0.f };
		if (offset.x < -1.f)
			offset.x += 1.f;
		_lowerline->SetOffset(offset);

		offset = _upperline->GetOffset() + DUOLMath::Vector2{ 1 * deltaT, 0.f };
		if (offset.x > 2.f)
			offset.x -= 1.f;
		_upperline->SetOffset(offset);

		offset = _cautionline->GetOffset() + DUOLMath::Vector2{ -1 * deltaT, 0.f };
		if (offset.x < -1.f)
			offset.x += 1.f;
		_cautionline->SetOffset(offset);
	}

	void CautionBorder::OnAwake()
	{
		auto children = GetTransform()->GetChildren();

		for (auto& child : children)
		{
			auto go = child->GetGameObject();

			if (go->GetName() == TEXT("LowerStripe"))
			{
				_lowerline = go->GetComponent<DUOLGameEngine::MeshRenderer>()->GetMaterials()[0];
				_meshObjs.push_back(go);
			}
			else if (go->GetName() == TEXT("Caution"))
			{
				_cautionline = go->GetComponent<DUOLGameEngine::MeshRenderer>()->GetMaterials()[0];
				go->GetTransform()->SetLocalScale(DUOLMath::Vector3{ 0.999f, 0.999f, 0.999f });
				_meshObjs.push_back(go);
			}
			else if (go->GetName() == TEXT("UpperStripe"))
			{
				_upperline = go->GetComponent<DUOLGameEngine::MeshRenderer>()->GetMaterials()[0];
				_meshObjs.push_back(go);
			}
		}
	}

	void CautionBorder::OnStart()
	{
		auto children = GetTransform()->GetChildren();

		for (auto& child : children)
		{
			auto go = child->GetGameObject();

			if (go->GetName() == TEXT("LowerStripe"))
			{
				_lowerline = go->GetComponent<DUOLGameEngine::MeshRenderer>()->GetMaterials()[0];
			}
			else if (go->GetName() == TEXT("Caution"))
			{
				_cautionline = go->GetComponent<DUOLGameEngine::MeshRenderer>()->GetMaterials()[0];
			}
			else if (go->GetName() == TEXT("UpperStripe"))
			{
				_upperline = go->GetComponent<DUOLGameEngine::MeshRenderer>()->GetMaterials()[0];
			}
		}
	}

	void CautionBorder::SetBorderOnlyMesh(bool value)
	{
			auto children = GetTransform()->GetChildGameObjects();

			for(auto& obj : children)
			{
				obj->SetIsActiveSelf(value);
			}
	}
}
