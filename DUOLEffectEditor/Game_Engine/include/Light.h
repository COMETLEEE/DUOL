#pragma once
#include "IComponents.h"
#include "export/RenderingData.h"

namespace Muscle
{

	class Light : public IComponents
	{
	public:
		Light(std::shared_ptr<GameObject> _GameObject);
		~Light();

	public:
		virtual void Start() override;

		virtual void Update() override; // 모든 Object Udpate돌릴 때 (ObjectManager) 오브젝트가 Components를 가지고 있으면 같이 업데이트 해주기.

		virtual void Render() override; // 모든 Object 렌더 돌릴 때 (ObjectManager) 오브젝트가 Components를 가지고 있으면 같이 렌더

	public:
		MuscleGrapics::LightInfo _lightInfo;
	};
}