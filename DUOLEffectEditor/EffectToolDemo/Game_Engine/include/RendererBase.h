#pragma once
#include "IComponents.h"
#include "Export\RenderingData.h"


namespace Muscle
{
	class Transform;

	class RendererBase : public IComponents, public std::enable_shared_from_this<RendererBase>
	{
	protected:
		RendererBase(std::shared_ptr<GameObject> _GameObject);

		virtual ~RendererBase();

	public:
		// �� ������Ʈ�� ���� ���� ������Ʈ�� �׸� �� �ʿ��� ������. ���� ���������� �̰��� �����Ѵ�.
		// �ϴ� ������� �׳� ��������. (����Ƽ �ν����� â ���� ����)
		std::shared_ptr<MuscleGrapics::RenderingData_3D> _renderingData;

	protected:
		// Transform ĳ��
		std::shared_ptr<Transform> _transform;

	public:
		virtual void Start() override;
	};
}
