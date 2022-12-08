#include "DUOLGameEngine/ECS/Component/RendererBase.h"

#include "DUOLGameEngine/ECS/Object/Material.h"
#include "DUOLGameEngine/Manager/ResourceManager.h"

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

		// TODO : 이렇게 관리하고 싶지 않은데 어떻게 방법 없을까요 ..? => RenderObject에 미리 셋팅한다던가 ..
		_primitiveMaterials.push_back(material->GetPrimitiveMaterial());
	}
}