#include "DUOLGameEngine/ECS/Component/RendererBase.h"

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

	void RendererBase::SetMaterial(const DUOLCommon::tstring& name)
	{
		// TODO : 조금 위험한 코드다.
		_materials.push_back(DUOLGameEngine::ResourceManager::GetInstance()->GetMaterialByID(name));
	}
}