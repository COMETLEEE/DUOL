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
}