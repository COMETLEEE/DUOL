#include "RendererBase.h"
#include "Transform.h"

namespace Muscle
{
	RendererBase::RendererBase(std::shared_ptr<GameObject> _GameObject) :IComponents(_GameObject)
	{
		_transform = m_GameObject.lock()->GetComponent<Transform>();
		_renderingData = std::make_shared<MuscleGrapics::RenderingData_3D>();
	}

	RendererBase::~RendererBase()
	{
		_renderingData.reset();
		_transform.reset();
	}

	void RendererBase::Start()
	{
		// �Ͼ �� �� ������Ʈ�� �����ϴ� ���� ���
		//SceneManager::Get()->GetCurrentScene()->AddRenderer(this->shared_from_this());
	}
}