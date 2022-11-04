#include "pch.h"
#include "RendererBase.h"


namespace Muscle
{
	RendererBase::RendererBase(std::shared_ptr<GameObject> _GameObject) :IComponents(_GameObject)
	{
		_transform = m_GameObject.lock()->GetComponent<Transform>();
		_renderingData = std::make_shared<RenderingData_3D>();
	}

	RendererBase::~RendererBase()
	{
		_renderingData.reset();
	}

	void RendererBase::Start()
	{
		// 일어날 때 이 컴포넌트가 존재하는 씬에 등록
		//SceneManager::Get()->GetCurrentScene()->AddRenderer(this->shared_from_this());
	}
}