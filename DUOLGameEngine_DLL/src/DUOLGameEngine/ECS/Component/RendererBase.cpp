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
	.constructor<const std::weak_ptr<DUOLGameEngine::GameObject>&, const DUOLCommon::tstring&>()
	(
		rttr::policy::ctor::as_raw_ptr
	);
}

namespace DUOLGameEngine
{
	RendererBase::RendererBase(const std::weak_ptr<DUOLGameEngine::GameObject>& owner, const DUOLCommon::tstring& name) :
		BehaviourBase(owner, name)
		, _transformInfo(DUOLGraphicsEngine::Transform())
	{
		
	}

	RendererBase::~RendererBase()
	{
		
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

		// TODO : �̷��� �����ϰ� ���� ������ ��� ��� ������� ..? => RenderObject�� �̸� �����Ѵٴ��� ..
		_primitiveMaterials.push_back(material->GetPrimitiveMaterial());
	}

	void RendererBase::OnEnable()
	{
		std::function<void()> functor = std::bind(&RendererBase::Render, this);

		_renderEventHandlerIDForGraphics = EventManager::GetInstance()->AddEventFunction(TEXT("SceneRendering"), functor);
	}

	void RendererBase::OnDisable()
	{
		EventManager::GetInstance()->RemoveEventFunction<void>(TEXT("SceneRendering"), _renderEventHandlerIDForGraphics);
	}

	void RendererBase::Render()
	{
	}
}