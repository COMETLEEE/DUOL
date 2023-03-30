#include "DUOLGameEngine/ECS/Component/RendererBase.h"

#include "DUOLGameEngine/ECS/Object/Material.h"
#include "DUOLGameEngine/Manager/EventManager.h"

#include "DUOLGameEngine/Manager/GraphicsManager.h"
#include "DUOLGameEngine/Manager/ResourceManager.h"

#include "DUOLGameEngine/ECS/GameObject.h"

#include <rttr/registration>
#include "DUOLCommon/MetaDataType.h"

using namespace rttr;

RTTR_PLUGIN_REGISTRATION
{
	rttr::registration::class_<DUOLGameEngine::RendererBase>("RendererBase")
	.constructor()
	(
		rttr::policy::ctor::as_raw_ptr
	)
	.constructor<DUOLGameEngine::GameObject*, const DUOLCommon::tstring&>()
	(
		rttr::policy::ctor::as_raw_ptr
	)
	.property("_materials", &DUOLGameEngine::RendererBase::_materials)
	(
		metadata(DUOLCommon::MetaDataType::Serializable, true)
		, metadata(DUOLCommon::MetaDataType::SerializeByString, true)
		, metadata(DUOLCommon::MetaDataType::MappingType, DUOLCommon::MappingType::Resource)
	)
	.property("Albedo", &DUOLGameEngine::RendererBase::GetAlbedo, &DUOLGameEngine::RendererBase::SetAlbedo)
		(
			metadata(DUOLCommon::MetaDataType::Serializable, true)
			, metadata(DUOLCommon::MetaDataType::Inspectable, true)
			, metadata(DUOLCommon::MetaDataType::InspectType, DUOLCommon::InspectType::Color)
		)
	.property("Roughness", &DUOLGameEngine::RendererBase::GetRoughness, &DUOLGameEngine::RendererBase::SetRoughness)
	(
	metadata(DUOLCommon::MetaDataType::Serializable, true)
	, metadata(DUOLCommon::MetaDataType::Inspectable, true)
	, metadata(DUOLCommon::MetaDataType::InspectType, DUOLCommon::InspectType::Float)
	)
	.property("Metallic", &DUOLGameEngine::RendererBase::GetMetallic, &DUOLGameEngine::RendererBase::SetMetallic)
	(
	metadata(DUOLCommon::MetaDataType::Serializable, true)
	, metadata(DUOLCommon::MetaDataType::Inspectable, true)
	, metadata(DUOLCommon::MetaDataType::InspectType, DUOLCommon::InspectType::Float)
	);
}

namespace DUOLGameEngine
{
	RendererBase::RendererBase() :
		BehaviourBase(nullptr, TEXT("RendererBase"))
		, _transformInfo(DUOLGraphicsEngine::Transform())
	{
	}

	RendererBase::RendererBase(DUOLGameEngine::GameObject* owner, const DUOLCommon::tstring& name) :
		BehaviourBase(owner, name)
		, _transformInfo(DUOLGraphicsEngine::Transform())
	{

	}

	RendererBase::~RendererBase()
	{
		EventManager::GetInstance()->RemoveEventFunction<void>(TEXT("SceneRendering"), _renderEventHandlerIDForGraphics);
	}

	std::vector<DUOLGameEngine::Material*> RendererBase::GetMaterials()
	{
		return _materials;
	}

	void RendererBase::AddMaterial(DUOLGameEngine::Material* material)
	{
		if (material == nullptr)
			return;

		_materials.push_back(material);

		// TODO : 이렇게 관리하고 싶지 않은데 어떻게 방법 없을까요 ..? => RenderObject에 미리 셋팅한다던가 ..
		_primitiveMaterials.push_back(material->GetPrimitiveMaterial());
	}

	void RendererBase::OnEnable()
	{
		std::function<void()> functor = std::bind(&RendererBase::Render, this);

		_renderEventHandlerIDForGraphics = EventManager::GetInstance()->AddEventFunction(TEXT("SceneRendering"), functor);

		// Primitive Material 초기화
		_primitiveMaterials.clear();

		for (auto mat : _materials)
		{
			_primitiveMaterials.push_back(mat->GetPrimitiveMaterial());
		}
	}

	void RendererBase::OnDisable()
	{
		EventManager::GetInstance()->RemoveEventFunction<void>(TEXT("SceneRendering"), _renderEventHandlerIDForGraphics);
	}

	void RendererBase::Render()
	{
	}

	void RendererBase::SetAlbedo(DUOLMath::Vector3 albedo)
	{
		DUOLMath::Vector4 input = DUOLMath::Vector4{albedo.x, albedo.y, albedo.z, 1.f};

		_materials[0]->SetAlbedo(input);
	}

	void RendererBase::SetMetallic(float value)
	{
		_materials[0]->SetMetaillic(value);
	}

	void RendererBase::SetRoughness(float value)
	{
		_materials[0]->SetRoughness(value);
	}

	DUOLMath::Vector3 RendererBase::GetAlbedo()
	{
		auto matColor = _materials[0]->GetAlbedo();
		DUOLMath::Vector3 output =
		{
			matColor.x, matColor.y, matColor.z
		};

		return output;

	}

	float RendererBase::GetMetallic()
	{
		return _materials[0]->GetMetallic();
	}

	float RendererBase::GetRoughness()
	{
		return _materials[0]->GetRoughness();
	}
}
