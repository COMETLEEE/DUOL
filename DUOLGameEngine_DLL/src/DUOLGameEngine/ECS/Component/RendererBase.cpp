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
	)
		.property("EnableGPUInstancing", &DUOLGameEngine::RendererBase::GetGpuInstancing, &DUOLGameEngine::RendererBase::SetGPUInstancing)
	(
	metadata(DUOLCommon::MetaDataType::Serializable, true)
	, metadata(DUOLCommon::MetaDataType::Inspectable, true)
	, metadata(DUOLCommon::MetaDataType::InspectType, DUOLCommon::InspectType::Bool)
	)
	.property("Material", &DUOLGameEngine::RendererBase::_materials)
	(
	metadata(DUOLCommon::MetaDataType::Serializable, true)
	, metadata(DUOLCommon::MetaDataType::SerializeByString, true)
	, metadata(DUOLCommon::MetaDataType::Inspectable, true)
	, metadata(DUOLCommon::MetaDataType::InspectType, DUOLCommon::InspectType::Material)
	);
}

namespace DUOLGameEngine
{
	RendererBase::RendererBase() :
		BehaviourBase(nullptr, TEXT("RendererBase"))
		, _transformInfo(DUOLGraphicsEngine::Transform())
		, _renderEventHandlerIDForGraphics(UINT64_MAX)
		, _currentSelectedMaterial(0)
	{

	}

	RendererBase::RendererBase(DUOLGameEngine::GameObject* owner, const DUOLCommon::tstring& name) :
		BehaviourBase(owner, name)
		, _transformInfo(DUOLGraphicsEngine::Transform())
		, _renderEventHandlerIDForGraphics(UINT64_MAX)
		, _currentSelectedMaterial(0)
	{

	}

	RendererBase::~RendererBase()
	{
		EventManager::GetInstance()->RemoveEventFunction<void>(TEXT("SceneRendering"), _renderEventHandlerIDForGraphics);
	}

	void RendererBase::SetCurrentSelectedMaterial(unsigned currentSelectedMaterial)
	{
		_currentSelectedMaterial = currentSelectedMaterial;
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

	void RendererBase::DeleteBackMaterial()
	{
		if (_materials.empty())
			return;

		_materials.pop_back();

		_primitiveMaterials.pop_back();
	}

	void RendererBase::DeleteAllMaterial()
	{
		while (!_materials.empty())
			DeleteBackMaterial();
	}

	void RendererBase::OnEnable()
	{
		std::function<void()> functor = std::bind(&RendererBase::Render, this);

		if (_renderEventHandlerIDForGraphics == UINT64_MAX)
			_renderEventHandlerIDForGraphics = EventManager::GetInstance()->AddEventFunction(TEXT("SceneRendering"), functor);

		// Primitive Material 초기화. 다시 넣자.
		_primitiveMaterials.clear();

		for (auto mat : _materials)
		{
			if (mat)
				_primitiveMaterials.push_back(mat->GetPrimitiveMaterial());
		}
	}

	void RendererBase::OnDisable()
	{
		EventManager::GetInstance()->RemoveEventFunction<void>(TEXT("SceneRendering"), _renderEventHandlerIDForGraphics);

		_renderEventHandlerIDForGraphics = UINT64_MAX;
	}

	void RendererBase::Render()
	{
		// 내용은 아래의 렌더러에서 채우자.
	}

	void RendererBase::SetAlbedo(DUOLMath::Vector3 albedo)
	{
		if (_materials.empty() || !_materials[_currentSelectedMaterial]) return;

		DUOLMath::Vector4 input = DUOLMath::Vector4{ albedo.x, albedo.y, albedo.z, 1.f };

		_materials[_currentSelectedMaterial]->SetAlbedo(input);
	}

	void RendererBase::SetMetallic(float value)
	{
		if (_materials.empty() || !_materials[_currentSelectedMaterial]) return;

		_materials[_currentSelectedMaterial]->SetMetaillic(value);
	}

	void RendererBase::SetRoughness(float value)
	{
		if (_materials.empty() || !_materials[_currentSelectedMaterial]) return;

		_materials[_currentSelectedMaterial]->SetRoughness(value);
	}

	void RendererBase::SetGPUInstancing(bool value)
	{
		if (_materials.empty() || !_materials[_currentSelectedMaterial])return;

		_materials[_currentSelectedMaterial]->SetGPUInstancing(value);
	}

	const DUOLCommon::tstring& RendererBase::GetMaterialName()
	{
		if (_materials.empty() || !_materials[_currentSelectedMaterial])return _T("Empty");

		return _materials[_currentSelectedMaterial]->GetName();
	}

	bool RendererBase::GetGpuInstancing()
	{
		if (_materials.empty() || !_materials[_currentSelectedMaterial]) return false;

		return _materials[_currentSelectedMaterial]->GetGPUInstancing();
	}

	DUOLMath::Vector3 RendererBase::GetAlbedo()
	{
		if (_materials.empty() || !_materials[_currentSelectedMaterial])return DUOLMath::Vector3::Zero;

		auto matColor = _materials[_currentSelectedMaterial]->GetAlbedo();
		DUOLMath::Vector3 output =
		{
			matColor.x, matColor.y, matColor.z
		};

		return output;

	}

	float RendererBase::GetMetallic()
	{
		if (_materials.empty() || !_materials[_currentSelectedMaterial])return 0.f;

		return _materials[_currentSelectedMaterial]->GetMetallic();
	}

	float RendererBase::GetRoughness()
	{
		if (_materials.empty() || !_materials[_currentSelectedMaterial])return 0.f;

		return _materials[_currentSelectedMaterial]->GetRoughness();
	}
}
