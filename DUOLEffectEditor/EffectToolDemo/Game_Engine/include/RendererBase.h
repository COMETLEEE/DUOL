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
		// 이 컴포넌트를 가진 게임 오브젝트를 그릴 때 필요한 정보들. 게임 엔진에서는 이것을 조작한다.
		// 싹다 열어놓고 그냥 셋팅하자. (유니티 인스펙터 창 같은 감성)
		std::shared_ptr<MuscleGrapics::RenderingData_3D> _renderingData;

	protected:
		// Transform 캐싱
		std::shared_ptr<Transform> _transform;

	public:
		virtual void Start() override;
	};
}
